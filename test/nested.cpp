// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
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
