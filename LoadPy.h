#ifndef CLOAD_PY_H
#define CLOAD_PY_H

#include "Python.h"

class CLoadPy
{
public:
	CLoadPy(const char* pPath, const char* file);
	virtual ~CLoadPy();

	bool init();

	bool call(const char* pFuncName, void * args, void* rst);

private:
	char	 m_sPath[256];
	char	 m_sFileName[100];
	PyObject *m_pName;
	PyObject *m_pModule;
	PyObject *m_pDict;
	PyObject *m_pFunc;
	PyObject *m_pArgs;
	PyObject *m_pRet;
	bool     m_bInit;
};

#endif