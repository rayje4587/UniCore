﻿#include "ULua.h"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include "UMemory.h"



namespace uni
{

static int lua_get_at(lua_State *L)
{
    int argCount = lua_gettop(L);
    if(argCount < 2)
    {
        lua_pushstring(L,"insufficient arguments");
        lua_error(L);
    }
    //至少有2个参数。
    int base = luaL_checkint(L,1);
    int *offsets = new int[argCount-1];
    for(int i = 2; i <= argCount; i++)
    {
        offsets[i-2] = luaL_checkint(L,i);
    }
    int result = 0;
    try
    {
        result = GetAt<int>(base,offsets,argCount-1);
        lua_pushinteger(L,result);
    }
    catch (...)
    {
    	lua_pushnil(L);
    }
    return 1;
}

static int lua_sleep(lua_State *L)
{
    int time = luaL_checkint(L,1);
    Sleep(time);
    return 0;
}

static const struct luaL_reg g_luaFunctions[] = {
    {"sleep",lua_sleep},
    {"get_at", lua_get_at},
    {NULL, NULL}  /* sentinel */
};

int RegisterCommonLuaFunctions( lua_State *L )
{
    for(int i = 0; g_luaFunctions[i].func; i++)
    {
        lua_pushcfunction(L,g_luaFunctions[i].func);
        lua_setglobal(L,g_luaFunctions[i].name);
    }
    return 0;
}

}//namespace uni