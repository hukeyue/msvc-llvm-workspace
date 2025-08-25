# LLVM MSVC Workspace

This is a simple example project demonstrating how to use LLVM in an MSVC environment. The project includes a C program that shows how to preload necessary dynamic link libraries (DLLs) and initialize all LLVM targets.

## Function Overview

- The **PreloadLibraries** function is used to load system DLLs and runtime DLLs.
- The **pLLVMInitializeAllTargets** function is used to load LLVM-C.dll and initialize all LLVM targets.
- The **main** function demonstrates how to call the above two functions and handle potential loading failures.

## Compilation and Execution

Ensure that you have installed LLVM and the necessary MSVC toolchain. Then you can compile this program using a compiler that supports MSVC.

### Compiling with MSVC

```sh
cl /EHsc /I <LLVM_INCLUDE_PATH> /link <LLVM_LIB_PATH>\LLVM-C.lib hello_world.c
```

Replace `<LLVM_INCLUDE_PATH>` and `<LLVM_LIB_PATH>` with your LLVM installation paths.

### Running the Program

After compilation, run the generated executable:

```sh
hello_world
```

If LLVM-C.dll and other required DLLs are loaded successfully, the program will output the name of each initialized target and exit successfully. If loading fails, the program will output an error message and exit.

## Notes

- Ensure that LLVM-C.dll and other required DLLs are located in the application directory or in the system path.
- This example is applicable only to the Windows platform, as it uses Windows-specific APIs such as LoadLibraryExW and GetProcAddress.

## License

This project follows the license adopted by the LLVM Project. Please refer to the distribution terms of LLVM-C.dll for details.