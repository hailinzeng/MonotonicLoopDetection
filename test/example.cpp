// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s

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

