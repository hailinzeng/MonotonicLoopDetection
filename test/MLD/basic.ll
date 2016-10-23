; RUN: lli %s
; RUN: llvm-as %s -o %t1
; RUN: lli %t1


%class.anon = type { i8 }
%class.anon.0 = type { i8 }
%class.anon.2 = type { i8 }
%class.anon.4 = type { i8 }
%class.anon.6 = type { i8 }

@"_ZZZ4mainENK3$_0clEvE3arr" = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@"_ZZZ4mainENK3$_1clEvE3arr" = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@"_ZZZ4mainENK3$_2clEiE3arr" = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@"_ZZZ4mainENK3$_3clEiE3arr" = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16
@"_ZZZ4mainENK3$_4clEvE3arr" = private unnamed_addr constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7], align 16

define i32 @_Z3fooi(i32 %x) {
entry:
  %add = add nsw i32 %x, 1
  ret i32 %add
}

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %t1 = alloca %class.anon, align 1
  %t2 = alloca %class.anon.0, align 1
  %t3 = alloca %class.anon.2, align 1
  %t4 = alloca %class.anon.4, align 1
  %t6 = alloca %class.anon.6, align 1
  call void @"_ZZ4mainENK3$_0clEv"(%class.anon* %t1)
  call void @"_ZZ4mainENK3$_1clEv"(%class.anon.0* %t2)
  call void @"_ZZ4mainENK3$_2clEi"(%class.anon.2* %t3, i32 1)
  call void @"_ZZ4mainENK3$_3clEi"(%class.anon.4* %t4, i32 1)
  call void @"_ZZ4mainENK3$_4clEv"(%class.anon.6* %t6)
  ret i32 0
}

; Function Attrs: inlinehint nounwind uwtable
define internal void @"_ZZ4mainENK3$_0clEv"(%class.anon* %this) align 2 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @"_ZZZ4mainENK3$_0clEvE3arr" to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, 3
  %idxprom = sext i32 %i.0 to i64
  %1 = call i1 @__check_array_min(i32 %i.0, i32 0)
  %2 = call i1 @__check_array_max(i32 %i.0, i32 7)
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom, !SAFE !1
  store i32 %add, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define internal void @"_ZZ4mainENK3$_1clEv"(%class.anon.0* %this) align 2 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @"_ZZZ4mainENK3$_1clEvE3arr" to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %g.0 = phi i32 [ 0, %entry ], [ %add, %for.inc ]
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %g.0 to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom, !SAFE !1
  store i32 %g.0, i32* %arrayidx, align 4
  %add = add nsw i32 %g.0, 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define internal void @"_ZZ4mainENK3$_2clEi"(%class.anon.2* %this, i32 %arg) align 2 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @"_ZZZ4mainENK3$_2clEiE3arr" to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 3, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, %arg
  %idxprom = sext i32 %add to i64
  %1 = call i1 @__check_array_min(i32 %add, i32 0)
  %2 = call i1 @__check_array_max(i32 %add, i32 7)
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom, !SAFE !1
  store i32 3, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define internal void @"_ZZ4mainENK3$_3clEi"(%class.anon.4* %this, i32 %arg) align 2 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @"_ZZZ4mainENK3$_3clEiE3arr" to i8*), i64 28, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 1, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, %arg
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %i.0 to i64
  %1 = call i1 @__check_array_min(i32 %i.0, i32 0)
  %2 = call i1 @__check_array_max(i32 %i.0, i32 7)
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom, !SAFE !1
  store i32 %i.0, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define internal void @"_ZZ4mainENK3$_4clEv"(%class.anon.6* %this) align 2 {
entry:
  %arr = alloca [7 x i32], align 16
  %0 = bitcast [7 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([7 x i32]* @"_ZZZ4mainENK3$_4clEvE3arr" to i8*), i64 28, i32 16, i1 false)
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %while.body ]
  %cmp = icmp slt i32 %i.0, 6
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %idxprom = sext i32 %i.0 to i64
  %1 = call i1 @__check_array_min(i32 %i.0, i32 0)
  %2 = call i1 @__check_array_max(i32 %i.0, i32 7)
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %arr, i64 0, i64 %idxprom, !SAFE !1
  store i32 3, i32* %arrayidx, align 4
  %inc = add nsw i32 %i.0, 1
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #3

declare void @exit(i32)

define i1 @__check_array_min(i32 %idx, i32 %mn) {
entry:
  %tmp = icmp sge i32 %idx, %mn
  br i1 %tmp, label %return, label %cond_false

return:                                           ; preds = %entry
  ret i1 %tmp

cond_false:                                       ; preds = %entry
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
  call void @exit(i32 -1)
  ret i1 %tmp
}

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
!1 = !{!"monotonic.loop"}
