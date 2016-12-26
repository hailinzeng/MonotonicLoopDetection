// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <stdlib.h>

void memcpy_for(char* dst, char* src, size_t count) {
    for (int i = 0; i < count; i++) {
        dst[i] = src[i];
    }
}

void memcpy_for1(char* dst, char* src, size_t count) {
    int i = 0;
    for (; i < count; ) {
        dst[i] = src[i];
        ++i;
    }
}

int main() {
    char src[] = "what does the llvm say";
    int len = sizeof(src);
    char* dst = new char[len];
    char* dst1 = (char*)malloc(len);

    memcpy_for(dst, src, len);
    memcpy_for1(dst1, src, len);
    return 0;
}
