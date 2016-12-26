// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <stdlib.h>

int randjump(int *arr, int N) {
    int step = 0;
    int sum = 0;
    if (N <= sum) {
        return 0;
    }

    while (sum < N) {
        arr[sum] = step;
        step ++;
        sum += rand()%3;
    }
    return step;
}

int main() {
    int len = 50;
    int *arr = new int(len);
    randjump(arr, 50);
    return 0;
}
