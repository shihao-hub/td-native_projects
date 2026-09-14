# trilang —— C / C++ / Lua 三语言协作 demo

native_projects 首个示范项目，演示三种语言在同一项目内的经典协作形态：

| 语言 | 文件 | 角色 |
|---|---|---|
| C | `src/mathutil.c/.h` | 算法库（fib / gcd），`extern "C"` 接口，被 C++ 与 Lua 双方使用 |
| C++ | `src/main.cpp` | 宿主：管理 Lua 虚拟机生命周期、注册 C 函数、执行脚本并回调 Lua 函数 |
| Lua | `scripts/demo.lua` | 脚本层：调用 C 函数、定义供 C++ 回调的函数 |

> `meta/host_env.lua`：宿主注入全局（`tri`）的类型声明，供 lua-language-server 分析用——LSP 按语言隔离、没有跨语言 LSP，宿主 API 用 EmmyLua 注解声明是业界通行做法。

## 依赖策略

- **Lua 5.1.5**：CMake `FetchContent` 拉源码随项目编译为静态库（本机 LuaForWindows 为 32 位，不可链接 x64）
- **doctest**：`FetchContent`，作单元测试

## 构建与运行

`out/` 构建目录由 `CMakePresets.json` 的 `binaryDir` 指定，`CMakeLists.txt` 只负责定义编译目标（源文件与链接关系）——源码树与构建树完全分离，删掉 `out/` 即彻底清理。

```powershell
# ① 配置：读 preset 在 out/build/clang-debug/ 生成 build.ninja、
#    compile_commands.json（clangd 用）、CMakeCache.txt，
#    并把 lua/doctest 源码下到 _deps/
cmake --preset clang-debug

# ② 构建：调 Ninja 按 build.ninja 并行编译链接，产出 trilang.exe / test_mathutil.exe
cmake --build --preset clang-debug

# ③ 测试：CTest 驱动 doctest 用例
ctest --preset clang-debug
```

preset 二选一：`clang-debug`（clang + Ninja，快） / `msvc-debug`（MSVC + VS 生成器，exe 在 `out/build/msvc-debug/Debug/`）。

运行 demo（一条命令：自动构建依赖 + 以 exe 目录为工作目录，免手动 cd）：

```powershell
cmake --build --preset clang-debug --target run
```

也可以直接执行 `.\out\build\clang-debug\trilang.exe`（脚本已随构建同步到 exe 旁）。
