#ifndef _MESSAGE_BASE_H_
#define _MESSAGE_BASE_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MtDefine.h"
extern "C" {
#include "uv.h"
}

class CMessageBase
{
public:
	CMessageBase();
	CMessageBase(char *pMsg, int nMsgLen);
	virtual ~CMessageBase();

public:
	bool getMsgIdText();
	inline char* getMsgBuf() { return m_lpMsgBuf; }
	inline int getMsgLen() { return m_nMsgLen; }

	void setMsg(const char* p);

public:
	//test method
	void revert();
private:
	int							m_nMsgIdType;
	char						m_sMsgId[MESSAGE_BASE_MSGID_LEN + 1];
	int							m_nMsgType;
	char*						m_lpMsgBuf;
	int							m_nMsgLen;
	mt_routingnode_t*			m_lpRouterIn;
	mt_routingnode_t*			m_lpRouterOut;
	
public:
	static uv_mutex_t			m_sMsgIdMutex;			//
	static long					m_sgMsgId;
};

#endif
