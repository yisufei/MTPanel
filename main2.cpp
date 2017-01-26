#include <stdio.h>
#include "ConfigWithLua.h"
#include "ConfigBusiness.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

bool getValue(lua_State *L, const char* key, int *value) {
    lua_getglobal(L, key);

    if (!lua_isnumber(L, -1)){
	printf("format error\n");
        return false;
    }

    *value = (int)lua_tonumber(L, -1);
    return true;
}

bool getValue(lua_State *L, const char* lpszTableName, const char* lpszTableItem, int *value){
    lua_getglobal(L, lpszTableName);
    lua_pushstring(L, lpszTableItem);

    lua_gettable(L, -2);
    
    if (!lua_isnumber(L, -1)){
        return false;
    }

    *value = (int)lua_tonumber(L, -1);

    return 0;
}

int main(int argc, char * argv[]){
    const char *filename = argc == 2? argv[1]: "./conf/basic.lua";

    //lua_State *L = lua_open();
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)){
        luaL_error(L, "loadfile error! %s\n", lua_tostring(L, -1));
        return -1;
    }

    int value;
	/*getValue(L, "width", &value);
    printf("width is %d\n", value);

    getValue(L, "luat_Test1", "a", &value);
    printf("luat_Test1[a]: %d\n", value);*/

    ConfigWithLua *cfgLua = new ConfigWithLua();
    cfgLua->init(filename);

	cfgLua->getValue("width", &value);
    printf("width is %d\n", value);

    cfgLua->getValue("luat_Test1", "a", &value);
    printf("luat_Test1[a]: %d\n", value);
    
    char ip[128];
    cfgLua->getValue("server", "ip", ip, sizeof(ip));
    printf("server[ip]:%s\n", ip);

    ConfigBusiness *cfgBusi = new ConfigBusiness();
    printf("setFuncid:%d\n", cfgBusi->setFuncid("QRY0001"));

    char filename2[128];
    cfgBusi->getFileName(filename2, sizeof(filename2));
    printf("filename:%s\n", filename);
    char method[128];
    
    cfgBusi->getMethod(method, sizeof(method));
    printf("method:%s\n", method);

    return 0;
}
