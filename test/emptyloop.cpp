// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
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
