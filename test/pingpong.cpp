// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <string.h>

bool pingpong(char* str, int N) {
    for (int i = 0; i < N/2; i++) {
        if (str[i] == 'a' || str[N - 1 -i] == 'a') {
            return true;
        }
    }
    if (N % 2 == 0) {
        return str[N/2] == 'a';
    }
    return false;
}

int main() {
    char str[]= "How much is reduction in compile time if move very simple definition of function to .cpp?";
    return pingpong(str, strlen(str));
}
