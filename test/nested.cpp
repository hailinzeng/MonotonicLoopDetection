// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
int main() {
    int i, j;
    int arr[16][16];
    for (i = 0; i < 16; i ++){
        for (int j = 16-1; j >=0; j--) {
            arr[i][j] = arr[j][i];
        }
    }
    return 0;
}
