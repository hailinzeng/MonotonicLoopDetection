// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK-LABEL: entry:
// loop bound > array, but break from internal

int main() {
    // fine_aeoiu
    char str[] = "tyiuolm";
    int len = sizeof(str);
    for (int i = 0; i < len*2; i++) {
        char c = str[i];
        if (c == 'u') {
            break;
        }
    }
    return 0;
}
