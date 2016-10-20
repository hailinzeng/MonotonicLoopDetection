; ModuleID = 'result1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZZ13test_sucess_1vE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ13test_sucess_2vE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_1iE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_2iE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_3iE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_4vE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_5vE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_6iE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_7iE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_8iE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ11test_fail_9iE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ12test_fail_11iE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@_ZZ12test_fail_12vE3arr = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16

; Function Attrs: noinline nounwind uwtable
define i32 @_Z3fooi(i32 %x) #0 {
entry:
  %add = add nsw i32 %x, 1
  ret i32 %add
}

; Function Attrs: nounwind uwtable
define void @_Z13test_sucess_1v() #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ13test_sucess_1vE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 3
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
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
define void @_Z13test_sucess_2v() #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ13test_sucess_2vE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %g.0 = phi i32 [ 3, %entry ], [ %add, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %g.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
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
define void @_Z11test_fail_1i(i32 %arg) #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_1iE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 3, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, %arg
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 3, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z11test_fail_2i(i32 %argc) #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_2iE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, %argc
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %i.0, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z11test_fail_3i(i32 %argc) #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_3iE3arr to i8*), i64 28, i32 16, i1 false)
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
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z11test_fail_4v() #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_4vE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.cond
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

return:                                           ; No predecessors!
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z11test_fail_5v() #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_5vE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.cond, %entry
  %add = add nsw i32 0, 1
  %idxprom = sext i32 0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.cond

return:                                           ; No predecessors!
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z11test_fail_6i(i32 %argc) #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_6iE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %argc, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z11test_fail_7i(i32 %argc) #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_7iE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 5, %argc
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z11test_fail_8i(i32 %argc) #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_8iE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ %argc, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z11test_fail_9i(i32 %argc) #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ11test_fail_9iE3arr to i8*), i64 28, i32 16, i1 false)
  %cmp = icmp slt i32 %argc, 2
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %j.0 = phi i32 [ 4, %if.then ], [ 3, %entry ]
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %i.0 = phi i32 [ 0, %if.end ], [ %inc, %for.inc ]
  %cmp1 = icmp slt i32 %i.0, %j.0
  br i1 %cmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z12test_fail_11i(i32 %argc) #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ12test_fail_11iE3arr to i8*), i64 28, i32 16, i1 false)
  %cmp = icmp slt i32 %argc, 2
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %j.0 = phi i32 [ 4, %if.then ], [ 3, %entry ]
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %i.0 = phi i32 [ %j.0, %if.end ], [ %inc, %for.inc ]
  %cmp1 = icmp slt i32 %i.0, 10
  br i1 %cmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z12test_fail_12v() #1 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @_ZZ12test_fail_12vE3arr to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.cond, %entry
  %i.0 = phi i32 [ 10, %entry ], [ %inc, %for.cond ]
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom
  store i32 12, i32* %arrayidx, align 4
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

return:                                           ; No predecessors!
  ret void
}

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
