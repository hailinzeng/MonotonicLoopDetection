// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK: is.monotonic

// CHECK: call i1 @__check_array_min

int main() {
    int* arr = new int[32];
    arr[31] = 0;
    // for (unsigned i = 31-1; i >= 0; i--) {
    for (unsigned i = 31-1; i >= 1; i--) {
        arr[i] = arr[i+1] + i;
        // if (i == 0)
        //     break;
    }
    return 0;
}
