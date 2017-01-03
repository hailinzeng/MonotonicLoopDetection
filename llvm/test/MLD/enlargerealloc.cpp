// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK-LABEL: entry:
#include <stdlib.h>
#include <memory.h>

const char* longtext = "Chinese students taking test in heavy smog draw outrage";

int main() {
    char* dst = strdup("");
    int dstlen = 1;

    const char* p = longtext;
    int len = strlen(p);
    for (int i = 0; i < len; i++) {
        if (i >= len) {
            dstlen = 2*dstlen;
            dst = (char*)realloc(dst, dstlen);
        }
        dst[i] = longtext[i];
    }
    return 0;
}
