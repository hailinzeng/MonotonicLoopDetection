// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK-LABEL: entry:
int main() {
    // fine_aeoiu
    char str[] = "tyiuolm";
    int len = sizeof(str);
    for (int i = 0; i < len*2; i++) {
        char c = str[i];
        if (c == '\0') {
           goto err; 
        }
    }
err:
    return 0;
}
