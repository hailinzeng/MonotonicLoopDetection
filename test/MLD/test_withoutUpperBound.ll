; RUN: clang++ -std=c++11 -w -O0 -c -emit-llvm ../../../test/MLD/withoutUpperBound.cpp -o withoutUpperBound.bc
; RUN: opt -instnamer -mem2reg -break-crit-edges withoutUpperBound.bc -o withoutUpperBound.bc
; RUN: clang++ -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang ../../../build/lib/MonotonicLoopDetection.so withoutUpperBound.bc -o - | llvm-dis | FileCheck %s


; Fist OOB test

; CHECK-LABEL: entry:
; CHECK: call i1 @__check_array_min(
; CHECK-NEXT: call i32 (i8*, ...) @printf(
; CHECK-NEXT: br label

; CHECK-LABEL: for.cond:
; CHECK: call i1 @__check_array_max(


; Function definition test
; CHECK: declare void @exit(i32)
; CHECK: define i1 @__check_array_min(i32 %idx, i32 %mn)
; CHECK: define i1 @__check_array_max(i32 %idx, i32 %mx)
