# 使用 CMake 构建与安装 awtk-modbus

本仓库同时保留 **SCons** 与 **CMake** 构建方式。CMake 路径用于生成 **共享库** `modbus`，并安装 **`find_package(awtk-modbus)`** 所需的配置文件。

## 前置条件

1. 已按 AWTK 官方方式用 CMake **安装** AWTK（安装根目录下存在 `lib/cmake/awtk/awtkConfig.cmake`）。
2. 若 AWTK 安装中的 `awtk::awtk` 目标还依赖其它包（例如 **SDL3**），请把对应前缀一并加入 `CMAKE_PREFIX_PATH`，否则链接阶段可能失败。

## 配置与编译

安装前缀一律通过 **`CMAKE_INSTALL_PREFIX`** 指定；下面以本机示例路径说明（请按需替换）。

```bash
export AWTK_PREFIX=/Users/jim/work/awtk-root/awtk-install   # 示例：AWTK 安装根
export STAGE=/tmp/awtk-modbus-install                       # 示例：本库安装前缀

cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$STAGE" \
  -DCMAKE_PREFIX_PATH="$AWTK_PREFIX"

cmake --build build -j
cmake --install build
```

安装后的典型布局：

- 头文件：`include/awtk-modbus/*.h`
- 库文件：`lib/libmodbus.so`（Linux）、`lib/libmodbus.dylib`（macOS）、`bin/modbus.dll` + `lib/modbus.lib`（Windows，具体以生成器为准）
- CMake 包：`lib/cmake/awtk-modbus/`（`awtk-modbusConfig.cmake`、`awtk-modbusTargets*.cmake` 等）

## 下游工程集成

```cmake
find_package(awtk-modbus CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE awtk_modbus::modbus)
```

`CMAKE_PREFIX_PATH` 需同时包含 **AWTK** 与 **awtk-modbus** 的安装根（若安装在不同前缀下），例如：

```bash
cmake -B consumer-build -DCMAKE_PREFIX_PATH="/path/to/awtk-install;/path/to/awtk-modbus-install"
```

头文件在工程内使用 `#include "modbus_client.h"` 等形式即可（需已链接 `awtk_modbus::modbus`，其会带上 `awtk-modbus` 与 AWTK 的包含目录）。

## 可搬迁安装与运行时库路径

- **Linux**：已为 `libmodbus.so` 设置 **`$ORIGIN`** 的 RPATH，便于与 **同目录** 下的 `libawtk.so` 等一起打包搬迁。
- **macOS**：使用 **`@loader_path`** 作为 RPATH，与位于同一 `lib/` 目录下的 `@rpath/libawtk.dylib` 等配合使用。
- **Windows**：请把 **`bin/`**（或你安装 DLL 的目录）加入 **PATH**，或与应用程序放在同一目录。常见需一并部署的动态库包括（以你实际 AWTK 构建为准）：

  - `modbus.dll`
  - `libawtk.dll`（或 `awtk.dll`，名称因构建而异）
  - SDL3 及 AWTK 所链接的其它第三方 DLL（可用 `dumpbin /dependents modbus.dll` 查看）

**MSVC 与 MinGW** 的 ABI 与运行时库不同，请使用 **与 AWTK 相同工具链** 编译 awtk-modbus，避免混用。

## 手动触发 CI（可选）

仓库提供 `.github/workflows/cmake-smoke.yml`，仅在 **workflow_dispatch** 下运行：在 Actions 中手动触发，并填写 **`awtk_prefix`**（AWTK 的 `CMAKE_PREFIX_PATH` 单一路径）。默认运行环境为 `ubuntu-latest`；若运行器上未安装对应 AWTK，任务会失败。
