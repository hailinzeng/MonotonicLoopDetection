; RUN: clang++ -std=c++11 -w -O0 -c -emit-llvm ../../../test/MLD/no_monotonic.cpp -o no_monotonic.bc
; RUN: opt -instnamer -mem2reg -break-crit-edges no_monotonic.bc -o no_monotonic.bc
; RUN: clang++ -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang ../../../build/lib/MonotonicLoopDetection.so no_monotonic.bc -o - | llvm-dis | FileCheck %s

; OOB test

; CHECK-NOT: call i1 @__check_array_min(
; CHECK-NOT: call i1 @__check_array_max(

; Function definition test
; CHECK-NOT: declare void @exit(i32)
; CHECK-NOT: define i1 @__check_array_min(i32 %idx, i32 %mn)
; CHECK-NOT: define i1 @__check_array_max(i32 %idx, i32 %mx)
