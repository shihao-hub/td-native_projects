// trilang —— C / C++ / Lua 三语言协作 demo
// 分工：mathutil.c(C) 提供算法 → main.cpp(C++) 做宿主管理 Lua 虚拟机 → demo.lua(Lua) 作脚本层

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <cstdio>

#include "mathutil.h"

// 注册给 Lua 的桥接函数：tri.fib(n)，内部转发到 C 实现
static int lua_fib(lua_State *L) {
    int n = (int)luaL_checkinteger(L, 1);
    lua_pushinteger(L, tri_fib(n));
    return 1;  // 返回值个数
}

// 注册给 Lua 的桥接函数：tri.gcd(a, b)
static int lua_gcd(lua_State *L) {
    int a = (int)luaL_checkinteger(L, 1);
    int b = (int)luaL_checkinteger(L, 2);
    lua_pushinteger(L, tri_gcd(a, b));
    return 1;
}

int main() {
    // C++ 宿主负责 Lua 虚拟机的生命周期
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 把 C 函数注册到全局表 tri（tri.fib / tri.gcd）
    lua_newtable(L);
    lua_pushcfunction(L, lua_fib);
    lua_setfield(L, -2, "fib");
    lua_pushcfunction(L, lua_gcd);
    lua_setfield(L, -2, "gcd");
    lua_setglobal(L, "tri");

    // 方向一：C++ 执行 Lua 脚本，脚本内调用 C 函数（Lua 5.1 无 LUA_OK，非 0 即错）
    if (luaL_dofile(L, "scripts/demo.lua") != 0) {
        std::fprintf(stderr, "[C++] lua error: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // 方向二：C++ 直接调用 Lua 定义的函数 tri_report(a, b)
    lua_getglobal(L, "tri_report");
    lua_pushinteger(L, 1071);
    lua_pushinteger(L, 462);
    if (lua_pcall(L, 2, 1, 0) != 0) {
        std::fprintf(stderr, "[C++] lua error: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    std::printf("[C++] tri_report 返回: %lld\n", (long long)lua_tointeger(L, -1));
    lua_pop(L, 1);

    lua_close(L);
    return 0;
}
