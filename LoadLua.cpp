#include "LoadLua.h"
#include <string>

lua_State * CLoadLua::m_pL = NULL;

CLoadLua::CLoadLua(const char* file)
{
	snprintf(m_sFileName, sizeof(m_sFileName), "%s", file);
}

bool CLoadLua::init()
{
    if (m_pL != NULL) return true;
	m_pL = luaL_newstate();
	if (m_pL == NULL)	{ return false; }

    luaopen_base(m_pL);
    luaL_openlibs(m_pL);
	int iRet = luaL_loadfile(m_pL, m_sFileName);
	if (iRet){ printf("luaL_loadfile erro\n"); return false; }

	iRet = lua_pcallk(m_pL, 0, 0, 0, 0, 0);
	const char *pErrorMsg = lua_tostring(m_pL, -1);
	if (iRet){ printf("error:%s\n:", pErrorMsg); return false; }

	return true;
}

bool CLoadLua::call(const char* pFunc, void *args, void* rst){
    //printf("pFunc:%s, args:%s\n", pFunc, (const char*)args);
    int iRet = lua_getglobal(m_pL, pFunc);
    /*if (iRet){
        printf("error here 11111111\n");
        sprintf((char*)rst, "error in lua_getglobal");
        //return false;
    }*/
    /*iRet = lua_isfunction(m_pL, -1);
    if (iRet){
        printf("error here 2222222222\n");
        sprintf((char*)rst, "error in lua_isfunction");
        //return false;
    }*/
    //lua_pushinteger(m_pL, 123);
    lua_pushstring(m_pL, (const char*)args);
    iRet = lua_pcall(m_pL, 1, 1, 0);
    if (iRet){
        printf("error here 3333333\n");
        sprintf((char*)rst, lua_tostring(m_pL, -1));
        return false;
    }
    sprintf((char*)rst, lua_tostring(m_pL, -1));
    return true;
}
/*
bool CLoadLua::call(const char* pFunc, void *args, void* rst)
{
	int iRet = lua_getglobal(m_pL, pFunc); 
	if (iRet)
	{
		const char *pErrorMsg = lua_tostring(m_pL, -1);
	}
	iRet = lua_isfunction(m_pL, -1);
	int a, b;
	sscanf((char*)args, "%d%d", &a, &b);
	lua_pushnumber(m_pL, a);    
	lua_pushnumber(m_pL, b);   
	iRet = lua_pcall(m_pL, 2, 1, 0);
	if (iRet)                      
	{
		const char *pErrorMsg = lua_tostring(m_pL, -1);
		return false;
	}
	if (lua_isnumber(m_pL, -1))   
	{
		double fValue = lua_tonumber(m_pL, -1);
		sprintf((char*)rst, "%.lf", fValue);
		return true;
	}
	if (lua_isstring(m_pL, -1))
	{
		const char *sValue = lua_tostring(m_pL, -1);
		sprintf((char*)rst, "%s", sValue);
		return true;
	}
	return false;
}*/

CLoadLua::~CLoadLua()
{
	if (m_pL != m_pL)
	{
		lua_close(m_pL);
		m_pL = NULL;
	}
}
