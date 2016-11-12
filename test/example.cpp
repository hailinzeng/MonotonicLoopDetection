// RUN: %clang -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %opt -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %clang -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang %S/../../build/lib/MonotonicLoopDetection.so %t1.bc -o - | %lli | FileCheck %s

#include <stdio.h>
int main()
{
    int arr[100];

    //CHECK: Monotonic loop detected
    for (int i = 0 ; i < 15; i++) { 
       arr[i] = i+1;
    }

    return 0;
}

