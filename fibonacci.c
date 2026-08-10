//===--- msvc-llvm-workspace/fibonacci.c - An example use of the LLVM C API -----===//
#include <stdbool.h>
#include <stdio.h>
#include "llvm-c/Core.h"
#include "llvm-c/Target.h"
#include "llvm-c/ExecutionEngine.h"

LLVMValueRef CreateFib(LLVMModuleRef M, LLVMContextRef Context) {
    // Create the fib function and insert it into module M. This function is said
    // to return an int and take an int parameter.
    LLVMTypeRef FibRTy[1] = { LLVMInt32TypeInContext(Context) };
    LLVMTypeRef FibFTy = LLVMFunctionType(LLVMInt32TypeInContext(Context), FibRTy, 1, false);
    LLVMValueRef FibF = LLVMAddFunction(M, "fib", FibFTy);
    LLVMSetLinkage(FibF, LLVMExternalLinkage);

    // Add a basic block to the function.
    LLVMBasicBlockRef BB = LLVMAppendBasicBlockInContext(Context, FibF, "EntryBlock");

    // Get pointers to the contants.
    LLVMValueRef One = LLVMConstInt(LLVMInt32TypeInContext(Context), 1, true);
    LLVMValueRef Two = LLVMConstInt(LLVMInt32TypeInContext(Context), 2, true);

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
    LLVMBuildRet(Builder, One);
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

int main(int argc, const char* argv[]) {
    int32_t n = argc > 1 ? atol(argv[1]) : 24;
    LLVMContextRef Context = NULL;
    LLVMModuleRef Owner = NULL;
    LLVMValueRef FibF = NULL;
    LLVMExecutionEngineRef EE = NULL;
    char *errStr = NULL;
    int retval = 1;

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmParser();

    Context = LLVMContextCreate();
    if (!Context)
        goto failure;

    // Create some module to put our function into it.
    Owner = LLVMModuleCreateWithNameInContext("test", Context);
    if (!Owner)
        goto failure;

    // We are about to create the "fib" function:
    FibF = CreateFib(Owner, Context);
    if (!FibF)
        goto failure;

    fprintf(stderr, "verifying...\n");
    fprintf(stderr, "OK\n");

    fprintf(stderr, "We just constructed this LLVM module:\n\n---------\n");

    LLVMDumpModule(Owner);

    // Now we going to create EE
    if (LLVMCreateExecutionEngineForModule(&EE, Owner, &errStr)) {
        fprintf(stderr, "Failed to construct ExecutionEngine: %s\n", errStr);
        LLVMDisposeMessage(errStr);
        goto failure;
    }

    Owner = NULL;

    fprintf(stderr, "---------\nstarting fibonacci(%d) with JIT...\n", n);

    {
        // Call the Fibonacci function with argument n:
        LLVMGenericValueRef Val[1] = {LLVMCreateGenericValueOfInt(LLVMInt32TypeInContext(Context), n, true)};
        LLVMGenericValueRef ReturnValue = LLVMRunFunction(EE, FibF, 1, Val);
        int32_t Ret = LLVMGenericValueToInt(ReturnValue, true);
        // import result of execution
        fprintf(stdout, "Result: %d\n", Ret);
        LLVMDisposeGenericValue(ReturnValue);
        LLVMDisposeGenericValue(Val[0]);
    }

    retval = 0;

failure:
    if (EE)
        LLVMDisposeExecutionEngine(EE);
    if (Owner)
        LLVMDisposeModule(Owner);
    if (Context)
        LLVMContextDispose(Context);
    return retval;
}
