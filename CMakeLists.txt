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

option(DYNAMIC_BUILD "Replace inplace kernel space's DLL Loader" OFF)

### SANITIY CHECK AFTER BOOT (AMI)

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

### WIN32 Platform Specific (preprocessor)

add_definitions(-D_WIN32)
add_definitions(-D_WIN32_WINNT=0x0601)
include_directories(${LLVM_INCLUDE_PATH})

### INBED EXECUTOR (cc1)

find_library(LLVM_C_LIBRARY LLVM-C REQUIRED
	PATHS ${LLVM_LIB_PATH}
	)
add_executable(hello_world hello_world.c)
target_link_libraries(hello_world ${LLVM_C_LIBRARY})

### POST STAGE (Linkage)
# .dummy
