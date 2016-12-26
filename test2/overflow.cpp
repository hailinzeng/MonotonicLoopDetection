// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
// loop index overflow
// dead loop
int main() {
    char str[] = "what does the llvm say";
    char ret = '\0';
    for (unsigned i = sizeof(str) -1; i >= 0; i--) { // dead loop
        ret = str[i] - '0';
    }
    return 0;
}
