#!/usr/bin/env cmake4
cmake_minimum_required(VERSION 4.0)

if (CMAKE_CURRENT_BINARY_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	message(FATAL_ERROR "Dummy build is not support")
endif()

## ESCALASTION
## ESCAPE
## KEY INVALID -- SERIAL NUMBER CALLED -- STALL

project(workspace VERSION 0.3 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Hell)

### ESCALATION (Battery)

option(DYNAMIC_BUILD "Replace inplace kernel space's DLL Loader" ON)

### SANITIY CHECK AFTER BOOT (AMI)

if (MSVC)
	set(CMAKE_GENERATOR_CC cl)
endif()
if (WIN32 AND NOT MSVC)
	message(FATAL_ERROR "Only MSVC build is permitted")
endif()

### INITIALIZATION (PE)

if (NOT DEFINED LLVM_INCLUDE_PATH)
	message(FATAL_ERROR "Build support (LLVM_INCLUDE_PATH) for inclusion path is not given")
endif()
if (NOT DEFINED LLVM_LIB_PATH)
	message(FATAL_ERROR "Build support (LLVM_LIB_PATH) for library path is not given")
endif()

find_library(LLVM_C_LIBRARY LLVM-C REQUIRED
	PATHS ${LLVM_LIB_PATH}
	)

### INITIALIZATION (COFF)

include(CheckLibraryExists)
check_library_exists("${LLVM_C_LIBRARY}" LLVMGetErrorMessage
  "GetErrorMessage Exists" HAVE_LLVM_ERROR_MESSAGE)
if (NOT HAVE_LLVM_ERROR_MESSAGE)
  message(FATAL_ERROR "INVALID: Symbol LLVMGetErrorMessage not found")
endif()
check_library_exists("${LLVM_C_LIBRARY}" LLVMGetSectionName
  "Section Exists" HAVE_LLVM_SECTION_NAME)
check_library_exists("${LLVM_C_LIBRARY}" LLVMGetSections
  "Sections Exist" HAVE_LLVM_SECTIONS)
check_library_exists("${LLVM_C_LIBRARY}" LLVMCreateObjectFile
  "Object Factory Exist" HAVE_LLVM_CREATE_OBJECT_FILE)
check_library_exists("${LLVM_C_LIBRARY}" LLVMOrcCreateNewThreadSafeContext
  "JIT RCU Thread Exist" HAVE_LLVM_ORC_CREATE_NEW_THREAD_CONTAINER)
check_library_exists("${LLVM_C_LIBRARY}" LLVMEnablePrettyStackTrace
  "Pretty Stack Printer" HAVE_LLVM_ENABLE_PRETTY_STACK_TRACE)
check_library_exists("${LLVM_C_LIBRARY}" LLVMLinkModules2
  "Linkage Module" HAVE_LLVM_MODULES_TWO)
check_library_exists("${LLVM_C_LIBRARY}" LLVMCreateDisasmCPUFeatures
  "CPUID Module" HAVE_LLVM_DISASM_CPU_FX)
check_library_exists("${LLVM_C_LIBRARY}" LLVMGetComdat
  "Comdat Module" HAVE_LLVM_COMDAT_GETTER)
check_library_exists("${LLVM_C_LIBRARY}" LLVMStripModuleDebugInfo
  "GNU Strip Tool" HAVE_LLVM_HAVE_GNU_STRIP)
check_library_exists("${LLVM_C_LIBRARY}" LLVMGetTargetMachineCPU
  "Loose CPU Detecter" HAVE_LLVM_HAVE_CPU_GETTER)
check_library_exists("${LLVM_C_LIBRARY}" LLVMRecompileAndRelinkFunction
  "Heavy CPU Detecter" HAVE_LLVM_RECOMPILE_AND_RELINK_FX)
check_library_exists("${LLVM_C_LIBRARY}" LLVMInitializeX86Target
  "Compile-Ahead-Time CPU Detecter" HAVE_LLVM_X86_INITIALIZER)
if (NOT HAVE_LLVM_X86_INITIALIZER)
  message(FATAL_ERROR "At Least X86 Target should be included")
endif()
check_library_exists("${LLVM_C_LIBRARY}" LLVMGetDINodeTag
  "Dwarf CPU Detecter" HAVE_LLVM_68K_INITIALIZER)

### WIN32 Platform Specific (preprocessor)

add_definitions(-D_WIN32)
add_definitions(-D_WIN32_WINNT=0x0601)
include_directories(${LLVM_INCLUDE_PATH})
link_directories(${LLVM_LIB_PATH})

check_library_exists(shell32 CloseHandle "" HAVE_WIN32_CLOSE_HANDLE)
check_library_exists(shell32 ReOpenFile "" HAVE_WIN32_RE_OPEN_FILE)
check_library_exists(shell32 ReadFile "" HAVE_WIN32_READ_FILE)
check_library_exists(shell32 WriteFile "" HAVE_WIN32_WRITE_FILE)
check_library_exists(shell32 CreateFileW "" HAVE_WIN32_CREATE_FILE_W)
check_library_exists(shell32 DeleteFileW "" HAVE_WIN32_DELETE_FILE_W)
check_library_exists(shell32 ReplaceFileW "" HAVE_WIN32_REPLACE_FILE_W)
check_library_exists(shell32 CreateDirectoryW "" HAVE_WIN32_CREATE_DIR_W)
check_library_exists(shell32 RemoveDirectoryW "" HAVE_WIN32_REMOVE_DIR_W)

check_library_exists(shell32 CreateFileA "" HAVE_WIN32_CREATE_FILE_ANSI)
check_library_exists(shell32 DeleteFileA "" HAVE_WIN32_DELETE_FILE_ANSI)
check_library_exists(shell32 ReplaceFileA "" HAVE_WIN32_REPLACE_FILE_ANSI)
check_library_exists(shell32 CreateDirectoryA "" HAVE_WIN32_CREATE_DIR_ANSI)
check_library_exists(shell32 RemoveDirectoryA "" HAVE_WIN32_REMOVE_DIR_ANSI)
check_library_exists(ntdll PoCallDriver "" HAVE_PO_CALL_DRIVER)
check_library_exists(ntdll NtQueryObject "" HAVE_NTDLL_QUERY_OBJECT)
check_library_exists(ntdll NtQueryVirtualMemory "" HAVE_NTDLL_QUERY_VIRTUAL_MEMORY)
check_library_exists(ntdll NtFreeVirtualMemory "" HAVE_NTDLL_FREE_VIRTUAL_MEMORY)
check_library_exists(ntdll NtDeviceIoControlFile "" HAVE_NTDLL_DEVICE_IO_CONTROL_FILE)
check_library_exists(ntdll NtCreateSection "" HAVE_NTDLL_CREATE_SECTION)
check_library_exists(ntdll NtCreateFile "" HAVE_NTDLL_CREATE_FILE)
check_library_exists(ntdll NtOpenFile "" HAVE_NTDLL_OPEN_FILE)
check_library_exists(ntdll NtReadFile "" HAVE_NTDLL_READ_FILE)
check_library_exists(ntdll NtWriteFile  "" HAVE_NTDLL_WRITE_FILE)
check_library_exists(ntdll NtClose "" HAVE_NTDLL_CLOSE)
check_library_exists(ntdll NtLockFile "" HAVE_NTDLL_LOCK_FILE)
check_library_exists(ntdll NtUnlockFile "" HAVE_NTDLL_UNLOCK_FILE)

check_library_exists(kernel32 MoveFileW "" HAVE_MOVE_FILE_W)
check_library_exists(kernel32 OpenFile "" HAVE_OPEN_FILE)
check_library_exists(kernel32 FreeLibrary "" HAVE_FREE_LIBRARY)
check_library_exists(kernel32 LoadLibraryExW "" HAVE_LOAD_LIBRARY_EX_W)
check_library_exists(kernel32 GetProcAddress "" HAVE_GET_PROC_ADDRESS)
check_library_exists(kernel32 GetModuleHandleW "" HAVE_GET_MODULE_HANDLE_W)
check_library_exists(kernel32 SetCurrentDirectoryW "" HAVE_SET_CURRENT_DIRECTORY_W)
if (DYNAMIC_BUILD AND NOT HAVE_LOAD_LIBRARY_EX_W)
  message(FATAL_ERROR "Dynamic build is required but loader does not exist")
endif()
check_library_exists(kernel32 MoveFileA "" HAVE_MOVE_FILE_ANSI)
check_library_exists(kernel32 LoadLibraryExA "" HAVE_LOAD_LIBRARY_EX_ANSI)
check_library_exists(kernel32 GetModuleHandleA "" HAVE_GET_MODULE_HANDLE_ANSI)
check_library_exists(kernel32 QueryDosDeviceA "" HAVE_DOS_QUERY__DEVICE_ANSI)
check_library_exists(kernel32 DefineDosDeviceA "" HAVE_DOS_DEFINE__DEVICE_ANSI)
check_library_exists(kernel32 SetCurrentDirectoryA "" HAVE_SET_CURRENT_DIRECTORY_ANSI)
if (HAVE_LLVM_COMDAT_GETTER AND NOTHAVE_DOS_QUERY__DEVICE_A)
  message(FATAL_ERROR "Comdat module is required but loader does not exist")
endif()

### *NIX Routine (Pass Manager)
include(CheckIncludeFile)
include(CheckCSourceCompiles)
include(CheckSymbolExists)
set(CMAKE_REQUIRED_INCLUDES "${CMAKE_REQUIRED_INCLUDES} ${LLVM_INCLUDE_PATH}")
set(CMAKE_REQUIRED_LINK_DIRECTORIES "${CMAKE_REQUIRED_LINK_DIRECTORIES} ${LLVM_LIB_PATH}")
set(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES} ${LLVM_C_LIBRARY}")
check_c_source_compiles ("
#include <windows.h>
extern void LLVMSetVolatile(void*, BOOL);
extern void LLVMSetWeak(void*, BOOL);
extern void LLVMSetOrdering(void*, BOOL);
int main() {
  LLVMSetVolatile(NULL, TRUE);
  LLVMSetWeak(NULL, TRUE);
  LLVMSetOrdering(NULL, TRUE);
  return 0;
}
" HAVE_LLVM_SET_VOLATILE_WEAK_ORDERING)
check_c_source_compiles ("
#include <stdlib.h>
extern void* LLVMSetGC(void*, const char*);
int main() {
  LLVMSetGC(NULL, \"mark-and-sweep\");
  return 0;
}
" HAVE_LLVM_SET_GARBAGE_COLLECTION)
check_c_source_compiles ("
#include <windows.h>
#include <stdlib.h>
extern void* LLVMSetTailCall(void*, BOOL);
int main() {
  LLVMSetTailCall(NULL, TRUE);
  return 0;
}
" HAVE_LLVM_SET_TAIL_CALL)
check_c_source_compiles ("
#include <stdlib.h>
extern void* LLVMSetAlignment(void*, unsigned);
int main() {
  LLVMSetAlignment(NULL, 4);
  return 0;
}
" HAVE_LLVM_SET_ALIGNMENT)
check_c_source_compiles ("
#include <stdlib.h>
extern void* LLVMGetSectionAddress(void*);
int main() {
  LLVMGetSectionAddress(NULL);
  return 0;
}
" HAVE_LLVM_HAVE_GET_SECTION_ADDRESS)
check_c_source_compiles ("
#include <windows.h>
#include <llvm-c/Core.h>
int main() {
  LARGE_INTEGER ioz;
  int iqz = LLVMAtomicOrderingNotAtomic;
  int isz = LLVMAtomicOrderingUnordered;
  ioz.QuadPart = iqz + isz;
  return ioz.LowPart;
}
" HAVE_LLVM_ATOMIC_LARGE_INTEGER_WINNT)
check_symbol_exists("LLVMSetIsDisjoint" "llvm-c/Core.h" HAVE_LLVM_IS_DISJOINT)
check_symbol_exists("LLVMSetCleanup" "llvm-c/Core.h" HAVE_LLVM_CLEANUP)
check_symbol_exists("LLVMSetCondition" "llvm-c/Core.h" HAVE_LLVM_CONDITION)
check_symbol_exists("LLVMSetSuccessor" "llvm-c/Core.h" HAVE_LLVM_SUCCESSOR)
check_symbol_exists("LLVMGetPoison" "llvm-c/Core.h" HAVE_LLVM_POISON)
check_symbol_exists("LLVMSetExact" "llvm-c/Core.h" HAVE_LLVM_EXACT)
check_symbol_exists("LLVMSetNUW" "llvm-c/Core.h" HAVE_LLVM_NUW)
check_symbol_exists("LLVMSetNSW" "llvm-c/Core.h" HAVE_LLVM_NSW)
check_symbol_exists("LLVMSetMetadata" "llvm-c/Core.h" HAVE_LLVM_METADATA)

### INBED EXECUTOR (cc1)
if (DYNAMIC_BUILD)
  add_definitions(-DDYNAMIC_BUILD=1)
endif()
add_executable(hello_world hello_world.c)
target_link_libraries(hello_world ${LLVM_C_LIBRARY})

### POST STAGE (Linkage)
# stale
