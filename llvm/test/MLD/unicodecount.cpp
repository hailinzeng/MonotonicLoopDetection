// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s

// CHECK-NOT: is.monotonic

// CHECK-NOT: call i1 @__check_array_min
// CHECK-NOT: call i1 @__check_array_max

#include <stdlib.h>
#include <string.h>

size_t getWordNum(const char *pStr) {
    size_t wordNum = 0;
    size_t max_len = strlen(pStr);

    for (size_t i = 0; i < max_len; ) {
        unsigned char c = (unsigned char)(pStr[i]);
        //U-00000000 - U-0000007F: 0xxxxxxx
        if ( (c & 0x80) == 0) {
            i++;
        }
        //U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
        else if ( (c & 0xE0) == 0xC0) {
            if (pStr[i + 1] == '\0')
                return 0;
            i += 2;
        }
        //U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
        else if ((c & 0xF0) == 0xE0) {
            if (pStr[i + 1] == '\0' || pStr[i + 2] == '\0')
                return 0;
            i += 3;
        } else {
            return 0;
        }
        wordNum++;
    }
    return wordNum;
}

int main() {
  size_t c = getWordNum("我是123ABC");
  return 0;
}
