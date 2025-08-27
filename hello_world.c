#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/Object.h>

#include <windows.h>
#include <stdio.h>

static inline void HookInCompileTime() {
#ifndef DYNAMIC_BUILD
  /* Operations on integer types */
  (void)LLVMVoidType();
  (void)LLVMInt1Type();
  (void)LLVMInt8Type();
  (void)LLVMInt16Type();
  (void)LLVMInt32Type();
  (void)LLVMInt64Type();
  // (void)LLVMCreateIntegerType()
  /* AVAILABLE IN FUTURE: (void)LLVMInt128Type(); */

  /* Operations on real types */
  /* AVAILABLE IN FUTURE: (void)LLVMHalfType(); */
  /* AVAILABLE IN FUTURE: (void)LLVMBFloatType(); */
  (void)LLVMFloatType();
  (void)LLVMDoubleType();
  (void)LLVMX86FP80Type();
  (void)LLVMFP128Type();
  (void)LLVMPPCFP128Type();

  /* Operations on function types */
  // (void)LLVMCreateFunctionType()
  // (void)LLVMGetFunctionReturnType()

  /* Operations on struct types */
  // (void)LLVMCreateStructType()

  /* Operations on array, pointer, and vector types (sequence types) */
  // (void)LLVMCreateArrayType()
  // (void)LLVMCreatePointerType()
  // (void)LLVMCreateVectorType()
  // (void)LLVMGetElementType()

  /* Operations on other types */
  (void)LLVMVoidType();
  (void)LLVMLabelType();
  // (void)LLVMCreateOpaqueType();
#endif
}

static inline HRESULT PreloadLibraries() {
  static const wchar_t* kSystemDLLs[] = {
    L"ADVAPI32.dll",
  };
  static const wchar_t* kRuntimeDLLs[] = {
    L"MSVCP140.dll",
    L"VCRUNTIME140.dll",
    L"VCRUNTIME140_1.dll",
  };
  HINSTANCE hDLL;

  for (DWORD i = 0; i < sizeof(kSystemDLLs) / sizeof(kSystemDLLs[0]); ++ i) {
#ifdef DYNAMIC_BUILD
    hDLL = LoadLibraryExW(kSystemDLLs[i], NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
#else
    hDLL = GetModuleHandleW(kSystemDLLs[i]);
#endif
    if (hDLL == NULL) {
      return ERROR_DELAY_LOAD_FAILED;
    }
  }

  for (DWORD i = 0; i < sizeof(kRuntimeDLLs) / sizeof(kRuntimeDLLs[0]); ++ i) {
#ifdef DYNAMIC_BUILD
    hDLL = LoadLibraryExW(kRuntimeDLLs[i], NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
#else
    hDLL = GetModuleHandleW(kRuntimeDLLs[i]);
#endif
    if (hDLL == NULL) {
      hDLL = LoadLibraryExW(kRuntimeDLLs[i], NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
      if (hDLL == NULL) {
        return ERROR_DELAY_LOAD_FAILED;
      }
    }
  }
  return ERROR_SUCCESS;
}

/** LLVMInitializeAllTargets - The main program should call this function if it
    wants to link in all available targets that LLVM is configured to
    support. */
static inline HINSTANCE PreLLVMInitializeAllTargets() {
#define LLVM_TARGET(TargetName) \
  typedef __typeof__(&LLVMInitialize##TargetName##Target) PFNLLVMInitialize##TargetName##Target; \
  PFNLLVMInitialize##TargetName##Target PreLLVMInitialize##TargetName##Target = NULL;
#include "llvm/Config/Targets.def"
#undef LLVM_TARGET

  HINSTANCE hDLL;

#ifdef DYNAMIC_BUILD
  hDLL = LoadLibraryExW(L"LLVM-C.dll", NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
#else
  hDLL = GetModuleHandleW(L"LLVM-C");
#endif

  if (hDLL == NULL) {
    goto done;
  }

#define LLVM_TARGET(TargetName) \
  PreLLVMInitialize##TargetName##Target = (PFNLLVMInitialize##TargetName##Target)GetProcAddress(hDLL, "LLVMInitialize" #TargetName "Target"); \
  if (PreLLVMInitialize##TargetName##Target != NULL) { \
    PreLLVMInitialize##TargetName##Target(); \
    fprintf(stdout, "Target " #TargetName " was initialized\n"); \
  } else { \
    fprintf(stdout, "Target " #TargetName " was requested but in vacuum\n"); \
  }
#include "llvm/Config/Targets.def"
#undef LLVM_TARGET

done:
  fflush(stdout);
  return hDLL;
}

int main() {
  HRESULT hr = ERROR_SUCCESS;
  HINSTANCE hDLL;

  /* hook on static build, available since year 2007,
   * aka llvm 2.2.0 (76a0374b252c2) */
  HookInCompileTime();

  hr = PreloadLibraries();
  if (hr != ERROR_SUCCESS) {
    goto failure_handler;
  }

  hDLL = PreLLVMInitializeAllTargets();
  if (hDLL == NULL) {
    hr = ERROR_DELAY_LOAD_FAILED;
    goto failure_handler;
  }

  return 0;

failure_handler:
  fprintf(stderr, "FATAL: Load Library Failure\n");
  fflush(stderr);

  return -1;
}
