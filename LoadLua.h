#ifndef _LOAD_LUA_H
#define _LOAD_LUA_H

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class CLoadLua
{
public:
	CLoadLua(const char* file);
	virtual ~CLoadLua();

	bool init();
	bool call(const char* pFunc, void *args, void* rst);
private:
	char		m_sFileName[128];
public:
	lua_State	*m_pL;
};

#endif
