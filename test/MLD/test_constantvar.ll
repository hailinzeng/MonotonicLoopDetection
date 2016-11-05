; RUN: clang++ -std=c++11 -w -O0 -c -emit-llvm ../../../test/MLD/constantvar.cpp -o constantvar.bc
; RUN: opt -instnamer -mem2reg -break-crit-edges constantvar.bc -o constantvar.bc
; RUN: clang++ -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang ../../../build/lib/MonotonicLoopDetection.so constantvar.bc -o - | llvm-dis | FileCheck %s


; OOB test
; CHECK-LABEL: entry:
; CHECK: call i1 @__check_array_min(
; CHECK-NEXT: call i1 @__check_array_max(
; CHECK-NEXT: br label

; Function definition test
; CHECK: declare void @exit(i32)
; CHECK: define i1 @__check_array_min(i32 %idx, i32 %mn)
; CHECK: define i1 @__check_array_max(i32 %idx, i32 %mx)
