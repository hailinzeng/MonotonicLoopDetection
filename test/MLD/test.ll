; RUN: clang++ -std=c++11 -O0 -c -emit-llvm ../../../test/MLD/basic.cpp -o - | opt -mem2reg | llvm-dis > %t1.ll
; RUN: clang++ -std=c++11 -O0 -c -emit-llvm -Xclang -load -Xclang ../../../build/lib/MonotonicLoopDetection.so %t1.ll -o - | llvm-dis | FileCheck %s

; Function test

; CHECK: _Z8functionv

; Fist OOB test

; CHECK: [[REGISTER:%[a-z0-9]+]] = call i1 @__check_array_min(i32 [[REGISTER:%[a-z0-9]+]]

; CHECK-NEXT: [[REGISTER:%[a-z0-9]+]] = call i1 @__check_array_max(i32 [[REGISTER:%[a-z0-9]+]]

; CHECK-NEXT: [[REGISTER:%[a-z0-9]+]] = getelementptr


; Second OOB test

; CHECK: [[REGISTER:%[a-z0-9]+]] = call i1 @__check_array_min(i32 [[REGISTER:%[a-z0-9]+]]

; CHECK-NEXT: [[REGISTER:%[a-z0-9]+]] = call i1 @__check_array_max(i32 [[REGISTER:%[a-z0-9]+]]

; CHECK-NEXT: [[REGISTER:%[a-z0-9]+]] = getelementptr


; Function definition test

; CHECK: declare void @exit(i32)

; CHECK: define i1 @__check_array_min(i32 %idx, i32 %mn)

; CHECK: define i1 @__check_array_max(i32 %idx, i32 %mx)
