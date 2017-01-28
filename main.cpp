#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <map>
#include <string>

#include "log4cpp/Category.hh"
#include "log4cpp/PropertyConfigurator.hh"

#include "MessageBase.h"
//#include "LoadPy.h"
#include "LoadLua.h"

#ifdef _MSC_VER 
#define setenv putenv
#endif

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

typedef std::map<std::string, uv_key_t* > MT_THREAD_KEY;

struct MT_req_t {
	uv_buf_t			buf;
	int					nCurLen;
	uv_tcp_t*			client;
	struct MT_rsp_t*	corrRsp;
};

struct MT_rsp_t {
	uv_write_t			rsp;
	uv_buf_t			buf;
	struct MT_req_t*	corrReq;
};

log4cpp::Category * logger() {
	return &log4cpp::Category::getRoot();
}

bool init_logger(const char * ps = "./conf/log4cpp.conf") {
	try {
		log4cpp::PropertyConfigurator::configure(ps);
	}
	catch (log4cpp::ConfigureFailure& f) {
		std::cout << "configure problem: " << f.what() << std::endl;
		return false;
	}
	return true;
}

void free_MT_rsp(MT_rsp_t* rsp) {		//MTPanel request object
	rsp->corrReq = NULL;
	if (rsp->buf.len != 0) {
		free(rsp->buf.base);
		rsp->buf.len = 0;
		rsp->buf.base = NULL;
	}
}

void free_MT_req(MT_req_t * req) {		//MTPanel response object
	if (req->buf.len != 0) {
		req->buf.len = 0;
		free(req->buf.base);
		req->buf.base = NULL;
	}
	if (req->corrRsp != NULL) {
		free_MT_rsp(req->corrRsp);
		req->corrRsp = NULL;
	}
}

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
    CMessageBase *pMsg;
} write_req_t;

#ifdef _MSC_VER
void setLuaPath() {
}
#else
void setLuaPath(){
	char szLUA_PATH[2048];
	sprintf(szLUA_PATH, "%s;%s;%s;%s;%s", getenv("LUA_PATH"), "./lua/?.lua", "./conf/?.lua", "./lib/?.lua", "./lib/?.so");
	setenv("LUA_PATH", szLUA_PATH, 1);
	//printf("lua_path:%s\n", getenv("LUA_PATH"));
}
#endif

uv_key_t get_uv_key(const char* p) {
	static MT_THREAD_KEY thread_key;
	if (thread_key.find(p) == thread_key.end()) {
		uv_key_t * key = (uv_key_t *)malloc(sizeof(uv_key_t));
		uv_key_create(key);
		thread_key[p] = key;
	}
	return *thread_key[p];
}

void free_write_req(uv_write_t *req) {
	write_req_t *wr = (write_req_t*)req;
	free(wr->buf.base);
    delete wr->pMsg;
	free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void after_close(uv_handle_t * handle) {
	MT_req_t * mt_req = (MT_req_t*)(handle->data);
	free(handle);
	free_MT_req(mt_req);
}

void echo_write(uv_write_t *req, int status) {
	if (status) {
		fprintf(stderr, "Write error %s\n", uv_strerror(status));
	}
	free_write_req(req);
}

CLoadLua* getLoadLua(const char * psFile) {
	uv_key_t  key = get_uv_key(psFile);
	CLoadLua * pLua = (CLoadLua*)uv_key_get(&key);
	if (pLua == NULL) {
		pLua = new CLoadLua(psFile);
		if (pLua->init() == false) {
			//printf("pLua->init() error!\n");
			logger()->error("pLua->init() erorr!");
			delete pLua;
			return NULL;
		}
		uv_key_set(&key, pLua);
	}
	
	return pLua;
}

bool processByLua(CMessageBase * pMsg)
{
	CLoadLua * pLua = getLoadLua("./lua/LuaMain.lua");
	if (pLua == NULL) {
		return false;
	}
	char rsp[1024 * 30];
	logger()->info("befor process, req msg:[%s]", pMsg->getMsgBuf());
	pLua->call("entry", pMsg->getMsgBuf(), rsp);
	pMsg->setMsg(rsp);
	logger()->info("after process, rsp msg:[%s]", pMsg->getMsgBuf());
	return true;
}

void after_write(uv_write_t *rsp, int status) {
	MT_req_t * mt_req = ((MT_rsp_t *)rsp)->corrReq;
	uv_tcp_t * client = mt_req->client;
	uv_close((uv_handle_t*)client, after_close);
}

void business_logic(uv_work_t *req) {
	uv_tcp_t * client = (uv_tcp_t*)(req->data);
	MT_req_t * mt_req = (MT_req_t*)(client->data);
	MT_rsp_t ** mt_rsp = &(mt_req->corrRsp);
	uv_buf_t * reqBuf = &(mt_req->buf);
	*mt_rsp = (MT_rsp_t*)malloc(sizeof(MT_rsp_t));
	uv_buf_t * rspBuf = &((*mt_rsp)->buf);

	CMessageBase * pMsg = new CMessageBase(reqBuf->base, reqBuf->len);
	processByLua(pMsg);
	char *p = (char*)malloc(pMsg->getMsgLen());
	memcpy(p, pMsg->getMsgBuf(), pMsg->getMsgLen());
	*rspBuf = uv_buf_init(p, pMsg->getMsgLen());
	delete pMsg;
}
void after_business_logic(uv_work_t *req, int status) {
	uv_tcp_t * client = (uv_tcp_t*)(req->data);
	MT_req_t * mt_req = (MT_req_t*)(client->data);
	MT_rsp_t * mt_rsp = mt_req->corrRsp;
	mt_rsp->corrReq = mt_req;
	uv_write((uv_write_t*)mt_rsp, (uv_stream_t*)client, &(mt_rsp->buf), 1, after_write);
}

void after_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	MT_req_t **mt_req = (MT_req_t**)(&((uv_tcp_t*)client)->data);
	uv_buf_t *	reqBuf = NULL;
	int *		pnCurLen = NULL;
	int			nCurLen = nread;
	char*		pCurPos = buf->base;
	if (nread > 0) {
		if (*mt_req == NULL) {
			*mt_req = (MT_req_t*)malloc(sizeof(MT_req_t));
			(*mt_req)->nCurLen = 0;
			(*mt_req)->client = (uv_tcp_t*)client;
			buf->base[nread] = 0;
			std::string tmp = buf->base;
			(*mt_req)->buf.len = atoi(tmp.substr(0, 8).c_str());
			(*mt_req)->buf.base = (char*)malloc((*mt_req)->buf.len);
		}
		reqBuf = &((*mt_req)->buf);
		pnCurLen = &((*mt_req)->nCurLen);
		memcpy(reqBuf->base + *pnCurLen, pCurPos, nCurLen);	// default head 8, limiting case, recv 0000 at first time, recv 1000 at second time , current ignore it
		*pnCurLen += nCurLen;
	}
	if (nread < 0) {
		/*if (nread != UV_EOF)
		fprintf(stderr, "Read error %s\n", uv_err_name(nread));*/
		//uv_close((uv_handle_t*)client, NULL);
		/*cautions*/
		return;
	}

	if (*pnCurLen == reqBuf->len) {
		logger()->info("received a complete request, put it in work queue!");
		uv_work_t * req = (uv_work_t*)malloc(sizeof (uv_work_t));
		req->data = (void*)client;
		uv_queue_work(loop, req, business_logic, after_business_logic);
		free(buf->base);
	}
}

/*
bool processByPy(CMessageBase * pMsg)
{
	CLoadPy * pL = new CLoadPy("D:/tmp/tPy/", "pytest");
	pL->init();

	char rst[3 * 1024 + 1];
	memset(rst, 0x00, sizeof(rst));
	pL->call("revert", rst, rst);
	pMsg->setMsg(rst);
	delete pL;

	return true;
}
*/

void on_new_connection(uv_stream_t *server, int status) {			//two arguments, 1. server:uv_stream_t, 2. status, callback on new connection signal
	if (status < 0) {
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		// error!
		return;
	}

	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	client->data = NULL;
	if (uv_accept(server, (uv_stream_t*)client) == 0) {
		uv_read_start((uv_stream_t*)client, alloc_buffer, after_read);	//start read, have two callback function, 
	}
	else {
		uv_close((uv_handle_t*)client, NULL);
	}
}

int main() {

	bool bInitLogger = init_logger();			//init logger
	if (bInitLogger == false) {
		printf("init_logger error!\n");
		return -1;
	}
	logger()->info("init_logger succ!");

	setLuaPath();		//adjust LUA_PATH

	loop = uv_default_loop();				//1. get default loop 

	uv_tcp_t server;						//2. init server
	uv_tcp_init(loop, &server);

	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);	//3. init addr

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);	//4. bind server & addr
	int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);		//5. start listen on server where new connection come callback on_new_connection
	if (r) {
		logger()->error("Listen error %s\n", uv_strerror(r));
		return 1;
	}
	logger()->info("Listen succ\n");
	uv_run(loop, UV_RUN_DEFAULT);
	system("pause");
	return 0;
}
