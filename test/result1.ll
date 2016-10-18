; ModuleID = 'result1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z3fooi(i32 %x) #0 {
entry:
  %add = add nsw i32 %x, 1
  ret i32 %add
}

; Function Attrs: norecurse nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #1 {
entry:
  %a = alloca [7 x i32], align 16
  %b = alloca [7 x i32], align 16
  %c = alloca [7 x i32], align 16
  %d = alloca [7 x i32], align 16
  %e = alloca [7 x i32], align 16
  %f = alloca [7 x i32], align 16
  %arrayinit.begin = getelementptr inbounds [7 x i32], [7 x i32]* %a, i64 0, i64 0
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
  store i32 %argc, i32* %arrayinit.element5, align 4
  %arrayinit.begin6 = getelementptr inbounds [7 x i32], [7 x i32]* %b, i64 0, i64 0
  store i32 1, i32* %arrayinit.begin6, align 4
  %arrayinit.element7 = getelementptr inbounds i32, i32* %arrayinit.begin6, i64 1
  store i32 2, i32* %arrayinit.element7, align 4
  %arrayinit.element8 = getelementptr inbounds i32, i32* %arrayinit.element7, i64 1
  store i32 3, i32* %arrayinit.element8, align 4
  %arrayinit.element9 = getelementptr inbounds i32, i32* %arrayinit.element8, i64 1
  store i32 4, i32* %arrayinit.element9, align 4
  %arrayinit.element10 = getelementptr inbounds i32, i32* %arrayinit.element9, i64 1
  store i32 5, i32* %arrayinit.element10, align 4
  %arrayinit.element11 = getelementptr inbounds i32, i32* %arrayinit.element10, i64 1
  store i32 6, i32* %arrayinit.element11, align 4
  %arrayinit.element12 = getelementptr inbounds i32, i32* %arrayinit.element11, i64 1
  store i32 %argc, i32* %arrayinit.element12, align 4
  %arrayinit.begin13 = getelementptr inbounds [7 x i32], [7 x i32]* %c, i64 0, i64 0
  store i32 1, i32* %arrayinit.begin13, align 4
  %arrayinit.element14 = getelementptr inbounds i32, i32* %arrayinit.begin13, i64 1
  store i32 2, i32* %arrayinit.element14, align 4
  %arrayinit.element15 = getelementptr inbounds i32, i32* %arrayinit.element14, i64 1
  store i32 3, i32* %arrayinit.element15, align 4
  %arrayinit.element16 = getelementptr inbounds i32, i32* %arrayinit.element15, i64 1
  store i32 4, i32* %arrayinit.element16, align 4
  %arrayinit.element17 = getelementptr inbounds i32, i32* %arrayinit.element16, i64 1
  store i32 5, i32* %arrayinit.element17, align 4
  %arrayinit.element18 = getelementptr inbounds i32, i32* %arrayinit.element17, i64 1
  store i32 6, i32* %arrayinit.element18, align 4
  %arrayinit.element19 = getelementptr inbounds i32, i32* %arrayinit.element18, i64 1
  store i32 %argc, i32* %arrayinit.element19, align 4
  %arrayinit.begin20 = getelementptr inbounds [7 x i32], [7 x i32]* %d, i64 0, i64 0
  store i32 1, i32* %arrayinit.begin20, align 4
  %arrayinit.element21 = getelementptr inbounds i32, i32* %arrayinit.begin20, i64 1
  store i32 2, i32* %arrayinit.element21, align 4
  %arrayinit.element22 = getelementptr inbounds i32, i32* %arrayinit.element21, i64 1
  store i32 3, i32* %arrayinit.element22, align 4
  %arrayinit.element23 = getelementptr inbounds i32, i32* %arrayinit.element22, i64 1
  store i32 4, i32* %arrayinit.element23, align 4
  %arrayinit.element24 = getelementptr inbounds i32, i32* %arrayinit.element23, i64 1
  store i32 5, i32* %arrayinit.element24, align 4
  %arrayinit.element25 = getelementptr inbounds i32, i32* %arrayinit.element24, i64 1
  store i32 6, i32* %arrayinit.element25, align 4
  %arrayinit.element26 = getelementptr inbounds i32, i32* %arrayinit.element25, i64 1
  store i32 %argc, i32* %arrayinit.element26, align 4
  %arrayinit.begin27 = getelementptr inbounds [7 x i32], [7 x i32]* %e, i64 0, i64 0
  store i32 1, i32* %arrayinit.begin27, align 4
  %arrayinit.element28 = getelementptr inbounds i32, i32* %arrayinit.begin27, i64 1
  store i32 2, i32* %arrayinit.element28, align 4
  %arrayinit.element29 = getelementptr inbounds i32, i32* %arrayinit.element28, i64 1
  store i32 3, i32* %arrayinit.element29, align 4
  %arrayinit.element30 = getelementptr inbounds i32, i32* %arrayinit.element29, i64 1
  store i32 4, i32* %arrayinit.element30, align 4
  %arrayinit.element31 = getelementptr inbounds i32, i32* %arrayinit.element30, i64 1
  store i32 5, i32* %arrayinit.element31, align 4
  %arrayinit.element32 = getelementptr inbounds i32, i32* %arrayinit.element31, i64 1
  store i32 6, i32* %arrayinit.element32, align 4
  %arrayinit.element33 = getelementptr inbounds i32, i32* %arrayinit.element32, i64 1
  store i32 %argc, i32* %arrayinit.element33, align 4
  %arrayinit.begin34 = getelementptr inbounds [7 x i32], [7 x i32]* %f, i64 0, i64 0
  store i32 1, i32* %arrayinit.begin34, align 4
  %arrayinit.element35 = getelementptr inbounds i32, i32* %arrayinit.begin34, i64 1
  store i32 2, i32* %arrayinit.element35, align 4
  %arrayinit.element36 = getelementptr inbounds i32, i32* %arrayinit.element35, i64 1
  store i32 3, i32* %arrayinit.element36, align 4
  %arrayinit.element37 = getelementptr inbounds i32, i32* %arrayinit.element36, i64 1
  store i32 4, i32* %arrayinit.element37, align 4
  %arrayinit.element38 = getelementptr inbounds i32, i32* %arrayinit.element37, i64 1
  store i32 5, i32* %arrayinit.element38, align 4
  %arrayinit.element39 = getelementptr inbounds i32, i32* %arrayinit.element38, i64 1
  store i32 6, i32* %arrayinit.element39, align 4
  %arrayinit.element40 = getelementptr inbounds i32, i32* %arrayinit.element39, i64 1
  store i32 %argc, i32* %arrayinit.element40, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 3, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %i.0, %argc
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds [7 x i32], [7 x i32]* %a, i64 0, i64 %idxprom, !monotonic.fail.index !1
  store i32 3, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  br label %for.cond41

for.cond41:                                       ; preds = %for.inc47, %for.end
  %j.0 = phi i32 [ 1, %for.end ], [ %inc48, %for.inc47 ]
  %cmp42 = icmp slt i32 %j.0, 3
  br i1 %cmp42, label %for.body43, label %for.end49

for.body43:                                       ; preds = %for.cond41
  %add44 = add nsw i32 %j.0, 3
  %idxprom45 = sext i32 %j.0 to i64
  %arrayidx46 = getelementptr inbounds [7 x i32], [7 x i32]* %b, i64 0, i64 %idxprom45, !monotonic.safe.index !1
  store i32 %add44, i32* %arrayidx46, align 4
  br label %for.inc47

for.inc47:                                        ; preds = %for.body43
  %inc48 = add nsw i32 %j.0, 1
  br label %for.cond41

for.end49:                                        ; preds = %for.cond41
  br label %for.cond50

for.cond50:                                       ; preds = %for.inc56, %for.end49
  %g.0 = phi i32 [ 3, %for.end49 ], [ %add55, %for.inc56 ]
  %k.0 = phi i32 [ 1, %for.end49 ], [ %inc57, %for.inc56 ]
  %cmp51 = icmp slt i32 %k.0, 3
  br i1 %cmp51, label %for.body52, label %for.end58

for.body52:                                       ; preds = %for.cond50
  %idxprom53 = sext i32 %g.0 to i64
  %arrayidx54 = getelementptr inbounds [7 x i32], [7 x i32]* %c, i64 0, i64 %idxprom53, !monotonic.safe.index !1
  store i32 %g.0, i32* %arrayidx54, align 4
  %add55 = add nsw i32 %g.0, 2
  br label %for.inc56

for.inc56:                                        ; preds = %for.body52
  %inc57 = add nsw i32 %k.0, 1
  br label %for.cond50

for.end58:                                        ; preds = %for.cond50
  br label %for.cond59

for.cond59:                                       ; preds = %for.inc66, %for.end58
  %m.0 = phi i32 [ 1, %for.end58 ], [ %inc67, %for.inc66 ]
  %cmp60 = icmp slt i32 %m.0, 3
  br i1 %cmp60, label %for.body61, label %for.end68

for.body61:                                       ; preds = %for.cond59
  %add62 = add nsw i32 %m.0, 1
  %add63 = add nsw i32 %argc, %m.0
  %call = call i32 @_Z3fooi(i32 %add63)
  %idxprom64 = sext i32 %call to i64
  %arrayidx65 = getelementptr inbounds [7 x i32], [7 x i32]* %d, i64 0, i64 %idxprom64, !monotonic.fail.index !1
  store i32 %add62, i32* %arrayidx65, align 4
  br label %for.inc66

for.inc66:                                        ; preds = %for.body61
  %inc67 = add nsw i32 %m.0, 1
  br label %for.cond59

for.end68:                                        ; preds = %for.cond59
  br label %for.cond69

for.cond69:                                       ; preds = %for.inc75, %for.end68
  %m2.0 = phi i32 [ 1, %for.end68 ], [ %inc76, %for.inc75 ]
  %cmp70 = icmp slt i32 %m2.0, 3
  br i1 %cmp70, label %for.body71, label %for.end77

for.body71:                                       ; preds = %for.cond69
  %add72 = add nsw i32 %m2.0, 1
  %idxprom73 = sext i32 %argc to i64
  %arrayidx74 = getelementptr inbounds [7 x i32], [7 x i32]* %e, i64 0, i64 %idxprom73, !monotonic.fail.index !1
  store i32 %add72, i32* %arrayidx74, align 4
  br label %for.inc75

for.inc75:                                        ; preds = %for.body71
  %inc76 = add nsw i32 %m2.0, 1
  br label %for.cond69

for.end77:                                        ; preds = %for.cond69
  br label %for.cond78

for.cond78:                                       ; preds = %for.inc82, %for.end77
  %l.0 = phi i32 [ 1, %for.end77 ], [ %inc83, %for.inc82 ]
  %add79 = add nsw i32 %l.0, 1
  %idxprom80 = sext i32 %l.0 to i64
  %arrayidx81 = getelementptr inbounds [7 x i32], [7 x i32]* %f, i64 0, i64 %idxprom80, !monotonic.unsafe.index !1
  store i32 %add79, i32* %arrayidx81, align 4
  br label %for.inc82

for.inc82:                                        ; preds = %for.cond78
  %inc83 = add nsw i32 %l.0, 1
  br label %for.cond78

return:                                           ; No predecessors!
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
!1 = !{!"monotonic"}
