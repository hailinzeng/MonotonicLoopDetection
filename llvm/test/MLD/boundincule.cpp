// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK: call i1 @__check_array_min

// CHECK: define i1 @__check_array_min({{i[0-9]+}} %idx, {{i[0-9]+}} %mn)



int main() {
    int* arr = new int[32];
    arr[0] = 0;
    for (unsigned i = 1; i <= 31; i++) {
        arr[i] = arr[i-1] + i;
    }
    return 0;
}
