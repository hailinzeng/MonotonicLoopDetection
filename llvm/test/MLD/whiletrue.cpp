// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <stdlib.h>
#include <memory.h>

bool whiletrue(char* str) {
    while(true) {
        if (*str == 0)
            break;
        else {
            char c = *str;
            if (c == 'a' || c == 'e' || c == 'i') {
                return true;
            }
            str++;
        }
    }
    return false;
}

int main() {
    char arr[] = "Any thing except AEIou";
    char *lowarr = strdup(arr);
    int len = sizeof(arr);
    lowarr[len-3] = 'i';

    return whiletrue(arr) || whiletrue(lowarr);
}
