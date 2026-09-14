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

// 调试用：打印当前 Lua 栈的完整快照，每格显示 正索引/负索引、类型、值预览
// 只读操作（lua_type/lua_tostring 等均不改栈），可在任意位置安全插入
// tag 用于标记调用点，如 dump_lua_stack(L, "L51 后")
static void dump_lua_stack(lua_State *L, const char *tag = nullptr) {
    int top = lua_gettop(L);
    if (tag != nullptr) {
        std::printf("---- Lua 栈快照：%s（共 %d 格）----\n", tag, top);
    } else {
        std::printf("---- Lua 栈快照（共 %d 格）----\n", top);
    }
    if (top == 0) {
        std::printf("    （空栈）\n");
        return;
    }
    for (int i = 1; i <= top; i++) {
        int t = lua_type(L, i);
        std::printf("    [%3d / %-3d] %-9s ",
                    i, i - top - 1, lua_typename(L, t));
        switch (t) {
        case LUA_TSTRING:
            std::printf("\"%s\"", lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            std::printf("%s", lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TNUMBER:
            std::printf("%g", lua_tonumber(L, i));
            break;
        case LUA_TFUNCTION:
        case LUA_TTABLE:
        case LUA_TUSERDATA:
        case LUA_TLIGHTUSERDATA:
        case LUA_TTHREAD:
            std::printf("%p", lua_topointer(L, i));
            break;
        default:
            break;  // nil 只打类型名
        }
        if (i == top) {
            std::printf("  <-- 栈顶");
        }
        std::printf("\n");
    }
}

static int lua_dump_lua_stack(lua_State *L) {
    const char *tag = luaL_optstring(L, 1, nullptr);  // 首参数可选,作快照标记
    dump_lua_stack(L, tag);
    return 0;  // 无返回值
}


int main() {
    // C++ 宿主负责 Lua 虚拟机的生命周期
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 把 C 函数注册到全局表 tri（tri.fib / tri.gcd）
    lua_newtable(L); //  压入新空表，记作 x
    lua_pushcfunction(L, lua_fib);
    dump_lua_stack(L);
    lua_setfield(L, -2, "fib"); // fib=lua_fib 塞进 x，栈还剩 x
    lua_pushcfunction(L, lua_gcd);
    lua_setfield(L, -2, "gcd"); // gcd=lua_gcd 塞进 x，栈还剩 x
    lua_pushcfunction(L, lua_dump_lua_stack);
    lua_setfield(L, -2, "dump_lua_stack");
    lua_setglobal(L, "tri"); // 弹出 x 绑定到全局 tri（_G.tri）上，栈空了
    dump_lua_stack(L);
    
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
    dump_lua_stack(L);
    if (lua_pcall(L, 2, 1, 0) != 0) {
        std::fprintf(stderr, "[C++] lua error: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    std::printf("[C++] tri_report 返回: %lld\n", (long long)lua_tointeger(L, -1));
    dump_lua_stack(L);
    lua_pop(L, 1);
    dump_lua_stack(L); 

    lua_close(L);
    return 0;
}
