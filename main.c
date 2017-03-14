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
	return 1;
}

int func_in_table(lua_State *L) {
	printf("func in table!!\n");
	return 1;
}

int func_return_table(lua_State *L) {
	lua_newtable(L);
	lua_pushnumber(L, 3.14159);
	lua_setfield(L, -2, "pi");
	lua_pushstring(L, "hello from c");
	lua_setfield(L, -2, "greeting");
	lua_pushcfunction(L, &func_in_table);
	lua_setfield(L, -2, "f");
	return 1;
}

static char *default_script =
"_G['io'] = nil\n"
"_G['file'] = nil\n"
"_G['rt'] = nil\n"
"_G['hwd'] = nil\n"
"_G['tcp'] = nil\n"
"_G['udp'] = nil\n"
"_G['os'] = nil\n"
"_G['package'] = nil\n"
"collectgarbage('collect')\n";

int main(void)
{
	int status;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	// register functions to global
	lua_register(L, "func", &func);
	lua_register(L, "func_return_table", &func_return_table);

	// run built-in script
	int ret = luaL_dostring(L, default_script);
	if (ret != 0) {
		fprintf(stderr, "Default script failed\n");
		return 1;
	}

	if ((status = luaL_loadfile(L, "script.lua")) || (status = lua_pcall(L, 0, 0, 0)))
	{
		fprintf(stderr, "Script error (status: %d)\n", status);
		const char *msg = lua_tostring(L, 1);
		luaL_traceback(L, L, msg, 1);
		fprintf(stderr, "%s\n", lua_tostring(L, -1));
		return 1;
	}

	if (!luaL_loadfile(L, "script.lua")) {
		printf("script.lua is %s value\n", luaL_typename(L, 1));
		if (lua_isfunction(L, -1)) printf("yeah!\n");
		lua_pop(L, 1);
	}

	// run lua function
	lua_getglobal(L, "print");
	lua_pushstring(L, "lua's print called from c");
	lua_pcall(L, 1, 0, 0);

	// run user function
	lua_getglobal(L, "luafunc");
	lua_pushinteger(L, 3);
	lua_pushinteger(L, 5);
	lua_pcall(L, 2, 1, 0); // 2 args, 1 returns
	if (lua_isnumber(L, -1)) {
		printf("luafunc(3, 5) == %d\n", lua_tointeger(L, -1));
	} else {
		printf("luafunc(3, 5) == (string) %s\n", lua_tostring(L, -1));
	}

	lua_close(L);
	return 0;
}
