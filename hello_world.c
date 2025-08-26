#include <llvm-c/Core.h>
#include <llvm-c/Target.h>

#include <windows.h>
#include <stdio.h>

#ifdef DYNAMIC_BUILD
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
    hDLL = LoadLibraryExW(kSystemDLLs[i], NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hDLL == NULL) {
      return ERROR_DELAY_LOAD_FAILED;
    }
  }

  for (DWORD i = 0; i < sizeof(kRuntimeDLLs) / sizeof(kRuntimeDLLs[0]); ++ i) {
    hDLL = LoadLibraryExW(kRuntimeDLLs[i], NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
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
static inline HINSTANCE pLLVMInitializeAllTargets() {
  HINSTANCE hDLL;

#define LLVM_TARGET(TargetName) \
  typedef typeof(&LLVMInitialize##TargetName##Target) PFNLLVMInitialize##TargetName##Target; \
  PFNLLVMInitialize##TargetName##Target pLLVMInitialize##TargetName##Target = NULL;
#include "llvm/Config/Targets.def"
#undef LLVM_TARGET

  hDLL = LoadLibraryExW(L"LLVM-C.dll", NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);

  if (hDLL == NULL) {
    goto done;
  }

#define LLVM_TARGET(TargetName) \
  pLLVMInitialize##TargetName##Target = (PFNLLVMInitialize##TargetName##Target)GetProcAddress(hDLL, "LLVMInitialize" #TargetName "Target"); \
  if (pLLVMInitialize##TargetName##Target != NULL) { \
    pLLVMInitialize##TargetName##Target(); \
    fprintf(stdout, #TargetName " Target Initialized\n"); \
  }
#include "llvm/Config/Targets.def"
#undef LLVM_TARGET

  fflush(stdout);

done:
  return hDLL;
}

#else

static inline HRESULT PreloadLibraries() {
  return ERROR_SUCCESS;
}

static inline HINSTANCE pLLVMInitializeAllTargets() {
#define LLVM_TARGET(TargetName) \
  LLVMInitialize##TargetName##Target(); \
  fprintf(stdout, #TargetName " Target Initialized\n");
#include "llvm/Config/Targets.def"
#undef LLVM_TARGET
  fflush(stdout);
  return GetModuleHandleW(L"LLVM-C");
}

#endif // DYNAMIC_BUILD

int main() {
  HRESULT hr = ERROR_SUCCESS;
  HINSTANCE hDLL;

  hr = PreloadLibraries();
  if (hr != ERROR_SUCCESS) {
    goto failure_handler;
  }

  hDLL = pLLVMInitializeAllTargets();
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
