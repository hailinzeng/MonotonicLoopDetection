; ModuleID = 'result1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZZ5test1iE4arr1 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ5test2vE4arr2 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ5test3vE4arr3 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ5test4iE4arr4 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ5test5iE4arr5 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ5test6vE4arr6 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ5test7vE4arr7 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ5test8iE4arr8 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ5test9iE4arr9 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ6test10iE5arr10 = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16

; Function Attrs: noinline nounwind uwtable
define i32 @_Z3fooi(i32 %x) #0 {
entry:
  %add = add nsw i32 %x, 1
  ret i32 %add
}

; Function Attrs: nounwind uwtable
define void @_Z5test1i(i32 %arg) #1 {
entry:
  %arr1 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test1iE4arr1 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 3, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, %arg
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr1, i64 0, i64 %idxprom, !monotonic.fail.index !1
  store i32 3, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

; Function Attrs: nounwind uwtable
define void @_Z5test2v() #1 {
entry:
  %arr2 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test2vE4arr2 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 3
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr2, i64 0, i64 %idxprom, !monotonic.safe.index !1
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z5test3v() #1 {
entry:
  %arr3 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test3vE4arr3 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %g.0 = phi i32 [ 3, %entry ], [ %add, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %g.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr3, i64 0, i64 %idxprom, !monotonic.safe.index !1
  store i32 %g.0, i32* %arrayidx, align 4
  %add = add nsw i32 %g.0, 2
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z5test4i(i32 %argc) #1 {
entry:
  %arr4 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test4iE4arr4 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %add1 = add nsw i32 %argc, %i.0
  %call = call i32 @_Z3fooi(i32 %add1)
  %idxprom = sext i32 %call to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr4, i64 0, i64 %idxprom, !monotonic.fail.index !1
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z5test5i(i32 %argc) #1 {
entry:
  %arr5 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test5iE4arr5 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %argc to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr5, i64 0, i64 %idxprom, !monotonic.fail.index !1
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z5test6v() #1 {
entry:
  %arr6 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test6vE4arr6 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr6, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.cond
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

return:                                           ; No predecessors!
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z5test7v() #1 {
entry:
  %arr7 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr7 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test7vE4arr7 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.cond, %entry
  %add = add nsw i32 0, 1
  %idxprom = sext i32 0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr7, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.cond

return:                                           ; No predecessors!
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z5test8i(i32 %argc) #1 {
entry:
  %arr8 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr8 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test8iE4arr8 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %argc, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr8, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z5test9i(i32 %argc) #1 {
entry:
  %arr9 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ5test9iE4arr9 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 5, %argc
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr9, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z6test10i(i32 %argc) #1 {
entry:
  %arr10 = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr10 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ6test10iE5arr10 to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ %argc, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr10, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: norecurse nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #3 {
entry:
  call void @_Z5test1i(i32 %argc)
  call void @_Z5test2v()
  call void @_Z5test3v()
  call void @_Z5test4i(i32 %argc)
  call void @_Z5test5i(i32 %argc)
  call void @_Z5test6v()
  call void @_Z5test7v()
  call void @_Z5test8i(i32 %argc)
  call void @_Z5test9i(i32 %argc)
  call void @_Z6test10i(i32 %argc)
  ret i32 %argc
}

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
!1 = !{!"monotonic"}
