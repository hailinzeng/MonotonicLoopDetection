// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
void emptyloop(char* str) {
    for (;*str; str++) {
    }
}
int main() {
    char str[] ="Unable to change profile picture while wearing hat";
    emptyloop(str);
    return 0;
}
