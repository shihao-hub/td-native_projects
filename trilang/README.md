# trilang —— C / C++ / Lua 三语言协作 demo

native_projects 首个示范项目，演示三种语言在同一项目内的经典协作形态：

| 语言 | 文件 | 角色 |
|---|---|---|
| C | `src/mathutil.c/.h` | 算法库（fib / gcd），`extern "C"` 接口，被 C++ 与 Lua 双方使用 |
| C++ | `src/main.cpp` | 宿主：管理 Lua 虚拟机生命周期、注册 C 函数、执行脚本并回调 Lua 函数 |
| Lua | `scripts/demo.lua` | 脚本层：调用 C 函数、定义供 C++ 回调的函数 |

## 依赖策略

- **Lua 5.1.5**：CMake `FetchContent` 拉源码随项目编译为静态库（本机 LuaForWindows 为 32 位，不可链接 x64）
- **doctest**：`FetchContent`，作单元测试

## 构建与运行

```powershell
# 二选一：clang + Ninja（快） / MSVC + VS 生成器
cmake --preset clang-debug
cmake --build --preset clang-debug

# 运行 demo（在构建目录内，脚本随构建同步）
cd out/build/clang-debug
./trilang.exe

# 跑测试
cd ../../..
ctest --preset clang-debug
```
