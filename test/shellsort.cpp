// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// CHECK-LABEL: entry:
void shell_sort(int* array,int size)
{
    int increment,i,j,k,temp;
    for(increment = size/2; increment > 0; increment /= 2)
    {
        for(i = 0; i < increment; i++)
        {
            for(j = i + increment; j < size; j += increment)
            {
                k = j;
                while((k > i)&&(array[k-increment] > array[k]))
                {
                    temp = array[k-increment];
                    array[k-increment] = array[k];
                    array[k] = temp;
                    k -= increment;
                }
            }
        }
    }
}

int main(){
    int arr[] = {9, 1, 0, 13, 04, 0x14, 011};
    int len = sizeof(arr)/sizeof(arr[0]);
    shell_sort(arr, len);
    return 0;
}
