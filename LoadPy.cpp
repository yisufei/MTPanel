#include "LoadPy.h"
#include <string>
#include <iostream>

CLoadPy::CLoadPy(const char* pPath, const char* file)
{
	sprintf(m_sPath, "%s", pPath);
	sprintf(m_sFileName, "%s", file);
	m_pName = m_pModule = m_pDict = m_pFunc = m_pArgs = m_pRet = NULL;
	m_bInit = false;
}

bool CLoadPy::init()
{
	Py_Initialize();
	if (!Py_IsInitialized()) return false;

	std::string sAddPath = "sys.path.append('";
	sAddPath.append(m_sPath).append("')");

	PyRun_SimpleString("import sys");
	PyRun_SimpleString(sAddPath.c_str());
	m_pName = PyString_FromString(m_sFileName);

	//check

	m_bInit = true;
	return true;
}

bool CLoadPy::call(const char* pFuncName, void * args, void* rst)
{
	if (pFuncName == NULL || args == NULL || rst == NULL) return false;

	m_pModule = PyImport_Import(m_pName);
	if (!m_pModule) return false;

	m_pDict = PyModule_GetDict(m_pModule);
	if (!m_pDict) return false;

	m_pFunc = PyDict_GetItemString(m_pDict, pFuncName);
	if (!m_pFunc || !PyCallable_Check(m_pFunc))
	{
		return false;
	}

	PyObject *pArgs = PyTuple_New(2);

	PyTuple_SetItem(pArgs, 0, Py_BuildValue("l", 3));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("l", 4));

	PyObject * pRet = PyObject_CallObject(m_pFunc, pArgs);

	//pRet->ob_type.tp_name;
	if (pRet != NULL)
	{
		const char * p = PyString_AS_STRING(pRet);
		int len = strlen(p);
		strncpy((char*)rst, p, len);
		Py_DECREF(pArgs);
		Py_DECREF(pRet);
		return true;
	}
	else
	{
		Py_DECREF(pArgs);
		return false;
	}
}

CLoadPy::~CLoadPy()
{
	if (m_pName != NULL)	Py_DECREF(m_pName);
	if (m_pModule != NULL) Py_DECREF(m_pModule);
	if (m_pDict != NULL) Py_DECREF(m_pDict);
	if (m_pFunc != NULL) Py_DECREF(m_pFunc);
	if (m_pRet != NULL) Py_DECREF(m_pRet);

	if (Py_IsInitialized()) Py_Finalize();
}
