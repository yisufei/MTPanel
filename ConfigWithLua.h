#ifndef _CONFIG_WITH_LUA_H
#define _CONFIG_WITH_LUA_H

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class ConfigWithLua{

public:
    ConfigWithLua();
    ~ConfigWithLua();

public:
    bool init(const char* psFileName);

public:
    bool getValue(const char* key, int *value);
    bool getValue(const char* psTableName,const char* psTableItem,int *value);
    bool getValue(const char* psTableName,const char* psTableItem,char *value, int size);

private:
    lua_State       *m_lpL;
};

#endif
