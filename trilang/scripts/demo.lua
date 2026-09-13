-- Lua 脚本层：由 C++ 宿主加载执行

-- 调用 C 库 mathutil 注册进来的 tri.fib / tri.gcd
print("[Lua] tri.fib(10) =", tri.fib(10))
print("[Lua] tri.gcd(1071, 462) =", tri.gcd(1071, 462))

-- 定义供 C++ 宿主回调的函数：返回两数之和
function tri_report(a, b)
    print(("[Lua] 被 C++ 调用: tri_report(%d, %d)"):format(a, b))
    return a + b
end
