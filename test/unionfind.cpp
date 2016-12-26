// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
// random jump, not monotonic
int find_root(int *arr, int node) {
    int i = 0;
    while (arr[i] != 0) {
        i = arr[i];
    }
    return i;
}

int main() {
    int arr[] = {0, 0, 1, 2, 6, 2, 3};
    find_root(arr, 4);
    return 0;
}
