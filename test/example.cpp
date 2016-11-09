// RUN: %clang -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %opt -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %clang -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang %S/../../build/lib/MonotonicLoopDetection.so %t1.bc -o - | %lli | FileCheck %s

// CHECK: Monotonic loop detected!


#include <stdio.h>
int main()
{
    int arr[100];

    printf(" ... here MLD pass should be inserting printf with 'Monotonic...' .");
    for (int i = 0 ; i < 15; i++){
       arr[i] = i+1;
    }

    return 0;
}
