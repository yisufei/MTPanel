#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

/*
#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/BasicLayout.hh"
*/

#include "MessageBase.h"
//#include "LoadPy.h"
#include "LoadLua.h"

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

//4.new a category object
//log4cpp::Category& mlog = log4cpp::Category::getInstance("log");

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
    CMessageBase *pMsg;
} write_req_t;

void free_write_req(uv_write_t *req) {
	write_req_t *wr = (write_req_t*)req;
	free(wr->buf.base);
    delete wr->pMsg;
	free(wr);
}

void prepare_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
	//buf->base = (char*)malloc(1);
	//buf->len = 1;
	buf->base = (char*)malloc(suggested_size);					
	buf->len = suggested_size;				
}

void echo_write(uv_write_t *req, int status) {
	if (status) {
		fprintf(stderr, "Write error %s\n", uv_strerror(status));
	}
	free_write_req(req);
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

//bool processByLua(const char *pFileName, CMessageBase *pMsg)
bool processByLua(CMessageBase * pMsg)
{
	//CLoadLua* pLua = new CLoadLua("./lua/LuaMain.lua");
	CLoadLua* pLua = new CLoadLua("./lua/LuaMain.lua");
	bool bInit = pLua->init();
    //printf("bInit:%s\n", bInit==true?"true":"false");
	char rst[1024 * 20];
	sprintf(rst, (const char*)pMsg->getMsgBuf());
	pLua->call("entry", rst, rst);
	pMsg->setMsg(rst);
	delete pLua;

	return true;
}

void data_received_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	//printf("run... nread:[%d]\n", nread);
	if (nread > 0) {
		write_req_t *req = (write_req_t*)malloc(sizeof(write_req_t));
		req->buf = uv_buf_init(buf->base, nread);
		req->pMsg = new CMessageBase(req->buf.base, req->buf.len);
		//req->pMsg->revert();
		//processByPy(pMsg);
		processByLua(req->pMsg);
		req->buf = uv_buf_init(req->pMsg->getMsgBuf(), req->pMsg->getMsgLen());
		uv_write((uv_write_t*)req, client, &req->buf, 1, echo_write);
		return;
	}
	if (nread < 0) {
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*)client, NULL);
	}

	free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {			//two arguments, 1. server:uv_stream_t, 2. status, callback on new connection signal
	if (status < 0) {
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		// error!
		return;
	}

	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(server, (uv_stream_t*)client) == 0) {
		uv_read_start((uv_stream_t*)client, prepare_buffer, data_received_cb);	//start read, have two callback function, 
	}
	else {
		uv_close((uv_handle_t*)client, NULL);
	}
}

int main() {
    /*
	//1.new a layout object
	log4cpp::Layout* layout = new log4cpp::BasicLayout();
	//2.init a appender object
	log4cpp::Appender* appender = new log4cpp::FileAppender("FileAppender", "./test.log");
	//3.attach layout object on appender object
	appender->setLayout(layout);
	//5. set additivity false, reset appender
	mlog.setAdditivity(false);
	//6.attach appender object on category
	mlog.setAppender(appender);
    */
    
    char szLUA_PATH[2048];
    sprintf(szLUA_PATH, "%s;%s;%s", getenv("LUA_PATH"), "./lua/?.lua", "./conf/?.lua");
    setenv("LUA_PATH", szLUA_PATH, 1);
    //printf("lua_path:%s\n", getenv("LUA_PATH"));

	loop = uv_default_loop();				//1. get default loop 

	uv_tcp_t server;						//2. init server
	uv_tcp_init(loop, &server);

	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);	//3. init addr

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);	//4. bind server & addr
	int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);		//5. start listen on server where new connection come callback on_new_connection
	if (r) {
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		//mlog.error("Listen error %s\n", uv_strerror(r));
		return 1;
	}
	//mlog.debug("Listen succ\n");

	uv_run(loop, UV_RUN_DEFAULT);
	system("pause");
	return 0;
}
