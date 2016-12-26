; ModuleID = 'example.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@n_bits = global i32 0, align 4
@maxbits = global i32 16, align 4
@maxcode = global i64 0, align 8
@maxmaxcode = global i64 65536, align 8
@hsize = global i64 257, align 8
@fsize = global i64 0, align 8
@free_ent = global i64 0, align 8
@exit_stat = global i32 0, align 4
@nomagic = global i32 1, align 4
@zcat_flg = global i32 0, align 4
@quiet = global i32 1, align 4
@block_compress = global i32 128, align 4
@clear_flg = global i32 0, align 4
@ratio = global i64 0, align 8
@checkpoint = global i64 10000, align 8
@force = global i32 0, align 4
@ofname = global [100 x i8] zeroinitializer, align 16
@InCnt = global i32 0, align 4
@apsim_InCnt = global i32 0, align 4
@InBuff = global i8* null, align 8
@OutBuff = global i8* null, align 8
@orig_text_buffer = global [50 x i8] zeroinitializer, align 16
@comp_text_buffer = global [55 x i8] zeroinitializer, align 16
@htab = global [257 x i64] zeroinitializer, align 16
@codetab = global [257 x i16] zeroinitializer, align 16
@buf = global [16 x i8] zeroinitializer, align 16
@in_count = global i64 1, align 8
@bytes_out = global i64 0, align 8
@out_count = global i64 0, align 8
@_ZL6offset = internal global i32 0, align 4
@lmask = global [9 x i8] c"\FF\FE\FC\F8\F0\E0\C0\80\00", align 1
@rmask = global [9 x i8] c"\00\01\03\07\0F\1F?\7F\FF", align 1
@0 = private unnamed_addr constant [26 x i8] c"Monotonic loop detected!\0A\00"
@1 = private unnamed_addr constant [19 x i8] c"Assertion failed!\0A\00"

; Function Attrs: norecurse uwtable
define i32 @main() #0 {
entry:
  call void @_Z10initbufferv()
  store i32 16, i32* @maxbits, align 4
  %tmp = load i32, i32* @maxbits, align 4
  %shl = shl i32 1, %tmp
  %conv = sext i32 %shl to i64
  store i64 %conv, i64* @maxmaxcode, align 8
  store i32 50, i32* @InCnt, align 4
  store i32 53, i32* @apsim_InCnt, align 4
  store i8* getelementptr inbounds ([50 x i8], [50 x i8]* @orig_text_buffer, i32 0, i32 0), i8** @InBuff, align 8
  store i8* getelementptr inbounds ([55 x i8], [55 x i8]* @comp_text_buffer, i32 0, i32 0), i8** @OutBuff, align 8
  call void @_Z8compressv()
  ret i32 0
}

; Function Attrs: nounwind uwtable
define void @_Z10initbufferv() #1 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %seed.0 = phi i32 [ 1, %entry ], [ %rem, %for.inc ]
  %cmp = icmp slt i32 %i.0, 50
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %mul = mul nsw i32 %seed.0, 133
  %add = add nsw i32 %mul, 81
  %rem = srem i32 %add, 8095
  %rem1 = srem i32 %rem, 256
  %conv = trunc i32 %rem1 to i8
  %0 = call i1 @__check_array_min(i32 %i.0, i32 0)
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [50 x i8], [50 x i8]* @orig_text_buffer, i64 0, i64 %idxprom, !not.monotonic.or.unknown !1
  store i8 %conv, i8* %arrayidx, align 1, !not.monotonic.or.unknown !1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: uwtable
define void @_Z8compressv() #2 {
entry:
  %fcode = alloca i64, align 8
  %i = alloca i64, align 8
  %c = alloca i32, align 4
  %ent = alloca i64, align 8
  %disp = alloca i32, align 4
  %hsize_reg = alloca i64, align 8
  %hshift = alloca i32, align 4
  %apsim_bound111 = alloca i32, align 4
  store i64 0, i64* %i, align 8
  store i32 0, i32* @_ZL6offset, align 4
  store i64 3, i64* @bytes_out, align 8
  store i64 0, i64* @out_count, align 8
  store i32 0, i32* @clear_flg, align 4
  store i64 0, i64* @ratio, align 8
  store i64 1, i64* @in_count, align 8
  store i64 10000, i64* @checkpoint, align 8
  store i32 9, i32* @n_bits, align 4
  store i64 511, i64* @maxcode, align 8
  %tmp = load i32, i32* @block_compress, align 4
  %tobool = icmp ne i32 %tmp, 0
  %cond = select i1 %tobool, i32 257, i32 256
  %conv = sext i32 %cond to i64
  store i64 %conv, i64* @free_ent, align 8
  %call = call i32 @_Z7getbytev()
  %conv1 = zext i32 %call to i64
  store i64 %conv1, i64* %ent, align 8
  store i32 0, i32* %hshift, align 4
  %tmp2 = load i64, i64* @hsize, align 8
  store i64 %tmp2, i64* %fcode, align 8
  br label %codeRepl1

codeRepl1:                                        ; preds = %entry
  call void @_Z8compressv_for.cond(i64* %fcode, i32* %hshift)
  br label %for.end

for.end:                                          ; preds = %codeRepl1
  %tmp3 = load i32, i32* %hshift, align 4
  %sub = sub nsw i32 8, %tmp3
  store i32 %sub, i32* %hshift, align 4
  %tmp4 = load i64, i64* @hsize, align 8
  store i64 %tmp4, i64* %hsize_reg, align 8
  %tmp5 = load i64, i64* %hsize_reg, align 8
  call void @_Z7cl_hashl(i64 %tmp5)
  br label %codeRepl

codeRepl:                                         ; preds = %for.end
  call void @_Z8compressv_while.cond(i32* %apsim_bound111, i32* %c, i64* %ent, i64* %fcode, i32* %hshift, i64* %i, i64* %hsize_reg, i32* %disp)
  br label %while.end

while.end:                                        ; preds = %codeRepl
  %tmp6 = load i64, i64* @bytes_out, align 8
  %tmp7 = load i64, i64* @in_count, align 8
  %cmp54 = icmp sgt i64 %tmp6, %tmp7
  br i1 %cmp54, label %if.then55, label %while.end.if.end56_crit_edge

while.end.if.end56_crit_edge:                     ; preds = %while.end
  br label %if.end56

if.then55:                                        ; preds = %while.end
  store i32 2, i32* @exit_stat, align 4
  br label %if.end56

if.end56:                                         ; preds = %while.end.if.end56_crit_edge, %if.then55
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @_Z7getbytev() #1 {
entry:
  %tmp = load i32, i32* @InCnt, align 4
  %cmp = icmp sgt i32 %tmp, 0
  br i1 %cmp, label %land.lhs.true, label %entry.if.else_crit_edge

entry.if.else_crit_edge:                          ; preds = %entry
  br label %if.else

land.lhs.true:                                    ; preds = %entry
  %tmp1 = load i32, i32* @apsim_InCnt, align 4
  %dec = add nsw i32 %tmp1, -1
  store i32 %dec, i32* @apsim_InCnt, align 4
  %cmp1 = icmp sgt i32 %tmp1, 0
  br i1 %cmp1, label %if.then, label %land.lhs.true.if.else_crit_edge

land.lhs.true.if.else_crit_edge:                  ; preds = %land.lhs.true
  br label %if.else

if.then:                                          ; preds = %land.lhs.true
  %tmp2 = load i32, i32* @InCnt, align 4
  %dec2 = add nsw i32 %tmp2, -1
  store i32 %dec2, i32* @InCnt, align 4
  %tmp3 = load i8*, i8** @InBuff, align 8
  %incdec.ptr = getelementptr inbounds i8, i8* %tmp3, i32 1
  store i8* %incdec.ptr, i8** @InBuff, align 8
  %tmp4 = load i8, i8* %tmp3, align 1
  %conv = zext i8 %tmp4 to i32
  br label %return

if.else:                                          ; preds = %land.lhs.true.if.else_crit_edge, %entry.if.else_crit_edge
  br label %return

return:                                           ; preds = %if.else, %if.then
  %retval.0 = phi i32 [ %conv, %if.then ], [ -1, %if.else ]
  ret i32 %retval.0
}

; Function Attrs: nounwind uwtable
define void @_Z7cl_hashl(i64 %hsize) #1 {
entry:
  %htab_p = alloca i64*, align 8
  %i = alloca i64, align 8
  %m1 = alloca i64, align 8
  %add.ptr = getelementptr inbounds i64, i64* getelementptr inbounds ([257 x i64], [257 x i64]* @htab, i32 0, i32 0), i64 %hsize
  store i64* %add.ptr, i64** %htab_p, align 8
  store i64 -1, i64* %m1, align 8
  %sub = sub nsw i64 %hsize, 16
  store i64 %sub, i64* %i, align 8
  br label %codeRepl1

codeRepl1:                                        ; preds = %entry
  call void @_Z7cl_hashl_do.body(i64* %m1, i64** %htab_p, i64* %i)
  br label %do.end

do.end:                                           ; preds = %codeRepl1
  %tmp3 = load i64, i64* %i, align 8
  %add = add nsw i64 %tmp3, 16
  store i64 %add, i64* %i, align 8
  br label %codeRepl

codeRepl:                                         ; preds = %do.end
  call void @_Z7cl_hashl_for.cond(i64* %i, i64* %m1, i64** %htab_p)
  br label %for.end

for.end:                                          ; preds = %codeRepl
  ret void
}

; Function Attrs: uwtable
define void @_Z8cl_blockv() #2 {
entry:
  %tmp = load i64, i64* @in_count, align 8
  %add = add nsw i64 %tmp, 10000
  store i64 %add, i64* @checkpoint, align 8
  %tmp1 = load i64, i64* @in_count, align 8
  %cmp = icmp sgt i64 %tmp1, 8388607
  br i1 %cmp, label %if.then, label %if.else3

if.then:                                          ; preds = %entry
  %tmp2 = load i64, i64* @bytes_out, align 8
  %shr = ashr i64 %tmp2, 8
  %cmp1 = icmp eq i64 %shr, 0
  br i1 %cmp1, label %if.then2, label %if.else

if.then2:                                         ; preds = %if.then
  br label %if.end

if.else:                                          ; preds = %if.then
  %tmp4 = load i64, i64* @in_count, align 8
  %div = sdiv i64 %tmp4, %shr
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then2
  %rat.0 = phi i64 [ 2147483647, %if.then2 ], [ %div, %if.else ]
  br label %if.end5

if.else3:                                         ; preds = %entry
  %tmp6 = load i64, i64* @in_count, align 8
  %shl = shl i64 %tmp6, 8
  %tmp7 = load i64, i64* @bytes_out, align 8
  %div4 = sdiv i64 %shl, %tmp7
  br label %if.end5

if.end5:                                          ; preds = %if.else3, %if.end
  %rat.1 = phi i64 [ %rat.0, %if.end ], [ %div4, %if.else3 ]
  %tmp9 = load i64, i64* @ratio, align 8
  %cmp6 = icmp sgt i64 %rat.1, %tmp9
  br i1 %cmp6, label %if.then7, label %if.else8

if.then7:                                         ; preds = %if.end5
  store i64 %rat.1, i64* @ratio, align 8
  br label %if.end9

if.else8:                                         ; preds = %if.end5
  store i64 0, i64* @ratio, align 8
  %tmp11 = load i64, i64* @hsize, align 8
  call void @_Z7cl_hashl(i64 %tmp11)
  store i64 257, i64* @free_ent, align 8
  store i32 1, i32* @clear_flg, align 4
  call void @_Z6outputl(i64 256)
  br label %if.end9

if.end9:                                          ; preds = %if.else8, %if.then7
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z6outputl(i64 %code) #1 {
entry:
  %bits = alloca i32, align 4
  %bp = alloca i8*, align 8
  %tmp = load i32, i32* @_ZL6offset, align 4
  %tmp1 = load i32, i32* @n_bits, align 4
  store i32 %tmp1, i32* %bits, align 4
  store i8* getelementptr inbounds ([16 x i8], [16 x i8]* @buf, i32 0, i32 0), i8** %bp, align 8
  %cmp = icmp sge i64 %code, 0
  br i1 %cmp, label %if.then, label %if.else50

if.then:                                          ; preds = %entry
  %shr = ashr i32 %tmp, 3
  %tmp4 = load i8*, i8** %bp, align 8
  %idx.ext = sext i32 %shr to i64
  %add.ptr = getelementptr inbounds i8, i8* %tmp4, i64 %idx.ext
  store i8* %add.ptr, i8** %bp, align 8
  %and = and i32 %tmp, 7
  %tmp6 = load i8*, i8** %bp, align 8
  %tmp7 = load i8, i8* %tmp6, align 1
  %conv = sext i8 %tmp7 to i32
  %idxprom = sext i32 %and to i64
  %arrayidx = getelementptr inbounds [9 x i8], [9 x i8]* @rmask, i64 0, i64 %idxprom
  %tmp9 = load i8, i8* %arrayidx, align 1
  %conv1 = zext i8 %tmp9 to i32
  %and2 = and i32 %conv, %conv1
  %conv3 = sext i32 %and2 to i64
  %sh_prom = zext i32 %and to i64
  %shl = shl i64 %code, %sh_prom
  %or = or i64 %conv3, %shl
  %idxprom4 = sext i32 %and to i64
  %arrayidx5 = getelementptr inbounds [9 x i8], [9 x i8]* @lmask, i64 0, i64 %idxprom4
  %tmp13 = load i8, i8* %arrayidx5, align 1
  %conv6 = zext i8 %tmp13 to i64
  %and7 = and i64 %or, %conv6
  %conv8 = trunc i64 %and7 to i8
  %tmp14 = load i8*, i8** %bp, align 8
  store i8 %conv8, i8* %tmp14, align 1
  %tmp15 = load i8*, i8** %bp, align 8
  %incdec.ptr = getelementptr inbounds i8, i8* %tmp15, i32 1
  store i8* %incdec.ptr, i8** %bp, align 8
  %sub = sub nsw i32 8, %and
  %tmp17 = load i32, i32* %bits, align 4
  %sub9 = sub nsw i32 %tmp17, %sub
  store i32 %sub9, i32* %bits, align 4
  %sub10 = sub nsw i32 8, %and
  %sh_prom11 = zext i32 %sub10 to i64
  %shr12 = ashr i64 %code, %sh_prom11
  %tmp20 = load i32, i32* %bits, align 4
  %cmp13 = icmp sge i32 %tmp20, 8
  br i1 %cmp13, label %if.then14, label %if.then.if.end_crit_edge

if.then.if.end_crit_edge:                         ; preds = %if.then
  br label %if.end

if.then14:                                        ; preds = %if.then
  %conv15 = trunc i64 %shr12 to i8
  %tmp22 = load i8*, i8** %bp, align 8
  %incdec.ptr16 = getelementptr inbounds i8, i8* %tmp22, i32 1
  store i8* %incdec.ptr16, i8** %bp, align 8
  store i8 %conv15, i8* %tmp22, align 1
  %shr17 = ashr i64 %shr12, 8
  %tmp24 = load i32, i32* %bits, align 4
  %sub18 = sub nsw i32 %tmp24, 8
  store i32 %sub18, i32* %bits, align 4
  br label %if.end

if.end:                                           ; preds = %if.then.if.end_crit_edge, %if.then14
  %code.addr.0 = phi i64 [ %shr17, %if.then14 ], [ %shr12, %if.then.if.end_crit_edge ]
  %tmp25 = load i32, i32* %bits, align 4
  %tobool = icmp ne i32 %tmp25, 0
  br i1 %tobool, label %if.then19, label %if.end.if.end21_crit_edge

if.end.if.end21_crit_edge:                        ; preds = %if.end
  br label %if.end21

if.then19:                                        ; preds = %if.end
  %conv20 = trunc i64 %code.addr.0 to i8
  %tmp27 = load i8*, i8** %bp, align 8
  store i8 %conv20, i8* %tmp27, align 1
  br label %if.end21

if.end21:                                         ; preds = %if.end.if.end21_crit_edge, %if.then19
  %tmp28 = load i32, i32* @n_bits, align 4
  %tmp29 = load i32, i32* @_ZL6offset, align 4
  %add = add nsw i32 %tmp29, %tmp28
  store i32 %add, i32* @_ZL6offset, align 4
  %tmp30 = load i32, i32* @_ZL6offset, align 4
  %tmp31 = load i32, i32* @n_bits, align 4
  %shl22 = shl i32 %tmp31, 3
  %cmp23 = icmp eq i32 %tmp30, %shl22
  br i1 %cmp23, label %if.then24, label %if.end21.if.end30_crit_edge

if.end21.if.end30_crit_edge:                      ; preds = %if.end21
  br label %if.end30

if.then24:                                        ; preds = %if.end21
  store i8* getelementptr inbounds ([16 x i8], [16 x i8]* @buf, i32 0, i32 0), i8** %bp, align 8
  %tmp32 = load i32, i32* @n_bits, align 4
  store i32 %tmp32, i32* %bits, align 4
  %tmp33 = load i32, i32* %bits, align 4
  %conv25 = sext i32 %tmp33 to i64
  %tmp34 = load i64, i64* @bytes_out, align 8
  %add26 = add nsw i64 %tmp34, %conv25
  store i64 %add26, i64* @bytes_out, align 8
  br label %codeRepl

codeRepl:                                         ; preds = %if.then24
  call void @_Z6outputl_do.body(i8** %bp, i32* %bits)
  br label %do.end

do.end:                                           ; preds = %codeRepl
  store i32 0, i32* @_ZL6offset, align 4
  br label %if.end30

if.end30:                                         ; preds = %if.end21.if.end30_crit_edge, %do.end
  %tmp35 = load i64, i64* @free_ent, align 8
  %tmp36 = load i64, i64* @maxcode, align 8
  %cmp31 = icmp sgt i64 %tmp35, %tmp36
  br i1 %cmp31, label %if.end30.if.then33_crit_edge, label %lor.lhs.false

if.end30.if.then33_crit_edge:                     ; preds = %if.end30
  br label %if.then33

lor.lhs.false:                                    ; preds = %if.end30
  %tmp37 = load i32, i32* @clear_flg, align 4
  %cmp32 = icmp sgt i32 %tmp37, 0
  br i1 %cmp32, label %lor.lhs.false.if.then33_crit_edge, label %lor.lhs.false.if.end49_crit_edge

lor.lhs.false.if.end49_crit_edge:                 ; preds = %lor.lhs.false
  br label %if.end49

lor.lhs.false.if.then33_crit_edge:                ; preds = %lor.lhs.false
  br label %if.then33

if.then33:                                        ; preds = %lor.lhs.false.if.then33_crit_edge, %if.end30.if.then33_crit_edge
  %tmp38 = load i32, i32* @_ZL6offset, align 4
  %cmp34 = icmp sgt i32 %tmp38, 0
  br i1 %cmp34, label %if.then35, label %if.then33.if.end38_crit_edge

if.then33.if.end38_crit_edge:                     ; preds = %if.then33
  br label %if.end38

if.then35:                                        ; preds = %if.then33
  %tmp39 = load i32, i32* @n_bits, align 4
  call void @_Z10writebytesPci(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @buf, i32 0, i32 0), i32 %tmp39)
  %tmp40 = load i32, i32* @n_bits, align 4
  %conv36 = sext i32 %tmp40 to i64
  %tmp41 = load i64, i64* @bytes_out, align 8
  %add37 = add nsw i64 %tmp41, %conv36
  store i64 %add37, i64* @bytes_out, align 8
  br label %if.end38

if.end38:                                         ; preds = %if.then33.if.end38_crit_edge, %if.then35
  store i32 0, i32* @_ZL6offset, align 4
  %tmp42 = load i32, i32* @clear_flg, align 4
  %tobool39 = icmp ne i32 %tmp42, 0
  br i1 %tobool39, label %if.then40, label %if.else

if.then40:                                        ; preds = %if.end38
  store i32 9, i32* @n_bits, align 4
  store i64 511, i64* @maxcode, align 8
  store i32 0, i32* @clear_flg, align 4
  br label %if.end48

if.else:                                          ; preds = %if.end38
  %tmp43 = load i32, i32* @n_bits, align 4
  %inc = add nsw i32 %tmp43, 1
  store i32 %inc, i32* @n_bits, align 4
  %tmp44 = load i32, i32* @n_bits, align 4
  %tmp45 = load i32, i32* @maxbits, align 4
  %cmp41 = icmp eq i32 %tmp44, %tmp45
  br i1 %cmp41, label %if.then42, label %if.else43

if.then42:                                        ; preds = %if.else
  %tmp46 = load i64, i64* @maxmaxcode, align 8
  store i64 %tmp46, i64* @maxcode, align 8
  br label %if.end47

if.else43:                                        ; preds = %if.else
  %tmp47 = load i32, i32* @n_bits, align 4
  %shl44 = shl i32 1, %tmp47
  %sub45 = sub nsw i32 %shl44, 1
  %conv46 = sext i32 %sub45 to i64
  store i64 %conv46, i64* @maxcode, align 8
  br label %if.end47

if.end47:                                         ; preds = %if.else43, %if.then42
  br label %if.end48

if.end48:                                         ; preds = %if.end47, %if.then40
  br label %if.end49

if.end49:                                         ; preds = %lor.lhs.false.if.end49_crit_edge, %if.end48
  br label %if.end59

if.else50:                                        ; preds = %entry
  %tmp48 = load i32, i32* @_ZL6offset, align 4
  %cmp51 = icmp sgt i32 %tmp48, 0
  br i1 %cmp51, label %if.then52, label %if.else50.if.end54_crit_edge

if.else50.if.end54_crit_edge:                     ; preds = %if.else50
  br label %if.end54

if.then52:                                        ; preds = %if.else50
  %tmp49 = load i32, i32* @_ZL6offset, align 4
  %add53 = add nsw i32 %tmp49, 7
  %div = sdiv i32 %add53, 8
  call void @_Z10writebytesPci(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @buf, i32 0, i32 0), i32 %div)
  br label %if.end54

if.end54:                                         ; preds = %if.else50.if.end54_crit_edge, %if.then52
  %tmp50 = load i32, i32* @_ZL6offset, align 4
  %add55 = add nsw i32 %tmp50, 7
  %div56 = sdiv i32 %add55, 8
  %conv57 = sext i32 %div56 to i64
  %tmp51 = load i64, i64* @bytes_out, align 8
  %add58 = add nsw i64 %tmp51, %conv57
  store i64 %add58, i64* @bytes_out, align 8
  store i32 0, i32* @_ZL6offset, align 4
  br label %if.end59

if.end59:                                         ; preds = %if.end54, %if.end49
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z7putbytec(i8 signext %c) #1 {
entry:
  %tmp1 = load i8*, i8** @OutBuff, align 8
  %incdec.ptr = getelementptr inbounds i8, i8* %tmp1, i32 1
  store i8* %incdec.ptr, i8** @OutBuff, align 8
  store i8 %c, i8* %tmp1, align 1
  ret void
}

; Function Attrs: nounwind uwtable
define void @_Z10writebytesPci(i8* %buf, i32 %n) #1 {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @0, i32 0, i32 0))
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ], !is.monotonic !1
  %cmp = icmp slt i32 %i.0, %n
  br i1 %cmp, label %land.rhs, label %for.cond.land.end_crit_edge

for.cond.land.end_crit_edge:                      ; preds = %for.cond
  br label %land.end

land.rhs:                                         ; preds = %for.cond
  %cmp1 = icmp slt i32 %i.0, 16
  br label %land.end

land.end:                                         ; preds = %for.cond.land.end_crit_edge, %land.rhs
  %tmp3 = phi i1 [ false, %for.cond.land.end_crit_edge ], [ %cmp1, %land.rhs ]
  br i1 %tmp3, label %for.body, label %for.end

for.body:                                         ; preds = %land.end
  %1 = call i1 @__check_array_min(i32 %i.0, i32 0)
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds i8, i8* %buf, i64 %idxprom, !is.monotonic !1
  %tmp6 = load i8, i8* %arrayidx, align 1, !is.monotonic !1
  %tmp7 = load i8*, i8** @OutBuff, align 8
  %incdec.ptr = getelementptr inbounds i8, i8* %tmp7, i32 1
  store i8* %incdec.ptr, i8** @OutBuff, align 8
  store i8 %tmp6, i8* %tmp7, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %land.end
  ret void
}

define internal void @_Z8compressv_while.cond(i32* %apsim_bound111, i32* %c, i64* %ent, i64* %fcode, i32* %hshift, i64* %i, i64* %hsize_reg, i32* %disp) {
newFuncRoot:
  br label %while.cond

while.end.exitStub:                               ; preds = %while.cond
  ret void

while.cond:                                       ; preds = %newFuncRoot, %while.cond.backedge
  %tmp = load i32, i32* @InCnt, align 4
  %cmp2 = icmp sgt i32 %tmp, 0
  br i1 %cmp2, label %while.body, label %while.end.exitStub

while.body:                                       ; preds = %while.cond
  store i32 0, i32* %apsim_bound111, align 4
  %call3 = call i32 @_Z7getbytev()
  store i32 %call3, i32* %c, align 4
  %tmp1 = load i64, i64* @in_count, align 8
  %inc4 = add nsw i64 %tmp1, 1
  store i64 %inc4, i64* @in_count, align 8
  %tmp2 = load i32, i32* %c, align 4
  %conv5 = sext i32 %tmp2 to i64
  %tmp3 = load i32, i32* @maxbits, align 4
  %sh_prom = zext i32 %tmp3 to i64
  %shl = shl i64 %conv5, %sh_prom
  %tmp4 = load i64, i64* %ent, align 8
  %add = add nsw i64 %shl, %tmp4
  store i64 %add, i64* %fcode, align 8
  %tmp5 = load i32, i32* %c, align 4
  %tmp6 = load i32, i32* %hshift, align 4
  %shl6 = shl i32 %tmp5, %tmp6
  %conv7 = sext i32 %shl6 to i64
  %tmp7 = load i64, i64* %ent, align 8
  %xor = xor i64 %conv7, %tmp7
  store i64 %xor, i64* %i, align 8
  %tmp8 = load i64, i64* %i, align 8
  %arrayidx = getelementptr inbounds [257 x i64], [257 x i64]* @htab, i64 0, i64 %tmp8
  %tmp9 = load i64, i64* %arrayidx, align 8
  %tmp10 = load i64, i64* %fcode, align 8
  %cmp8 = icmp eq i64 %tmp9, %tmp10
  br i1 %cmp8, label %if.then, label %if.else

if.else:                                          ; preds = %while.body
  %tmp11 = load i64, i64* %i, align 8
  %arrayidx11 = getelementptr inbounds [257 x i64], [257 x i64]* @htab, i64 0, i64 %tmp11
  %tmp12 = load i64, i64* %arrayidx11, align 8
  %cmp12 = icmp slt i64 %tmp12, 0
  br i1 %cmp12, label %if.then13, label %if.end

if.end:                                           ; preds = %if.else
  br label %if.end14

if.end14:                                         ; preds = %if.end
  %tmp13 = load i64, i64* %hsize_reg, align 8
  %tmp14 = load i64, i64* %i, align 8
  %sub15 = sub nsw i64 %tmp13, %tmp14
  %conv16 = trunc i64 %sub15 to i32
  store i32 %conv16, i32* %disp, align 4
  %tmp15 = load i64, i64* %i, align 8
  %cmp17 = icmp eq i64 %tmp15, 0
  br i1 %cmp17, label %if.then18, label %if.end14.if.end19_crit_edge

if.end14.if.end19_crit_edge:                      ; preds = %if.end14
  br label %if.end19

if.then18:                                        ; preds = %if.end14
  store i32 1, i32* %disp, align 4
  br label %if.end19

if.end19:                                         ; preds = %if.end14.if.end19_crit_edge, %if.then18
  br label %probe

probe:                                            ; preds = %if.then37, %if.end19
  %tmp16 = load i32, i32* %disp, align 4
  %conv20 = sext i32 %tmp16 to i64
  %tmp17 = load i64, i64* %i, align 8
  %sub21 = sub nsw i64 %tmp17, %conv20
  store i64 %sub21, i64* %i, align 8
  %cmp22 = icmp slt i64 %sub21, 0
  br i1 %cmp22, label %if.then23, label %probe.if.end25_crit_edge

probe.if.end25_crit_edge:                         ; preds = %probe
  br label %if.end25

if.then23:                                        ; preds = %probe
  %tmp18 = load i64, i64* %hsize_reg, align 8
  %tmp19 = load i64, i64* %i, align 8
  %add24 = add nsw i64 %tmp19, %tmp18
  store i64 %add24, i64* %i, align 8
  br label %if.end25

if.end25:                                         ; preds = %probe.if.end25_crit_edge, %if.then23
  %tmp20 = load i64, i64* %i, align 8
  %arrayidx26 = getelementptr inbounds [257 x i64], [257 x i64]* @htab, i64 0, i64 %tmp20
  %tmp21 = load i64, i64* %arrayidx26, align 8
  %tmp22 = load i64, i64* %fcode, align 8
  %cmp27 = icmp eq i64 %tmp21, %tmp22
  br i1 %cmp27, label %if.then28, label %if.end31

if.end31:                                         ; preds = %if.end25
  %tmp23 = load i64, i64* %i, align 8
  %arrayidx32 = getelementptr inbounds [257 x i64], [257 x i64]* @htab, i64 0, i64 %tmp23
  %tmp24 = load i64, i64* %arrayidx32, align 8
  %cmp33 = icmp sgt i64 %tmp24, 0
  br i1 %cmp33, label %land.lhs.true, label %if.end31.if.end38_crit_edge

if.end31.if.end38_crit_edge:                      ; preds = %if.end31
  br label %if.end38

land.lhs.true:                                    ; preds = %if.end31
  %tmp25 = load i32, i32* %apsim_bound111, align 4
  %inc34 = add nsw i32 %tmp25, 1
  store i32 %inc34, i32* %apsim_bound111, align 4
  %conv35 = sext i32 %inc34 to i64
  %tmp26 = load i64, i64* @in_count, align 8
  %cmp36 = icmp slt i64 %conv35, %tmp26
  br i1 %cmp36, label %if.then37, label %land.lhs.true.if.end38_crit_edge

land.lhs.true.if.end38_crit_edge:                 ; preds = %land.lhs.true
  br label %if.end38

if.end38:                                         ; preds = %land.lhs.true.if.end38_crit_edge, %if.end31.if.end38_crit_edge
  br label %nomatch

if.then37:                                        ; preds = %land.lhs.true
  br label %probe

if.then28:                                        ; preds = %if.end25
  %tmp27 = load i64, i64* %i, align 8
  %arrayidx29 = getelementptr inbounds [257 x i16], [257 x i16]* @codetab, i64 0, i64 %tmp27
  %tmp28 = load i16, i16* %arrayidx29, align 2
  %conv30 = zext i16 %tmp28 to i64
  store i64 %conv30, i64* %ent, align 8
  br label %while.cond.backedge

if.then13:                                        ; preds = %if.else
  br label %nomatch

nomatch:                                          ; preds = %if.end38, %if.then13
  %tmp29 = load i64, i64* @out_count, align 8
  %inc39 = add nsw i64 %tmp29, 1
  store i64 %inc39, i64* @out_count, align 8
  %tmp30 = load i32, i32* %c, align 4
  %conv40 = sext i32 %tmp30 to i64
  store i64 %conv40, i64* %ent, align 8
  %tmp31 = load i64, i64* @free_ent, align 8
  %tmp32 = load i64, i64* @maxmaxcode, align 8
  %cmp41 = icmp slt i64 %tmp31, %tmp32
  br i1 %cmp41, label %if.then42, label %if.else47

if.else47:                                        ; preds = %nomatch
  %tmp33 = load i64, i64* @in_count, align 8
  %tmp34 = load i64, i64* @checkpoint, align 8
  %cmp48 = icmp sge i64 %tmp33, %tmp34
  br i1 %cmp48, label %land.lhs.true49, label %if.else47.if.end52_crit_edge

if.else47.if.end52_crit_edge:                     ; preds = %if.else47
  br label %if.end52

land.lhs.true49:                                  ; preds = %if.else47
  %tmp35 = load i32, i32* @block_compress, align 4
  %tobool50 = icmp ne i32 %tmp35, 0
  br i1 %tobool50, label %if.then51, label %land.lhs.true49.if.end52_crit_edge

land.lhs.true49.if.end52_crit_edge:               ; preds = %land.lhs.true49
  br label %if.end52

if.then51:                                        ; preds = %land.lhs.true49
  call void @_Z8cl_blockv()
  br label %if.end52

if.end52:                                         ; preds = %land.lhs.true49.if.end52_crit_edge, %if.else47.if.end52_crit_edge, %if.then51
  br label %if.end53

if.then42:                                        ; preds = %nomatch
  %tmp36 = load i64, i64* @free_ent, align 8
  %inc43 = add nsw i64 %tmp36, 1
  store i64 %inc43, i64* @free_ent, align 8
  %conv44 = trunc i64 %tmp36 to i16
  %tmp37 = load i64, i64* %i, align 8
  %arrayidx45 = getelementptr inbounds [257 x i16], [257 x i16]* @codetab, i64 0, i64 %tmp37
  store i16 %conv44, i16* %arrayidx45, align 2
  %tmp38 = load i64, i64* %fcode, align 8
  %tmp39 = load i64, i64* %i, align 8
  %arrayidx46 = getelementptr inbounds [257 x i64], [257 x i64]* @htab, i64 0, i64 %tmp39
  store i64 %tmp38, i64* %arrayidx46, align 8
  br label %if.end53

if.end53:                                         ; preds = %if.end52, %if.then42
  br label %while.cond.backedge

if.then:                                          ; preds = %while.body
  %tmp40 = load i64, i64* %i, align 8
  %arrayidx9 = getelementptr inbounds [257 x i16], [257 x i16]* @codetab, i64 0, i64 %tmp40
  %tmp41 = load i16, i16* %arrayidx9, align 2
  %conv10 = zext i16 %tmp41 to i64
  store i64 %conv10, i64* %ent, align 8
  br label %while.cond.backedge

while.cond.backedge:                              ; preds = %if.then, %if.then28, %if.end53
  br label %while.cond
}

define internal void @_Z8compressv_for.cond(i64* %fcode, i32* %hshift) {
newFuncRoot:
  br label %for.cond

for.end.exitStub:                                 ; preds = %for.cond
  ret void

for.cond:                                         ; preds = %newFuncRoot, %for.inc
  %tmp = load i64, i64* %fcode, align 8
  %cmp = icmp slt i64 %tmp, 65536
  br i1 %cmp, label %for.body, label %for.end.exitStub

for.body:                                         ; preds = %for.cond
  %tmp1 = load i32, i32* %hshift, align 4
  %inc = add nsw i32 %tmp1, 1
  store i32 %inc, i32* %hshift, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp2 = load i64, i64* %fcode, align 8
  %mul = mul nsw i64 %tmp2, 2
  store i64 %mul, i64* %fcode, align 8
  br label %for.cond
}

; Function Attrs: nounwind
define internal void @_Z7cl_hashl_for.cond(i64* %i, i64* %m1, i64** %htab_p) #3 {
newFuncRoot:
  br label %for.cond

for.end.exitStub:                                 ; preds = %for.cond
  ret void

for.cond:                                         ; preds = %newFuncRoot, %for.inc
  %tmp = load i64, i64* %i, align 8
  %cmp19 = icmp sgt i64 %tmp, 0
  br i1 %cmp19, label %for.body, label %for.end.exitStub

for.body:                                         ; preds = %for.cond
  %tmp1 = load i64, i64* %m1, align 8
  %tmp2 = load i64*, i64** %htab_p, align 8
  %incdec.ptr = getelementptr inbounds i64, i64* %tmp2, i32 -1
  store i64* %incdec.ptr, i64** %htab_p, align 8
  store i64 %tmp1, i64* %incdec.ptr, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp3 = load i64, i64* %i, align 8
  %dec = add nsw i64 %tmp3, -1
  store i64 %dec, i64* %i, align 8
  br label %for.cond
}

; Function Attrs: nounwind
define internal void @_Z7cl_hashl_do.body(i64* %m1, i64** %htab_p, i64* %i) #3 {
newFuncRoot:
  br label %do.body

do.end.exitStub:                                  ; preds = %do.cond
  ret void

do.body:                                          ; preds = %newFuncRoot, %do.cond.do.body_crit_edge
  %tmp = load i64, i64* %m1, align 8
  %tmp1 = load i64*, i64** %htab_p, align 8
  %add.ptr1 = getelementptr inbounds i64, i64* %tmp1, i64 -16
  store i64 %tmp, i64* %add.ptr1, align 8
  %tmp2 = load i64, i64* %m1, align 8
  %tmp3 = load i64*, i64** %htab_p, align 8
  %add.ptr2 = getelementptr inbounds i64, i64* %tmp3, i64 -15
  store i64 %tmp2, i64* %add.ptr2, align 8
  %tmp4 = load i64, i64* %m1, align 8
  %tmp5 = load i64*, i64** %htab_p, align 8
  %add.ptr3 = getelementptr inbounds i64, i64* %tmp5, i64 -14
  store i64 %tmp4, i64* %add.ptr3, align 8
  %tmp6 = load i64, i64* %m1, align 8
  %tmp7 = load i64*, i64** %htab_p, align 8
  %add.ptr4 = getelementptr inbounds i64, i64* %tmp7, i64 -13
  store i64 %tmp6, i64* %add.ptr4, align 8
  %tmp8 = load i64, i64* %m1, align 8
  %tmp9 = load i64*, i64** %htab_p, align 8
  %add.ptr5 = getelementptr inbounds i64, i64* %tmp9, i64 -12
  store i64 %tmp8, i64* %add.ptr5, align 8
  %tmp10 = load i64, i64* %m1, align 8
  %tmp11 = load i64*, i64** %htab_p, align 8
  %add.ptr6 = getelementptr inbounds i64, i64* %tmp11, i64 -11
  store i64 %tmp10, i64* %add.ptr6, align 8
  %tmp12 = load i64, i64* %m1, align 8
  %tmp13 = load i64*, i64** %htab_p, align 8
  %add.ptr7 = getelementptr inbounds i64, i64* %tmp13, i64 -10
  store i64 %tmp12, i64* %add.ptr7, align 8
  %tmp14 = load i64, i64* %m1, align 8
  %tmp15 = load i64*, i64** %htab_p, align 8
  %add.ptr8 = getelementptr inbounds i64, i64* %tmp15, i64 -9
  store i64 %tmp14, i64* %add.ptr8, align 8
  %tmp16 = load i64, i64* %m1, align 8
  %tmp17 = load i64*, i64** %htab_p, align 8
  %add.ptr9 = getelementptr inbounds i64, i64* %tmp17, i64 -8
  store i64 %tmp16, i64* %add.ptr9, align 8
  %tmp18 = load i64, i64* %m1, align 8
  %tmp19 = load i64*, i64** %htab_p, align 8
  %add.ptr10 = getelementptr inbounds i64, i64* %tmp19, i64 -7
  store i64 %tmp18, i64* %add.ptr10, align 8
  %tmp20 = load i64, i64* %m1, align 8
  %tmp21 = load i64*, i64** %htab_p, align 8
  %add.ptr11 = getelementptr inbounds i64, i64* %tmp21, i64 -6
  store i64 %tmp20, i64* %add.ptr11, align 8
  %tmp22 = load i64, i64* %m1, align 8
  %tmp23 = load i64*, i64** %htab_p, align 8
  %add.ptr12 = getelementptr inbounds i64, i64* %tmp23, i64 -5
  store i64 %tmp22, i64* %add.ptr12, align 8
  %tmp24 = load i64, i64* %m1, align 8
  %tmp25 = load i64*, i64** %htab_p, align 8
  %add.ptr13 = getelementptr inbounds i64, i64* %tmp25, i64 -4
  store i64 %tmp24, i64* %add.ptr13, align 8
  %tmp26 = load i64, i64* %m1, align 8
  %tmp27 = load i64*, i64** %htab_p, align 8
  %add.ptr14 = getelementptr inbounds i64, i64* %tmp27, i64 -3
  store i64 %tmp26, i64* %add.ptr14, align 8
  %tmp28 = load i64, i64* %m1, align 8
  %tmp29 = load i64*, i64** %htab_p, align 8
  %add.ptr15 = getelementptr inbounds i64, i64* %tmp29, i64 -2
  store i64 %tmp28, i64* %add.ptr15, align 8
  %tmp30 = load i64, i64* %m1, align 8
  %tmp31 = load i64*, i64** %htab_p, align 8
  %add.ptr16 = getelementptr inbounds i64, i64* %tmp31, i64 -1
  store i64 %tmp30, i64* %add.ptr16, align 8
  %tmp32 = load i64*, i64** %htab_p, align 8
  %add.ptr17 = getelementptr inbounds i64, i64* %tmp32, i64 -16
  store i64* %add.ptr17, i64** %htab_p, align 8
  br label %do.cond

do.cond:                                          ; preds = %do.body
  %tmp33 = load i64, i64* %i, align 8
  %sub18 = sub nsw i64 %tmp33, 16
  store i64 %sub18, i64* %i, align 8
  %cmp = icmp sge i64 %sub18, 0
  br i1 %cmp, label %do.cond.do.body_crit_edge, label %do.end.exitStub

do.cond.do.body_crit_edge:                        ; preds = %do.cond
  br label %do.body
}

; Function Attrs: nounwind
define internal void @_Z6outputl_do.body(i8** %bp, i32* %bits) #3 {
newFuncRoot:
  br label %do.body

do.end.exitStub:                                  ; preds = %land.end
  ret void

do.body:                                          ; preds = %newFuncRoot, %land.end.do.body_crit_edge
  %tmp = load i8*, i8** %bp, align 8
  %incdec.ptr27 = getelementptr inbounds i8, i8* %tmp, i32 1
  store i8* %incdec.ptr27, i8** %bp, align 8
  %tmp1 = load i8, i8* %tmp, align 1
  call void @_Z7putbytec(i8 signext %tmp1)
  br label %do.cond

do.cond:                                          ; preds = %do.body
  %tmp2 = load i32, i32* %bits, align 4
  %dec = add nsw i32 %tmp2, -1
  store i32 %dec, i32* %bits, align 4
  %tobool28 = icmp ne i32 %dec, 0
  br i1 %tobool28, label %land.rhs, label %do.cond.land.end_crit_edge

do.cond.land.end_crit_edge:                       ; preds = %do.cond
  br label %land.end

land.rhs:                                         ; preds = %do.cond
  %tmp3 = load i8*, i8** %bp, align 8
  %sub.ptr.lhs.cast = ptrtoint i8* %tmp3 to i64
  %sub.ptr.sub = sub i64 %sub.ptr.lhs.cast, ptrtoint ([16 x i8]* @buf to i64)
  %cmp29 = icmp slt i64 %sub.ptr.sub, 16
  br label %land.end

land.end:                                         ; preds = %do.cond.land.end_crit_edge, %land.rhs
  %tmp4 = phi i1 [ false, %do.cond.land.end_crit_edge ], [ %cmp29, %land.rhs ]
  br i1 %tmp4, label %land.end.do.body_crit_edge, label %do.end.exitStub

land.end.do.body_crit_edge:                       ; preds = %land.end
  br label %do.body
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

attributes #0 = { norecurse uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
!1 = !{!"monotonicity"}
