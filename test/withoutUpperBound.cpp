// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s


// Fist OOB test

// CHECK-LABEL: entry:
// CHECK: call i1 @__check_array_min(
// CHECK-NEXT: call {{i[0-9]+}} (i8*, ...) @printf(
// CHECK-NEXT: br label

// CHECK-LABEL: for.cond:
// CHECK: call i1 @__check_array_max(


// Function definition test
// CHECK: declare void @exit({{i[0-9]+}})
// CHECK: define i1 @__check_array_min({{i[0-9]+}} %idx, {{i[0-9]+}} %mn)
// CHECK: define i1 @__check_array_max({{i[0-9]+}} %idx, {{i[0-9]+}} %mx)


void function()
{
	int arr[100];
	for(unsigned int i = 0; ;i++)
	{
		arr[i+1] = i;
	}
}
