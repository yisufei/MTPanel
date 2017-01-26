#include "ConfigWithLua.h"
#include <string.h>

ConfigWithLua::ConfigWithLua(){
    m_lpL = NULL;
}

ConfigWithLua::~ConfigWithLua(){
    if (m_lpL != NULL){
       lua_close(m_lpL); 
       m_lpL = NULL;
    }
}

bool ConfigWithLua::init(const char* psFileName){
    m_lpL = luaL_newstate();
    luaL_openlibs(m_lpL);
    if (luaL_loadfile(m_lpL, psFileName) || lua_pcall(m_lpL, 0, 0, 0)){
        luaL_error(m_lpL, "loadfile error! %s\n", lua_tostring(m_lpL, -1));
        lua_close(m_lpL);
        m_lpL = NULL;
        return false;
    }
    return true;
}

bool ConfigWithLua::getValue(const char* key, int *value){
    if (m_lpL == NULL) return false;
    lua_getglobal(m_lpL, key);

    if (!lua_isnumber(m_lpL, -1)){
	printf("format error\n");
        return false;
    }

    *value = (int)lua_tonumber(m_lpL, -1);
    return true;
}

bool ConfigWithLua::getValue(const char* psTableName,const char* psTableItem,int *value){
    if (m_lpL == NULL) return false;
    lua_getglobal(m_lpL, psTableName);
    lua_pushstring(m_lpL, psTableItem);

    lua_gettable(m_lpL, -2);
    
    if (!lua_isnumber(m_lpL, -1)){
        return false;
    }
    *value = (int)lua_tonumber(m_lpL, -1);

    return true;
}

bool ConfigWithLua::getValue(const char* psTableName,const char* psTableItem,char *value, int size){
    if (m_lpL == NULL) return false;
    lua_getglobal(m_lpL, psTableName);
    lua_pushstring(m_lpL, psTableItem);

    lua_gettable(m_lpL, -2);
    if (!lua_isstring(m_lpL, -1)){
        return false;
    }
    const char * p = lua_tostring(m_lpL, -1);  
    strncpy(value, p, size);

    return true;
}
