// RUN: %clang -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %opt -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %clang -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang %S/../../build/lib/MonotonicLoopDetection.so %t1.bc -o - | llvm-dis | FileCheck %s

// OOB test

// CHECK-NOT: call i1 @__check_array_min(
// CHECK-NOT: call i1 @__check_array_max(

// Function definition test
// CHECK-NOT: declare void @exit(i32)
// CHECK-NOT: define i1 @__check_array_min(i32 %idx, i32 %mn)
// CHECK-NOT: define i1 @__check_array_max(i32 %idx, i32 %mx)

void function()
{
	int arr[100];
	for(unsigned int i = 0; i < 10; i++)
	{
		arr[i] = 1 - i;
	}

	for (int i = 0 ; i < 50; i++) {
	       arr[125 - i] = i;
	}
}
