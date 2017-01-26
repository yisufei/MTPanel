#include "MtDefine.h"
#include "MessageBase.h"

extern "C" {
#include "uv.h"
}

long CMessageBase::m_sgMsgId = 0;
uv_mutex_t CMessageBase::m_sMsgIdMutex;


CMessageBase::CMessageBase()
{
	//init start 
	m_nMsgIdType = MSGIDTYPE_TEXT;
	m_nMsgType = MSGTYPE_TEXT;
	m_lpMsgBuf = NULL;
	//init end
}

CMessageBase::CMessageBase(char * pMsg, int nMsgLen)
{
	//init start
	m_nMsgIdType = MSGIDTYPE_TEXT;
	m_nMsgType = MSGTYPE_TEXT;
	m_lpMsgBuf = NULL;
	//init end
	m_nMsgLen = nMsgLen;
	m_lpMsgBuf = new char[m_nMsgLen + 1];
	snprintf(m_lpMsgBuf, m_nMsgLen + 1, "%s", pMsg);
}


CMessageBase::~CMessageBase()
{
	if (m_lpMsgBuf != NULL)
	{
		//free(m_lpMsgBuf);
		m_lpMsgBuf = NULL;
	}
}

bool CMessageBase::getMsgIdText()
{
	uv_mutex_lock(&m_sMsgIdMutex);
	sprintf(m_sMsgId, "%032ld", m_sgMsgId++);
	uv_mutex_unlock(&m_sMsgIdMutex);
	return true;
}


void CMessageBase::revert()
{
	if (m_lpMsgBuf == NULL) return;
	for (int i = 0; i < m_nMsgLen / 2; i++)
	{
		char c = m_lpMsgBuf[i];
		m_lpMsgBuf[i] = m_lpMsgBuf[m_nMsgLen - 1 - i];
		m_lpMsgBuf[m_nMsgLen - 1 - i] = c;
	}
}

void CMessageBase::setMsg(const char* p)
{
	if (m_lpMsgBuf != NULL) delete m_lpMsgBuf;
	m_nMsgLen = strlen(p);
	m_lpMsgBuf = new char[m_nMsgLen + 1];
	sprintf(m_lpMsgBuf, "%s", p);
}
