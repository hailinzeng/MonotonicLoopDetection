// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <stdlib.h>
#include <memory.h>

void merge(int* array,int halfsize,int totalsize)
{
    //array is seperated to two part.one part is from 0 to halfsize-1
    //the other is from halfsize to totalsize-1
    int *temparray = (int*)malloc(sizeof(int) * totalsize);
    int i = 0,j = halfsize,index = 0;
    while((i < halfsize) && (j <totalsize))
    {
        if(array[i] > array[j])
            temparray[index++] = array[j++];
        else
            temparray[index++] = array[i++];
    }
    //if the previous part travel over this loop will not excute
    while(i < halfsize)
        temparray[index++] = array[i++];
    //if the rear part travle over this loop will not excute
    while(j < totalsize)
        temparray[index++] = array[j++];
    memcpy(array,temparray,sizeof(int) * totalsize);
    free(temparray);
}

void merge_sort(int* array,int size)
{
    if(size < 2)
        return;
    merge_sort(array,size/2);
    merge_sort(array + size/2,size - size/2);
    merge(array,size/2,size);
}

int main(){
    int arr[] = {9, 1, 0, 13, 04, 0x14, 011};
    int len = sizeof(arr)/sizeof(arr[0]);
    merge_sort(arr, len);
    return 0;
}
