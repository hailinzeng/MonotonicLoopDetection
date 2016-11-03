; RUN: clang++ -std=c++11 -w -O0 -c -emit-llvm ../../../test/MLD/basic.cpp -o basic.bc
; RUN: opt -instnamer -mem2reg -break-crit-edges basic.bc -o basic.bc
; RUN: clang++ -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang ../../../build/lib/MonotonicLoopDetection.so basic.bc -o - | llvm-dis | FileCheck %s


; Function test

; CHECK: _Z8functionv

; Fist OOB test

; CHECK: [[REGISTER:%[a-z0-9]+]] = call i1 @__check_array_min(

; CHECK-NEXT: [[REGISTER:%[a-z0-9]+]] = call i1 @__check_array_max(

; Second OOB test

; CHECK: [[REGISTER:%[a-z0-9]+]] = call i1 @__check_array_min(

; CHECK-NEXT: [[REGISTER:%[a-z0-9]+]] = call i1 @__check_array_max(



; Function definition test

; CHECK: declare void @exit(i32)

; CHECK: define i1 @__check_array_min(i32 %idx, i32 %mn)

; CHECK: define i1 @__check_array_max(i32 %idx, i32 %mx)
