#ifndef _MT_DEFINE_H_
#define _MT_DEFINE_H_

#pragma pack (push , 1)

enum MsgIdType
{
	MSGIDTYPE_TEXT,
	MSGIDTYPE_BIN
};

enum MsgType
{
	MSGTYPE_TEXT,
	MSGTYPE_BIN
};

//COMMON start
#define FULL_PATH_LEN		256
#define FILE_NAME_LEN		50
#define FUNC_NAME_LEN		50
//COMMON end

//MESSAGE_BASE start
#define MESSAGE_BASE_MSGID_LEN 32
//MESSAGE_BASE end


//ROUTENODE start

#define ROUTENODE_NAME_LEN		30
#define ROUTENODE_CHNAME_LEN	50

typedef struct ROUTINGNODE {
	char				szName[ROUTENODE_NAME_LEN + 1];
	char				szChName[ROUTENODE_CHNAME_LEN + 1];
	char				szModule[FUNC_NAME_LEN];
	char				szPath[FILE_NAME_LEN];
}mt_routingnode_t;

//ROUTENODE end

#pragma pack (pop)

#endif