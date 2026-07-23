# AGENTS.md

Guidance for AI coding agents working on the libde265 codebase.

## 构建项目

### Linux / macOS

```bash
./build.sh           # Release，产物到 build/，生成 compile_commands.json 软链
./build.sh build-debug  # 自定义构建目录
```

脚本内部执行：
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build -j$(nproc)
```

### Windows（MSVC）

```bat
build.bat
```

自动通过 vswhere 找到 Visual Studio，调用 `vcvars64.bat`，在 `build/` 下用 CMake + NMake 构建共享库。关键 CMake 选项：
- `-DBUILD_SHARED_LIBS=ON`
- `-DENABLE_SDL=OFF`
- `-DENABLE_DECODER=OFF`（命令行解码器，非库本身）
- `-DENABLE_ENCODER=OFF`

### CMake Presets

```bash
cmake --preset release   # Release 构建，ENABLE_ENCODER=OFF
cmake --preset fuzzing   # libFuzzer，产物到 build-fuzzing/
cmake --preset afl       # AFL++，产物到 build-afl/
cmake --build --preset release
```

## 构建产物

- `build/libde265/libde265.dll` / `libde265.so` — 解码器共享库
- `build/dec265/dec265` — 命令行解码工具（可选）
