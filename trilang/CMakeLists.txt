cmake_minimum_required(VERSION 3.21)
project(trilang C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源码为 UTF-8 无 BOM，MSVC 默认按本地编码（GBK）误读中文注释，必须显式声明
if(MSVC)
    add_compile_options(/utf-8)
endif()

# ---- Lua 5.1.5：FetchContent 拉官方源码包，随项目编译为静态库 ----
# 本机的 LuaForWindows 是 32 位老安装，不能给 x64 链接，故直接源码编译；
# lua/lua 官方镜像缺 v5.1.5 tag（5.1 系列只到 v5.1.1），故走 lua.org tarball
include(FetchContent)
FetchContent_Declare(
    lua
    URL https://www.lua.org/ftp/lua-5.1.5.tar.gz
    URL_HASH SHA256=2640fc56a795f29d28ef15e13c34a47e223960b0240e8cb0a82d9b0738695333
)
FetchContent_MakeAvailable(lua)
# CMake 解压时自动剥掉 tarball 顶层目录，.c/.h 都在源码包的 src/ 子目录
set(LUA_SRC_DIR ${lua_SOURCE_DIR}/src)

# Lua 官方包无 CMake 支持，手工列出源文件编译
add_library(lua51 STATIC
    ${LUA_SRC_DIR}/lapi.c
    ${LUA_SRC_DIR}/lcode.c
    ${LUA_SRC_DIR}/ldebug.c
    ${LUA_SRC_DIR}/ldo.c
    ${LUA_SRC_DIR}/ldump.c
    ${LUA_SRC_DIR}/lfunc.c
    ${LUA_SRC_DIR}/lgc.c
    ${LUA_SRC_DIR}/llex.c
    ${LUA_SRC_DIR}/lmem.c
    ${LUA_SRC_DIR}/lobject.c
    ${LUA_SRC_DIR}/lopcodes.c
    ${LUA_SRC_DIR}/lparser.c
    ${LUA_SRC_DIR}/lstate.c
    ${LUA_SRC_DIR}/lstring.c
    ${LUA_SRC_DIR}/ltable.c
    ${LUA_SRC_DIR}/ltm.c
    ${LUA_SRC_DIR}/lundump.c
    ${LUA_SRC_DIR}/lvm.c
    ${LUA_SRC_DIR}/lzio.c
    ${LUA_SRC_DIR}/lauxlib.c
    ${LUA_SRC_DIR}/lbaselib.c
    ${LUA_SRC_DIR}/ldblib.c
    ${LUA_SRC_DIR}/liolib.c
    ${LUA_SRC_DIR}/lmathlib.c
    ${LUA_SRC_DIR}/loslib.c
    ${LUA_SRC_DIR}/ltablib.c
    ${LUA_SRC_DIR}/lstrlib.c
    ${LUA_SRC_DIR}/loadlib.c
    ${LUA_SRC_DIR}/linit.c
)
target_include_directories(lua51 PUBLIC ${LUA_SRC_DIR})
# Lua 5.1 源码用了 sprintf/fopen 等老 CRT 接口，屏蔽安全警告
if(MSVC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_definitions(lua51 PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# ---- C 算法库：mathutil ----
add_library(mathutil STATIC src/mathutil.c)
target_include_directories(mathutil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ---- C++ 宿主：trilang ----
add_executable(trilang src/main.cpp)
target_link_libraries(trilang PRIVATE lua51 mathutil)
# 每次构建后把 scripts/ 同步到 exe 所在目录（VS 生成器 exe 在 Debug/ 子目录），
# 运行时按相对路径 scripts/demo.lua 加载
add_custom_command(TARGET trilang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts $<TARGET_FILE_DIR:trilang>/scripts
)

# ---- 测试：doctest + CTest ----
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.12
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(doctest)
# doctest_discover_tests 命令由 doctest 自带的 cmake 模块提供
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

enable_testing()
add_executable(test_mathutil tests/test_mathutil.cpp)
target_link_libraries(test_mathutil PRIVATE mathutil doctest::doctest)
doctest_discover_tests(test_mathutil)
