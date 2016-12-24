// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <stdlib.h>
#include <string.h>

void shiftnota(char* arr, int N) {
    char* p = arr;
    char* q = arr;
    while (*q) {
        if (*q != 'a') {
            *p = *q;
            ++p;
        }
        ++q;
    }
}

int main() {
    char str[] = "How can I override another .bbappend";
    int N = strlen(str);
    shiftnota(str, N);

    return 0;
}
