// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
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
