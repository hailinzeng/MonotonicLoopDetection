; ModuleID = 'example.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [26 x i8] c"Monotonic loop detected!\0A\00"
@1 = private unnamed_addr constant [19 x i8] c"Assertion failed!\0A\00"

; Function Attrs: nounwind uwtable
define void @_Z8functionv() #0 {
entry:
  %a = alloca [100 x i32], align 16
  %0 = call i1 @__check_array_max(i32 99, i32 100)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 99, %entry ], [ %dec, %for.inc ], !monotonic.safe !1
  %1 = call i1 @__check_array_min(i32 %i.0, i32 0)
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [100 x i32], [100 x i32]* %a, i64 0, i64 %idxprom, !is.monotonic !1
  store i32 %i.0, i32* %arrayidx, align 4, !is.monotonic !1
  br label %for.inc

for.inc:                                          ; preds = %for.cond
  %dec = add nsw i32 %i.0, -1
  br label %for.cond

return:                                           ; No predecessors!
  ret void
}

declare i32 @printf(i8* noalias, ...)

declare void @exit(i32)

define i1 @__check_array_min(i32 %idx, i32 %mn) {
entry:
  %tmp = icmp sge i32 %idx, %mn
  br i1 %tmp, label %return, label %cond_false

return:                                           ; preds = %entry
  ret i1 %tmp

cond_false:                                       ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @1, i32 0, i32 0))
  call void @exit(i32 -1)
  ret i1 %tmp
}

define i1 @__check_array_max(i32 %idx, i32 %mx) {
entry:
  %tmp = icmp slt i32 %idx, %mx
  br i1 %tmp, label %return, label %cond_false

return:                                           ; preds = %entry
  ret i1 %tmp

cond_false:                                       ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @1, i32 0, i32 0))
  call void @exit(i32 -1)
  ret i1 %tmp
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
!1 = !{!"monotonicity"}
