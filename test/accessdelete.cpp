// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <stdlib.h>
#include <string.h>

int main() {
    char * arr = strdup("nested function change variable in an outside function not working");
    int len = strlen(arr);

    for (int i = 0; i < len; i++) {
        if (arr[i] == 'v') {
           arr[i] = 'V';
           free(arr);
        }
    }
    return 0;
}