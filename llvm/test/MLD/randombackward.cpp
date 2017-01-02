// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK-LABEL: entry:
// not monotonic

#include <stdlib.h>

int randombackward(int *arr, int N) {
    int step = 0;
    int sum = 0;
    if (N <= sum) {
        return 0;
    }

    while (sum < N) {
        step ++;
        arr[sum] = step; // possible OOB
        if (rand() % 3 == 2) {
            sum--;
        } else {
            sum += (rand() % 3);
        }
    }

    return 0;
}

int main() {
    int len = 50;
    int *arr = new int(len);
    randombackward(arr, 50);
    return 0;
}
