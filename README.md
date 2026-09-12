# native_projects —— 原生语言项目 monorepo

收录 C / C++ / Lua 项目的**混合语言子仓**，一个子目录 = 一个项目，具体语言由各项目自定。

## 为什么叫 native

- **native（原生代码）** 是业界标准叫法：编译为机器码、直接运行在操作系统上的语言生态，C / C++ 是典型代表。
- Lua 虽是脚本语言，但其解释器为 C 实现，嵌入场景（游戏脚本、Redis、Neovim 等）均以 C/C++ 为宿主，属原生生态圈内。
- 总仓按语言划分子仓（go / python / rust / typescript），C 与 C++ 分开建仓成本过高；`ccpp` 之类命名在收录 Lua 后会名不副实，`native` 统称整个原生生态圈最为贴切。

## 工具链（Windows）

- 构建主推 **CMake + CMakePresets.json + Ninja**，编译器默认 **MSVC**（VS Build Tools 2022），MSYS2 MinGW64 (gcc/g++) 备选；轻量项目可用 **xmake**。
- clang-format / clangd 按需安装；包管理（vcpkg / conan）暂不引入，需要时再定。

详细约定见父仓库 `AGENTS.md`。
