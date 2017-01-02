// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK: is.monotonic

// CHECK: call i1 @__check_array_min

int main() {
    int* arr = new int[32];
    arr[0] = 0;
    for (unsigned i = 1; i < 32; i++) {
        arr[i] = arr[i-1] + i;
    }
    return 0;
}
