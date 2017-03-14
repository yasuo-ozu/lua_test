#include <stdio.h>

#ifdef USE_LUA_REPO
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#else
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#endif

int func(lua_State *L)
{
	printf("hello, world\n");
	return 0;
}

int main(void)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_register(L, "func", &func);
	if (luaL_loadfile(L, "script.lua") || lua_pcall(L, 0, 0, 0))
	{
		lua_close(L);
		return 1;
	}

	lua_close(L);
	return 0;
}
