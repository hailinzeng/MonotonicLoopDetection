// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <stdlib.h>

char* strchr_ud(char* str, char c) {
    while(*str) {
        if (c == *str) {
            return str;
        }
    }
    return NULL;
}

int main() {
    char str[] = "to be or not to be";
    strchr_ud(str, 'x');
    strchr_ud(str, 'n');
    return 0;
}
