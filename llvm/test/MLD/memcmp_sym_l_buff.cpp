// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
   unsigned int s = 10;

   //buffer size gets concretized to 128
   char a[s]; 

   printf("S: %d\n", s);
   for (int i = 0; i < s-1 ; i++) {
	a[i] = i + '1';
   }
 
   const char b[500] = "123456789";
   
   //NOTE is l is not defined as unsigned, symbolic manipulation gets "noisy"
   unsigned int l = 5;

   //symbolic constrained by symbolic, nice!

   printf("A = %s\n", a);
   printf("B = %s\n", b);

   int out = memcmp(a, b, l);

   if (out == 0)
     printf("Mems are equal\n");
   else
     printf("Mems are NOT equal\n");
   return(0);
}







