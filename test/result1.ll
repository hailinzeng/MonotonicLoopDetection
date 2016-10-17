; ModuleID = '<stdin>'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZZ4mainE1a = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16

; Function Attrs: norecurse nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %a = alloca [7 x i32], align 16
  %b = alloca [7 x i32], align 16
  %add = add nsw i32 %argc, 1
  %0 = bitcast [7 x i32]* %a to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ4mainE1a to i8*), i64 28, i32 16, i1 false)
  %arrayinit.begin = getelementptr inbounds [7 x i32], [7 x i32]* %b, i64 0, i64 0
  store i32 1, i32* %arrayinit.begin, align 4
  %arrayinit.element = getelementptr inbounds i32, i32* %arrayinit.begin, i64 1
  store i32 2, i32* %arrayinit.element, align 4
  %arrayinit.element1 = getelementptr inbounds i32, i32* %arrayinit.element, i64 1
  store i32 3, i32* %arrayinit.element1, align 4
  %arrayinit.element2 = getelementptr inbounds i32, i32* %arrayinit.element1, i64 1
  store i32 4, i32* %arrayinit.element2, align 4
  %arrayinit.element3 = getelementptr inbounds i32, i32* %arrayinit.element2, i64 1
  store i32 5, i32* %arrayinit.element3, align 4
  %arrayinit.element4 = getelementptr inbounds i32, i32* %arrayinit.element3, i64 1
  store i32 6, i32* %arrayinit.element4, align 4
  %arrayinit.element5 = getelementptr inbounds i32, i32* %arrayinit.element4, i64 1
  store i32 %add, i32* %arrayinit.element5, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 3, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add6 = add nsw i32 %add, 1
  %idxprom = sext i32 %add6 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %b, i64 0, i64 %idxprom
  %1 = load i32, i32* %arrayidx, align 4
  %add7 = add nsw i32 %1, 100
  %idxprom8 = sext i32 %i.0 to i64
  %arrayidx9 = getelementptr inbounds [7 x i32], [7 x i32]* %a, i64 0, i64 %idxprom8
  store i32 %add7, i32* %arrayidx9, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %arrayidx10 = getelementptr inbounds [7 x i32], [7 x i32]* %a, i64 0, i64 0
  %2 = load i32, i32* %arrayidx10, align 16
  %add11 = add nsw i32 %add, %2
  ret i32 %add11
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
