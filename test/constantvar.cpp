// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o - | llvm-dis | FileCheck %s
// OOB test
// CHECK-LABEL: entry:
// CHECK: call i1 @__check_array_min(
// CHECK-NEXT: call i1 @__check_array_max(
// CHECK-NEXT: call {{i[0-9]+}} (i8*, ...) @printf(
// CHECK: br label

// Function definition test
// CHECK: declare void @exit({{i[0-9]+}})
// CHECK: define i1 @__check_array_min({{i[0-9]+}} %idx, {{i[0-9]+}} %mn)
// CHECK: define i1 @__check_array_max({{i[0-9]+}} %idx, {{i[0-9]+}} %mx)

void function()
{
	int arr[100];
	for(unsigned int i = 0; i < 10; i++)
	{
		arr[i] = i;
	}
}
