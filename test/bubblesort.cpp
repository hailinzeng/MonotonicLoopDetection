// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK-LABEL: entry:

void bubblesort(int *arr, int length)
{
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        int i;
        for (i = 0; i < length - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                sorted = 0;
                int swap = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = swap;
            }
        }
    }
}

int main() {
    int arr[] = {12,4,6,12,67,1,7,1,854,2,41};
    int length = sizeof(arr)/sizeof(arr[0]);
    bubblesort(arr, length);
    return 0;
}
