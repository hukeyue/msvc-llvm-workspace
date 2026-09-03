//===--- msvc-llvm-workspace/fibonacci.c - An example use of the LLVM C API -----===//
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "llvm/Config/llvm-config.h"
#include "llvm-c/Analysis.h"
#include "llvm-c/Core.h"
#include "llvm-c/Target.h"
#include "llvm-c/ExecutionEngine.h"

LLVMValueRef CreateFib(LLVMModuleRef M, LLVMContextRef Context) {
    // Create the fib function and insert it into module M. This function is said
    // to return an int and take an int parameter.
    LLVMTypeRef FibRTy[1] = { LLVMInt32TypeInContext(Context) };
    LLVMTypeRef FibFTy = LLVMFunctionType(LLVMInt64TypeInContext(Context), FibRTy, 1, false);
    LLVMValueRef FibF = LLVMAddFunction(M, "fib", FibFTy);
    LLVMSetLinkage(FibF, LLVMExternalLinkage);

    // Add a basic block to the function.
    LLVMBasicBlockRef BB = LLVMAppendBasicBlockInContext(Context, FibF, "EntryBlock");

    // Get pointers to the contants.
    LLVMValueRef One = LLVMConstInt(LLVMInt32TypeInContext(Context), 1, true);
    LLVMValueRef Two = LLVMConstInt(LLVMInt32TypeInContext(Context), 2, true);
    LLVMValueRef One64 = LLVMConstInt(LLVMInt64TypeInContext(Context), 1, true);

    // Get pointer to the integer argument of the add1 function...
    LLVMValueRef ArgX = LLVMGetParam(FibF, 0); // Get the arg.
    LLVMSetValueName2(ArgX, "AnArg", sizeof("AnArg")-1);  // Give it a nice symbolic name for fun

    // Create the true_block.
    LLVMBasicBlockRef RetBB = LLVMAppendBasicBlockInContext(Context, FibF, "return");
    // Create an exit block.
    LLVMBasicBlockRef RecurseBB = LLVMAppendBasicBlockInContext(Context, FibF, "recurse");

    // Create the "if (arg <= 2) goto exitbb"
    LLVMBuilderRef Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, BB);
    LLVMValueRef CondInst = LLVMBuildICmp(Builder, LLVMIntSLE, ArgX, Two, "cond");
    LLVMDisposeBuilder(Builder);
    Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, BB);
    LLVMBuildCondBr(Builder, CondInst, RetBB, RecurseBB);
    LLVMDisposeBuilder(Builder);

    // Create: ret int 1
    Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, RetBB);
    LLVMBuildRet(Builder, One64);
    LLVMDisposeBuilder(Builder);

    // create fib(x-1)
    Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, RecurseBB);
    LLVMValueRef Sub = LLVMBuildSub(Builder, ArgX, One, "arg");
    LLVMDisposeBuilder(Builder);

    Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, RecurseBB);
    LLVMValueRef SubArgs[1] = { Sub };
    LLVMValueRef CallFibX1 = LLVMBuildCall2(Builder, // Builder
                                              FibFTy,  // Ty
                                              FibF,    // Fn
                                              SubArgs, // Args
                                              1,       // NumArgs
                                              "fibx1"  // Name
                                              );
    LLVMSetTailCall(CallFibX1, true);
    LLVMDisposeBuilder(Builder);

    // create fib(x-2)
    Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, RecurseBB);
    Sub = LLVMBuildSub(Builder, ArgX, Two, "arg");
    LLVMDisposeBuilder(Builder);

    Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, RecurseBB);
    SubArgs[0] = Sub;
    LLVMValueRef CallFibX2 = LLVMBuildCall2(Builder, // Builder
                                              FibFTy,  // Ty
                                              FibF,    // Fn
                                              SubArgs, // Args
                                              1,       // NumArgs
                                              "fibx2"  // Name
                                              );
    LLVMSetTailCall(CallFibX2, true);
    LLVMDisposeBuilder(Builder);

    // fib(x-1) + fib(x-2)
    Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, RecurseBB);
    LLVMValueRef Sum = LLVMBuildAdd(Builder, CallFibX1, CallFibX2, "addresult");
    LLVMDisposeBuilder(Builder);

    // Create the return instruction and add it to the basic block
    Builder = LLVMCreateBuilderInContext(Context);
    LLVMPositionBuilderAtEnd(Builder, RecurseBB);
    LLVMBuildRet(Builder, Sum);
    LLVMDisposeBuilder(Builder);

    return FibF;
}

void LLVMPrintVersion(void) {
    unsigned int Major, Minor, Patch;
    LLVMGetVersion(&Major, &Minor, &Patch);
    fprintf(stderr, "LLVM (Compiled) %s (Loaded) %u.%u.%u\n", LLVM_VERSION_STRING, Major, Minor, Patch);
}

int main(int argc, const char* argv[]) {
    int32_t n = argc > 1 ? atol(argv[1]) : 24;
    LLVMContextRef Context = NULL;
    LLVMModuleRef Owner = NULL, M = NULL;
    LLVMValueRef FibF = NULL;
    LLVMExecutionEngineRef EE = NULL;
    char *errStr = NULL;
    int retval = 1;

#ifdef CROSS_COMPILE
    LLVMInitializeAllTargetInfos();
    LLVMInitializeAllTargets();
    LLVMInitializeAllTargetMCs();
    LLVMInitializeAllAsmPrinters();
    LLVMInitializeAllAsmParsers();
#else
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
#endif

    Context = LLVMContextCreate();
    if (!Context)
        goto failure;

    // Create some module to put our function into it.
    M = Owner = LLVMModuleCreateWithNameInContext("test", Context);
    if (!Owner)
        goto failure;

    // We are about to create the "fib" function:
    FibF = CreateFib(Owner, Context);
    if (!FibF)
        goto failure;

    LLVMLinkInMCJIT();

    // Now we going to create EE
    if (LLVMCreateJITCompilerForModule(&EE, Owner, 2, &errStr) != 0) {
        fprintf(stderr, "%s: Failed to construct ExecutionEngine: %s\n", argv[0], errStr);
        LLVMDisposeMessage(errStr);
        goto failure;
    }

    Owner = NULL;

    fprintf(stderr, "verifying...\n");
    if (LLVMVerifyModule(M, LLVMPrintMessageAction, &errStr) != 0) {
        fprintf(stderr, "%s: Error constructing function! %s\n", argv[0], errStr);
        LLVMDisposeMessage(errStr);
        goto failure;
    }

    fprintf(stderr, "OK\n");

    fprintf(stderr, "We just constructed this LLVM module:\n\n---------\n");

    LLVMDumpModule(M);

    fprintf(stderr, "---------\nstarting fibonacci(%d) with JIT...\n", n);

    {
        uint64_t raw = LLVMGetFunctionAddress(EE, "fib");
        int64_t(*usable)(int32_t) = (int64_t(*)(int32_t))raw;
        int64_t Ret = usable(n);
        // import result of execution
        fprintf(stdout, "Result: %lld\n", Ret);
    }

    retval = 0;

failure:
    if (EE)
        LLVMDisposeExecutionEngine(EE);
    if (Owner)
        LLVMDisposeModule(Owner);
    if (Context)
        LLVMContextDispose(Context);

    LLVMPrintVersion();
    return retval;
}
