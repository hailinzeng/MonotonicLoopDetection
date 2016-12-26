// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s



// Fist OOB test

// CHECK-LABEL: entry:
// CHECK: call i1 @__check_array_min(
// CHECK: call i1 @__check_array_max(


// Function definition test
// CHECK: declare void @exit(i32)
// CHECK: define i1 @__check_array_min(i32 %idx, i32 %mn)
// CHECK: define i1 @__check_array_max(i32 %idx, i32 %mx)

void function()
{
        int a[100];
        for (int i = 99; i > 0; i--)
        {
                a[i] = i;
        }
}

