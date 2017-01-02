// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
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
