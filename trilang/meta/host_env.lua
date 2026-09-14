-- host_env.lua —— 宿主注入全局的类型声明
-- 仅供 lua-language-server 分析用（EmmyLua 注解），运行时不 require 本文件。
-- tri 表由 C++ 宿主注册（src/main.cpp：lua_setglobal(L, "tri")），
-- 函数实现在 C 库 src/mathutil.c，经 main.cpp 桥接暴露给 Lua。

-- 先声明全局（宿主注入），让分析器认识 tri
tri = {}

---迭代法斐波那契：tri.fib(n)
---@param n integer 序号（0 起）
---@return integer fib(n)
function tri.fib(n) end

---辗转相除最大公约数：tri.gcd(a, b)
---@param a integer
---@param b integer
---@return integer gcd(a, b)
function tri.gcd(a, b) end
