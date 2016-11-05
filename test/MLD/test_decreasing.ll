; RUN: clang++ -std=c++11 -w -O0 -c -emit-llvm ../../../test/MLD/decreasing.cpp -o decreasing.bc
; RUN: opt -instnamer -mem2reg -break-crit-edges decreasing.bc -o decreasing.bc
; RUN: clang++ -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang ../../../build/lib/MonotonicLoopDetection.so decreasing.bc -o - | llvm-dis | FileCheck %s


; Fist OOB test

; CHECK-LABEL: entry:
; CHECK: call i1 @__check_array_min(
; CHECK: call i1 @__check_array_max(


; Function definition test
; CHECK: declare void @exit(i32)
; CHECK: define i1 @__check_array_min(i32 %idx, i32 %mn)
; CHECK: define i1 @__check_array_max(i32 %idx, i32 %mx)
