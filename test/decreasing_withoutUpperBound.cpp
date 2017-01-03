// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s

// Fist OOB test

// CHECK-LABEL: entry:
// CHECK: call i1 @__check_array_max(
// CHECK-NEXT: call {{i[0-9]+}} (i8*, ...) @printf(
// CHECK: br label

// CHECK-LABEL: for.cond:
// CHECK: call i1 @__check_array_min(


// Function definition test
// CHECK: declare void @exit({{i[0-9]+}})
// CHECK: define i1 @__check_array_min({{i[0-9]+}} %idx, {{i[0-9]+}} %mn)
// CHECK: define i1 @__check_array_max({{i[0-9]+}} %idx, {{i[0-9]+}} %mx)

void function()
{
        int a[100];
        for (int i = 99; ; i--)
        {
                a[i] = i;
        }
}

