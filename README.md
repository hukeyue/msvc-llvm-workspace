

# LLVM MSVC 工作区

这是一个用于演示如何在 MSVC 环境下使用 LLVM 的简单示例项目。项目包含一个 C 程序，展示了如何预加载必要的动态链接库（DLL）并初始化 LLVM 的所有目标。

## 功能概述

- **PreloadLibraries** 函数用来加载系统 DLL 和运行时 DLL。
- **pLLVMInitializeAllTargets** 函数用来加载 LLVM-C.dll 并初始化所有 LLVM 目标。
- **main** 函数则演示了如何调用上述两个函数并处理可能发生的加载失败情况。

## 编译和运行

确保你已经安装了 LLVM 以及必要的 MSVC 工具链。然后你可以使用支持 MSVC 的编译器来编译这个程序。

### 使用 MSVC 编译

```sh
cl /EHsc /I <LLVM_INCLUDE_PATH> /link <LLVM_LIB_PATH>\LLVM-C.lib hello_world.c
```

替换 `<LLVM_INCLUDE_PATH>` 和 `<LLVM_LIB_PATH>` 为你的 LLVM 安装路径。

### 运行程序

编译完成后，运行生成的可执行文件：

```sh
hello_world
```

如果 LLVM-C.dll 和其它依赖的 DLL 能够被正确加载，程序将输出每个初始化的目标名称并成功退出。如果加载失败，程序将输出错误信息并退出。

## 注意事项

- 确保 LLVM-C.dll 和其它必要的 DLL 在应用程序目录或系统路径中。
- 这个示例只适用于 Windows 平台，因为它使用了 Windows 特定的 API 如 LoadLibraryExW 和 GetProcAddress。

## 许可证

本项目遵循 LLVM 项目所采用的许可证。详情请查看 LLVM-C.dll 的分发条款。