; ModuleID = 'filter.opt.o'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

@LPbuff = common global [10 x i32] zeroinitializer, align 4
@HPbuff = common global [25 x i32] zeroinitializer, align 4
@DERIVbuff = common global [2 x i32] zeroinitializer, align 4
@LPy1 = common global i32 0, align 4
@LPy2 = common global i32 0, align 4
@HPy1 = common global i32 0, align 4
@WINsum = common global i32 0, align 4
@LPbuff_ptr = common global i32 0, align 4
@HPbuff_ptr = common global i32 0, align 4
@DERIVbuff_ptr = common global i32 0, align 4
@WINbuff_ptr = common global i32 0, align 4
@.str = private constant [16 x i8] c"filt_input_pipe\00"
@.str1 = private constant [11 x i8] c"LPout_pipe\00"
@.str2 = private constant [11 x i8] c"HPout_pipe\00"
@.str3 = private constant [14 x i8] c"DERIVout_pipe\00"
@WINbuff = common global [16 x i32] zeroinitializer, align 4
@.str4 = private constant [17 x i8] c"filt_output_pipe\00"

define i64 @divideUnsigned(i64 %dividend, i32 %divisor) nounwind readnone {
; <label>:0
  %1 = icmp eq i32 %divisor, 0
  br i1 %1, label %.loopexit, label %.preheader

.preheader:                                       ; preds = %0
  %2 = zext i32 %divisor to i64
  %3 = icmp ugt i64 %2, %dividend
  br i1 %3, label %.loopexit, label %bb.nph6.preheader

bb.nph6.preheader:                                ; preds = %.preheader
  br label %bb.nph6

bb.nph6:                                          ; preds = %._crit_edge, %bb.nph6.preheader
  %.05 = phi i64 [ %10, %._crit_edge ], [ %dividend, %bb.nph6.preheader ]
  %quotient.04 = phi i64 [ %9, %._crit_edge ], [ 0, %bb.nph6.preheader ]
  %4 = lshr i64 %.05, 1
  %5 = icmp ult i64 %2, %4
  br i1 %5, label %bb.nph.preheader, label %._crit_edge

bb.nph.preheader:                                 ; preds = %bb.nph6
  br label %bb.nph

bb.nph:                                           ; preds = %bb.nph, %bb.nph.preheader
  %shifted_divisor.02 = phi i64 [ %6, %bb.nph ], [ %2, %bb.nph.preheader ]
  %curr_quotient.01 = phi i64 [ %7, %bb.nph ], [ 1, %bb.nph.preheader ]
  %6 = shl i64 %shifted_divisor.02, 1
  %7 = shl i64 %curr_quotient.01, 1
  %8 = icmp ult i64 %6, %4
  br i1 %8, label %bb.nph, label %._crit_edge.loopexit

._crit_edge.loopexit:                             ; preds = %bb.nph
  %.lcssa1 = phi i64 [ %7, %bb.nph ]
  %.lcssa = phi i64 [ %6, %bb.nph ]
  br label %._crit_edge

._crit_edge:                                      ; preds = %._crit_edge.loopexit, %bb.nph6
  %shifted_divisor.0.lcssa = phi i64 [ %2, %bb.nph6 ], [ %.lcssa, %._crit_edge.loopexit ]
  %curr_quotient.0.lcssa = phi i64 [ 1, %bb.nph6 ], [ %.lcssa1, %._crit_edge.loopexit ]
  %9 = add i64 %curr_quotient.0.lcssa, %quotient.04
  %10 = sub i64 %.05, %shifted_divisor.0.lcssa
  %11 = icmp ult i64 %10, %2
  br i1 %11, label %.loopexit.loopexit, label %bb.nph6

.loopexit.loopexit:                               ; preds = %._crit_edge
  %.lcssa2 = phi i64 [ %9, %._crit_edge ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %.preheader, %0
  %quotient.1 = phi i64 [ 0, %0 ], [ 0, %.preheader ], [ %.lcssa2, %.loopexit.loopexit ]
  ret i64 %quotient.1
}

define i64 @divideSigned(i64 %dividend, i32 %divisor) nounwind readnone {
  %1 = icmp slt i64 %dividend, 0
  %2 = sub nsw i64 0, %dividend
  %3 = select i1 %1, i64 %2, i64 %dividend
  %4 = icmp slt i64 %3, 0
  %5 = icmp slt i32 %divisor, 0
  %6 = sub nsw i32 0, %divisor
  %7 = select i1 %5, i32 %6, i32 %divisor
  %8 = select i1 %4, i32 -1, i32 1
  %9 = icmp eq i32 %7, 0
  br i1 %9, label %26, label %.preheader.i

.preheader.i:                                     ; preds = %0
  %10 = sub nsw i32 0, %8
  %11 = icmp slt i32 %7, 0
  %12 = select i1 %11, i32 %10, i32 %8
  %13 = zext i32 %7 to i64
  %14 = icmp ugt i64 %13, %3
  br i1 %14, label %divideUnsigned.exit, label %bb.nph6.i.preheader

bb.nph6.i.preheader:                              ; preds = %.preheader.i
  br label %bb.nph6.i

bb.nph6.i:                                        ; preds = %._crit_edge.i, %bb.nph6.i.preheader
  %.05.i = phi i64 [ %21, %._crit_edge.i ], [ %3, %bb.nph6.i.preheader ]
  %quotient.04.i = phi i64 [ %20, %._crit_edge.i ], [ 0, %bb.nph6.i.preheader ]
  %15 = lshr i64 %.05.i, 1
  %16 = icmp ult i64 %13, %15
  br i1 %16, label %bb.nph.i.preheader, label %._crit_edge.i

bb.nph.i.preheader:                               ; preds = %bb.nph6.i
  br label %bb.nph.i

bb.nph.i:                                         ; preds = %bb.nph.i, %bb.nph.i.preheader
  %shifted_divisor.02.i = phi i64 [ %17, %bb.nph.i ], [ %13, %bb.nph.i.preheader ]
  %curr_quotient.01.i = phi i64 [ %18, %bb.nph.i ], [ 1, %bb.nph.i.preheader ]
  %17 = shl i64 %shifted_divisor.02.i, 1
  %18 = shl i64 %curr_quotient.01.i, 1
  %19 = icmp ult i64 %17, %15
  br i1 %19, label %bb.nph.i, label %._crit_edge.i.loopexit

._crit_edge.i.loopexit:                           ; preds = %bb.nph.i
  %.lcssa1 = phi i64 [ %18, %bb.nph.i ]
  %.lcssa = phi i64 [ %17, %bb.nph.i ]
  br label %._crit_edge.i

._crit_edge.i:                                    ; preds = %._crit_edge.i.loopexit, %bb.nph6.i
  %shifted_divisor.0.lcssa.i = phi i64 [ %13, %bb.nph6.i ], [ %.lcssa, %._crit_edge.i.loopexit ]
  %curr_quotient.0.lcssa.i = phi i64 [ 1, %bb.nph6.i ], [ %.lcssa1, %._crit_edge.i.loopexit ]
  %20 = add i64 %curr_quotient.0.lcssa.i, %quotient.04.i
  %21 = sub i64 %.05.i, %shifted_divisor.0.lcssa.i
  %22 = icmp ult i64 %21, %13
  br i1 %22, label %divideUnsigned.exit.loopexit, label %bb.nph6.i

divideUnsigned.exit.loopexit:                     ; preds = %._crit_edge.i
  %.lcssa2 = phi i64 [ %20, %._crit_edge.i ]
  br label %divideUnsigned.exit

divideUnsigned.exit:                              ; preds = %divideUnsigned.exit.loopexit, %.preheader.i
  %quotient.1.i = phi i64 [ 0, %.preheader.i ], [ %.lcssa2, %divideUnsigned.exit.loopexit ]
  %23 = icmp slt i32 %12, 0
  %24 = sub nsw i64 0, %quotient.1.i
  %25 = select i1 %23, i64 %24, i64 %quotient.1.i
  ret i64 %25

; <label>:26                                      ; preds = %0
  ret i64 0
}

define void @initFilt() nounwind {
bb.nph6:
  br label %0

; <label>:0                                       ; preds = %0, %bb.nph6
  %index.05 = phi i32 [ 0, %bb.nph6 ], [ %1, %0 ]
  %scevgep10 = getelementptr [10 x i32]* @LPbuff, i32 0, i32 %index.05
  tail call void @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep10, align 4
  %1 = add nsw i32 %index.05, 1
  %exitcond4 = icmp eq i32 %1, 10
  br i1 %exitcond4, label %bb.nph4.preheader, label %0

bb.nph4.preheader:                                ; preds = %0
  br label %bb.nph4

bb.nph4:                                          ; preds = %bb.nph4, %bb.nph4.preheader
  %index.13 = phi i32 [ %2, %bb.nph4 ], [ 0, %bb.nph4.preheader ]
  %scevgep8 = getelementptr [25 x i32]* @HPbuff, i32 0, i32 %index.13
  tail call void @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep8, align 4
  %2 = add nsw i32 %index.13, 1
  %exitcond = icmp eq i32 %2, 25
  br i1 %exitcond, label %bb.nph.preheader, label %bb.nph4

bb.nph.preheader:                                 ; preds = %bb.nph4
  br label %bb.nph

bb.nph:                                           ; preds = %bb.nph, %bb.nph.preheader
  %index.21 = phi i32 [ %3, %bb.nph ], [ 0, %bb.nph.preheader ]
  %scevgep = getelementptr [2 x i32]* @DERIVbuff, i32 0, i32 %index.21
  tail call void @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep, align 4
  %3 = add nsw i32 %index.21, 1
  %exitcond1 = icmp eq i32 %3, 2
  br i1 %exitcond1, label %._crit_edge, label %bb.nph

._crit_edge:                                      ; preds = %bb.nph
  store i32 0, i32* @LPy1, align 4
  store i32 0, i32* @LPy2, align 4
  store i32 0, i32* @HPy1, align 4
  store i32 0, i32* @WINsum, align 4
  store i32 0, i32* @LPbuff_ptr, align 4
  store i32 0, i32* @HPbuff_ptr, align 4
  store i32 0, i32* @DERIVbuff_ptr, align 4
  store i32 0, i32* @WINbuff_ptr, align 4
  ret void
}

declare void @__loop_pipelining_on__(i32, i32, i32)

define i32 @circUpdateFilt(i32 %ptr, i32 %size) nounwind readnone {
  %1 = add nsw i32 %ptr, 1
  %2 = icmp eq i32 %1, %size
  %3 = select i1 %2, i32 0, i32 %1
  ret i32 %3
}

define void @lpFilt() nounwind {
; <label>:0
  %1 = tail call i32 @read_uint32(i8* getelementptr inbounds ([16 x i8]* @.str, i32 0, i32 0)) nounwind
  %2 = load i32* @LPbuff_ptr, align 4
  %3 = add nsw i32 %2, -5
  %4 = icmp slt i32 %3, 0
  %5 = add nsw i32 %2, 5
  %6 = select i1 %4, i32 %5, i32 %3
  %7 = load i32* @LPy1, align 4
  %8 = shl i32 %7, 1
  %9 = load i32* @LPy2, align 4
  %10 = getelementptr inbounds [10 x i32]* @LPbuff, i32 0, i32 %6
  %11 = load i32* %10, align 4
  %12 = shl i32 %11, 1
  %13 = getelementptr inbounds [10 x i32]* @LPbuff, i32 0, i32 %2
  %14 = load i32* %13, align 4
  %15 = add i32 %8, %1
  %16 = sub i32 %15, %9
  %17 = add i32 %16, %14
  %18 = sub i32 %17, %12
  store i32 %7, i32* @LPy2, align 4
  store i32 %18, i32* @LPy1, align 4
  %19 = sext i32 %18 to i64
  %20 = icmp slt i32 %18, 0
  %21 = sub nsw i64 0, %19
  %22 = select i1 %20, i64 %21, i64 %19
  %23 = icmp slt i64 %22, 0
  %24 = icmp ult i64 %22, 25
  br i1 %24, label %divideSigned.exit, label %bb.nph6.i.i.preheader

bb.nph6.i.i.preheader:                            ; preds = %0
  br label %bb.nph6.i.i

bb.nph6.i.i:                                      ; preds = %._crit_edge.i.i, %bb.nph6.i.i.preheader
  %.05.i.i = phi i64 [ %31, %._crit_edge.i.i ], [ %22, %bb.nph6.i.i.preheader ]
  %quotient.04.i.i = phi i64 [ %30, %._crit_edge.i.i ], [ 0, %bb.nph6.i.i.preheader ]
  %25 = lshr i64 %.05.i.i, 1
  %26 = icmp ugt i64 %25, 25
  br i1 %26, label %bb.nph.i.i.preheader, label %._crit_edge.i.i

bb.nph.i.i.preheader:                             ; preds = %bb.nph6.i.i
  br label %bb.nph.i.i

bb.nph.i.i:                                       ; preds = %bb.nph.i.i, %bb.nph.i.i.preheader
  %shifted_divisor.02.i.i = phi i64 [ %27, %bb.nph.i.i ], [ 25, %bb.nph.i.i.preheader ]
  %curr_quotient.01.i.i = phi i64 [ %28, %bb.nph.i.i ], [ 1, %bb.nph.i.i.preheader ]
  %27 = shl i64 %shifted_divisor.02.i.i, 1
  %28 = shl i64 %curr_quotient.01.i.i, 1
  %29 = icmp ult i64 %27, %25
  br i1 %29, label %bb.nph.i.i, label %._crit_edge.i.i.loopexit

._crit_edge.i.i.loopexit:                         ; preds = %bb.nph.i.i
  %.lcssa1 = phi i64 [ %28, %bb.nph.i.i ]
  %.lcssa = phi i64 [ %27, %bb.nph.i.i ]
  br label %._crit_edge.i.i

._crit_edge.i.i:                                  ; preds = %._crit_edge.i.i.loopexit, %bb.nph6.i.i
  %shifted_divisor.0.lcssa.i.i = phi i64 [ 25, %bb.nph6.i.i ], [ %.lcssa, %._crit_edge.i.i.loopexit ]
  %curr_quotient.0.lcssa.i.i = phi i64 [ 1, %bb.nph6.i.i ], [ %.lcssa1, %._crit_edge.i.i.loopexit ]
  %30 = add i64 %curr_quotient.0.lcssa.i.i, %quotient.04.i.i
  %31 = sub i64 %.05.i.i, %shifted_divisor.0.lcssa.i.i
  %32 = icmp ult i64 %31, 25
  br i1 %32, label %divideSigned.exit.loopexit, label %bb.nph6.i.i

divideSigned.exit.loopexit:                       ; preds = %._crit_edge.i.i
  %.lcssa2 = phi i64 [ %30, %._crit_edge.i.i ]
  br label %divideSigned.exit

divideSigned.exit:                                ; preds = %divideSigned.exit.loopexit, %0
  %quotient.1.i.i = phi i64 [ 0, %0 ], [ %.lcssa2, %divideSigned.exit.loopexit ]
  %33 = sub nsw i64 0, %quotient.1.i.i
  %34 = select i1 %23, i64 %33, i64 %quotient.1.i.i
  %35 = trunc i64 %34 to i32
  store i32 %1, i32* %13, align 4
  %36 = add nsw i32 %2, 1
  %37 = icmp eq i32 %36, 10
  %38 = select i1 %37, i32 0, i32 %36
  store i32 %38, i32* @LPbuff_ptr, align 4
  tail call void @write_uint32(i8* getelementptr inbounds ([11 x i8]* @.str1, i32 0, i32 0), i32 %35) nounwind
  ret void
}

declare i32 @read_uint32(i8*)

declare void @write_uint32(i8*, i32)

define void @hpFilt() nounwind {
; <label>:0
  %1 = tail call i32 @read_uint32(i8* getelementptr inbounds ([11 x i8]* @.str1, i32 0, i32 0)) nounwind
  %2 = load i32* @HPbuff_ptr, align 4
  %3 = add nsw i32 %2, -12
  %4 = icmp slt i32 %3, 0
  %5 = add nsw i32 %2, 13
  %6 = select i1 %4, i32 %5, i32 %3
  %7 = load i32* @HPy1, align 4
  %8 = getelementptr inbounds [25 x i32]* @HPbuff, i32 0, i32 %2
  %9 = load i32* %8, align 4
  %10 = add nsw i32 %7, %1
  %11 = sub i32 %10, %9
  store i32 %11, i32* @HPy1, align 4
  %12 = getelementptr inbounds [25 x i32]* @HPbuff, i32 0, i32 %6
  %13 = load i32* %12, align 4
  %14 = zext i32 %13 to i64
  %15 = sext i32 %11 to i64
  %16 = icmp slt i32 %11, 0
  %17 = sub nsw i64 0, %15
  %18 = select i1 %16, i64 %17, i64 %15
  %19 = icmp slt i64 %18, 0
  %20 = icmp ult i64 %18, 25
  br i1 %20, label %divideSigned.exit, label %bb.nph6.i.i.preheader

bb.nph6.i.i.preheader:                            ; preds = %0
  br label %bb.nph6.i.i

bb.nph6.i.i:                                      ; preds = %._crit_edge.i.i, %bb.nph6.i.i.preheader
  %.05.i.i = phi i64 [ %27, %._crit_edge.i.i ], [ %18, %bb.nph6.i.i.preheader ]
  %quotient.04.i.i = phi i64 [ %26, %._crit_edge.i.i ], [ 0, %bb.nph6.i.i.preheader ]
  %21 = lshr i64 %.05.i.i, 1
  %22 = icmp ugt i64 %21, 25
  br i1 %22, label %bb.nph.i.i.preheader, label %._crit_edge.i.i

bb.nph.i.i.preheader:                             ; preds = %bb.nph6.i.i
  br label %bb.nph.i.i

bb.nph.i.i:                                       ; preds = %bb.nph.i.i, %bb.nph.i.i.preheader
  %shifted_divisor.02.i.i = phi i64 [ %23, %bb.nph.i.i ], [ 25, %bb.nph.i.i.preheader ]
  %curr_quotient.01.i.i = phi i64 [ %24, %bb.nph.i.i ], [ 1, %bb.nph.i.i.preheader ]
  %23 = shl i64 %shifted_divisor.02.i.i, 1
  %24 = shl i64 %curr_quotient.01.i.i, 1
  %25 = icmp ult i64 %23, %21
  br i1 %25, label %bb.nph.i.i, label %._crit_edge.i.i.loopexit

._crit_edge.i.i.loopexit:                         ; preds = %bb.nph.i.i
  %.lcssa1 = phi i64 [ %24, %bb.nph.i.i ]
  %.lcssa = phi i64 [ %23, %bb.nph.i.i ]
  br label %._crit_edge.i.i

._crit_edge.i.i:                                  ; preds = %._crit_edge.i.i.loopexit, %bb.nph6.i.i
  %shifted_divisor.0.lcssa.i.i = phi i64 [ 25, %bb.nph6.i.i ], [ %.lcssa, %._crit_edge.i.i.loopexit ]
  %curr_quotient.0.lcssa.i.i = phi i64 [ 1, %bb.nph6.i.i ], [ %.lcssa1, %._crit_edge.i.i.loopexit ]
  %26 = add i64 %curr_quotient.0.lcssa.i.i, %quotient.04.i.i
  %27 = sub i64 %.05.i.i, %shifted_divisor.0.lcssa.i.i
  %28 = icmp ult i64 %27, 25
  br i1 %28, label %divideSigned.exit.loopexit, label %bb.nph6.i.i

divideSigned.exit.loopexit:                       ; preds = %._crit_edge.i.i
  %.lcssa2 = phi i64 [ %26, %._crit_edge.i.i ]
  br label %divideSigned.exit

divideSigned.exit:                                ; preds = %divideSigned.exit.loopexit, %0
  %quotient.1.i.i = phi i64 [ 0, %0 ], [ %.lcssa2, %divideSigned.exit.loopexit ]
  %29 = sub nsw i64 0, %quotient.1.i.i
  %30 = select i1 %19, i64 %29, i64 %quotient.1.i.i
  %31 = sub nsw i64 %14, %30
  %32 = trunc i64 %31 to i32
  store i32 %1, i32* %8, align 4
  %33 = add nsw i32 %2, 1
  %34 = icmp eq i32 %33, 25
  %35 = select i1 %34, i32 0, i32 %33
  store i32 %35, i32* @HPbuff_ptr, align 4
  tail call void @write_uint32(i8* getelementptr inbounds ([11 x i8]* @.str2, i32 0, i32 0), i32 %32) nounwind
  ret void
}

define void @deriv() nounwind {
  %1 = tail call i32 @read_uint32(i8* getelementptr inbounds ([11 x i8]* @.str2, i32 0, i32 0)) nounwind
  %2 = load i32* @DERIVbuff_ptr, align 4
  %3 = getelementptr inbounds [2 x i32]* @DERIVbuff, i32 0, i32 %2
  %4 = load i32* %3, align 4
  %5 = sub nsw i32 %1, %4
  %ispos = icmp sgt i32 %5, -1
  %neg = sub i32 0, %5
  %6 = select i1 %ispos, i32 %5, i32 %neg
  store i32 %1, i32* %3, align 4
  %7 = add nsw i32 %2, 1
  %8 = icmp eq i32 %7, 2
  %9 = select i1 %8, i32 0, i32 %7
  store i32 %9, i32* @DERIVbuff_ptr, align 4
  tail call void @write_uint32(i8* getelementptr inbounds ([14 x i8]* @.str3, i32 0, i32 0), i32 %6) nounwind
  ret void
}

define void @mvWin() nounwind {
; <label>:0
  %1 = tail call i32 @read_uint32(i8* getelementptr inbounds ([14 x i8]* @.str3, i32 0, i32 0)) nounwind
  %2 = load i32* @WINbuff_ptr, align 4
  %3 = load i32* @WINsum, align 4
  %4 = getelementptr inbounds [16 x i32]* @WINbuff, i32 0, i32 %2
  %5 = load i32* %4, align 4
  %6 = add nsw i32 %3, %1
  %7 = sub i32 %6, %5
  store i32 %7, i32* @WINsum, align 4
  %8 = icmp sgt i32 %7, 512000
  br i1 %8, label %divideUnsigned.exit, label %9

; <label>:9                                       ; preds = %0
  %10 = icmp ult i32 %7, 16
  br i1 %10, label %divideUnsigned.exit, label %bb.nph6.i.preheader

bb.nph6.i.preheader:                              ; preds = %9
  %11 = sext i32 %7 to i64
  br label %bb.nph6.i

bb.nph6.i:                                        ; preds = %._crit_edge.i, %bb.nph6.i.preheader
  %.05.i = phi i64 [ %18, %._crit_edge.i ], [ %11, %bb.nph6.i.preheader ]
  %quotient.04.i = phi i64 [ %17, %._crit_edge.i ], [ 0, %bb.nph6.i.preheader ]
  %12 = lshr i64 %.05.i, 1
  %13 = icmp ugt i64 %12, 16
  br i1 %13, label %bb.nph.i.preheader, label %._crit_edge.i

bb.nph.i.preheader:                               ; preds = %bb.nph6.i
  br label %bb.nph.i

bb.nph.i:                                         ; preds = %bb.nph.i, %bb.nph.i.preheader
  %shifted_divisor.02.i = phi i64 [ %14, %bb.nph.i ], [ 16, %bb.nph.i.preheader ]
  %curr_quotient.01.i = phi i64 [ %15, %bb.nph.i ], [ 1, %bb.nph.i.preheader ]
  %14 = shl i64 %shifted_divisor.02.i, 1
  %15 = shl i64 %curr_quotient.01.i, 1
  %16 = icmp ult i64 %14, %12
  br i1 %16, label %bb.nph.i, label %._crit_edge.i.loopexit

._crit_edge.i.loopexit:                           ; preds = %bb.nph.i
  %.lcssa1 = phi i64 [ %15, %bb.nph.i ]
  %.lcssa = phi i64 [ %14, %bb.nph.i ]
  br label %._crit_edge.i

._crit_edge.i:                                    ; preds = %._crit_edge.i.loopexit, %bb.nph6.i
  %shifted_divisor.0.lcssa.i = phi i64 [ 16, %bb.nph6.i ], [ %.lcssa, %._crit_edge.i.loopexit ]
  %curr_quotient.0.lcssa.i = phi i64 [ 1, %bb.nph6.i ], [ %.lcssa1, %._crit_edge.i.loopexit ]
  %17 = add i64 %curr_quotient.0.lcssa.i, %quotient.04.i
  %18 = sub i64 %.05.i, %shifted_divisor.0.lcssa.i
  %19 = icmp ult i64 %18, 16
  br i1 %19, label %divideUnsigned.exit.loopexit, label %bb.nph6.i

divideUnsigned.exit.loopexit:                     ; preds = %._crit_edge.i
  %.lcssa2 = phi i64 [ %17, %._crit_edge.i ]
  %extract.t = trunc i64 %.lcssa2 to i32
  br label %divideUnsigned.exit

divideUnsigned.exit:                              ; preds = %divideUnsigned.exit.loopexit, %9, %0
  %output.0 = phi i32 [ 32000, %0 ], [ 0, %9 ], [ %extract.t, %divideUnsigned.exit.loopexit ]
  store i32 %1, i32* %4, align 4
  %20 = add nsw i32 %2, 1
  %21 = icmp eq i32 %20, 16
  %22 = select i1 %21, i32 0, i32 %20
  store i32 %22, i32* @WINbuff_ptr, align 4
  tail call void @write_uint32(i8* getelementptr inbounds ([17 x i8]* @.str4, i32 0, i32 0), i32 %output.0) nounwind
  ret void
}

define void @QRSFilt(i8 zeroext %initialize) nounwind {
  %1 = icmp eq i8 %initialize, 0
  br i1 %1, label %5, label %.preheader.preheader

.preheader.preheader:                             ; preds = %0
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %index.05.i = phi i32 [ %2, %.preheader ], [ 0, %.preheader.preheader ]
  %scevgep10.i = getelementptr [10 x i32]* @LPbuff, i32 0, i32 %index.05.i
  tail call void @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep10.i, align 4
  %2 = add nsw i32 %index.05.i, 1
  %exitcond2 = icmp eq i32 %2, 10
  br i1 %exitcond2, label %bb.nph4.i.preheader, label %.preheader

bb.nph4.i.preheader:                              ; preds = %.preheader
  br label %bb.nph4.i

bb.nph4.i:                                        ; preds = %bb.nph4.i, %bb.nph4.i.preheader
  %index.13.i = phi i32 [ %3, %bb.nph4.i ], [ 0, %bb.nph4.i.preheader ]
  %scevgep8.i = getelementptr [25 x i32]* @HPbuff, i32 0, i32 %index.13.i
  tail call void @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep8.i, align 4
  %3 = add nsw i32 %index.13.i, 1
  %exitcond = icmp eq i32 %3, 25
  br i1 %exitcond, label %bb.nph.i.preheader, label %bb.nph4.i

bb.nph.i.preheader:                               ; preds = %bb.nph4.i
  br label %bb.nph.i

bb.nph.i:                                         ; preds = %bb.nph.i, %bb.nph.i.preheader
  %index.21.i = phi i32 [ %4, %bb.nph.i ], [ 0, %bb.nph.i.preheader ]
  %scevgep.i = getelementptr [2 x i32]* @DERIVbuff, i32 0, i32 %index.21.i
  tail call void @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep.i, align 4
  %4 = add nsw i32 %index.21.i, 1
  %exitcond1 = icmp eq i32 %4, 2
  br i1 %exitcond1, label %initFilt.exit, label %bb.nph.i

initFilt.exit:                                    ; preds = %bb.nph.i
  store i32 0, i32* @LPy1, align 4
  store i32 0, i32* @LPy2, align 4
  store i32 0, i32* @HPy1, align 4
  store i32 0, i32* @WINsum, align 4
  store i32 0, i32* @LPbuff_ptr, align 4
  store i32 0, i32* @HPbuff_ptr, align 4
  store i32 0, i32* @DERIVbuff_ptr, align 4
  store i32 0, i32* @WINbuff_ptr, align 4
  ret void

; <label>:5                                       ; preds = %0
  tail call void @lpFilt()
  tail call void @hpFilt()
  %6 = tail call i32 @read_uint32(i8* getelementptr inbounds ([11 x i8]* @.str2, i32 0, i32 0)) nounwind
  %7 = load i32* @DERIVbuff_ptr, align 4
  %8 = getelementptr inbounds [2 x i32]* @DERIVbuff, i32 0, i32 %7
  %9 = load i32* %8, align 4
  %10 = sub nsw i32 %6, %9
  %ispos.i = icmp sgt i32 %10, -1
  %neg.i = sub i32 0, %10
  %11 = select i1 %ispos.i, i32 %10, i32 %neg.i
  store i32 %6, i32* %8, align 4
  %12 = add nsw i32 %7, 1
  %13 = icmp eq i32 %12, 2
  %14 = select i1 %13, i32 0, i32 %12
  store i32 %14, i32* @DERIVbuff_ptr, align 4
  tail call void @write_uint32(i8* getelementptr inbounds ([14 x i8]* @.str3, i32 0, i32 0), i32 %11) nounwind
  %15 = tail call i32 @read_uint32(i8* getelementptr inbounds ([14 x i8]* @.str3, i32 0, i32 0)) nounwind
  %16 = load i32* @WINbuff_ptr, align 4
  %17 = load i32* @WINsum, align 4
  %18 = getelementptr inbounds [16 x i32]* @WINbuff, i32 0, i32 %16
  %19 = load i32* %18, align 4
  %20 = add nsw i32 %17, %15
  %21 = sub i32 %20, %19
  store i32 %21, i32* @WINsum, align 4
  %22 = icmp sgt i32 %21, 512000
  br i1 %22, label %mvWin.exit, label %23

; <label>:23                                      ; preds = %5
  %24 = icmp ult i32 %21, 16
  br i1 %24, label %mvWin.exit, label %bb.nph6.i.preheader.i

bb.nph6.i.preheader.i:                            ; preds = %23
  %25 = sext i32 %21 to i64
  br label %bb.nph6.i.i

bb.nph6.i.i:                                      ; preds = %._crit_edge.i.i, %bb.nph6.i.preheader.i
  %.05.i.i = phi i64 [ %32, %._crit_edge.i.i ], [ %25, %bb.nph6.i.preheader.i ]
  %quotient.04.i.i = phi i64 [ %31, %._crit_edge.i.i ], [ 0, %bb.nph6.i.preheader.i ]
  %26 = lshr i64 %.05.i.i, 1
  %27 = icmp ugt i64 %26, 16
  br i1 %27, label %bb.nph.i.i.preheader, label %._crit_edge.i.i

bb.nph.i.i.preheader:                             ; preds = %bb.nph6.i.i
  br label %bb.nph.i.i

bb.nph.i.i:                                       ; preds = %bb.nph.i.i, %bb.nph.i.i.preheader
  %shifted_divisor.02.i.i = phi i64 [ %28, %bb.nph.i.i ], [ 16, %bb.nph.i.i.preheader ]
  %curr_quotient.01.i.i = phi i64 [ %29, %bb.nph.i.i ], [ 1, %bb.nph.i.i.preheader ]
  %28 = shl i64 %shifted_divisor.02.i.i, 1
  %29 = shl i64 %curr_quotient.01.i.i, 1
  %30 = icmp ult i64 %28, %26
  br i1 %30, label %bb.nph.i.i, label %._crit_edge.i.i.loopexit

._crit_edge.i.i.loopexit:                         ; preds = %bb.nph.i.i
  %.lcssa3 = phi i64 [ %29, %bb.nph.i.i ]
  %.lcssa = phi i64 [ %28, %bb.nph.i.i ]
  br label %._crit_edge.i.i

._crit_edge.i.i:                                  ; preds = %._crit_edge.i.i.loopexit, %bb.nph6.i.i
  %shifted_divisor.0.lcssa.i.i = phi i64 [ 16, %bb.nph6.i.i ], [ %.lcssa, %._crit_edge.i.i.loopexit ]
  %curr_quotient.0.lcssa.i.i = phi i64 [ 1, %bb.nph6.i.i ], [ %.lcssa3, %._crit_edge.i.i.loopexit ]
  %31 = add i64 %curr_quotient.0.lcssa.i.i, %quotient.04.i.i
  %32 = sub i64 %.05.i.i, %shifted_divisor.0.lcssa.i.i
  %33 = icmp ult i64 %32, 16
  br i1 %33, label %divideUnsigned.exit.loopexit.i, label %bb.nph6.i.i

divideUnsigned.exit.loopexit.i:                   ; preds = %._crit_edge.i.i
  %.lcssa4 = phi i64 [ %31, %._crit_edge.i.i ]
  %extract.t.i = trunc i64 %.lcssa4 to i32
  br label %mvWin.exit

mvWin.exit:                                       ; preds = %divideUnsigned.exit.loopexit.i, %23, %5
  %output.0.i = phi i32 [ 32000, %5 ], [ 0, %23 ], [ %extract.t.i, %divideUnsigned.exit.loopexit.i ]
  store i32 %15, i32* %18, align 4
  %34 = add nsw i32 %16, 1
  %35 = icmp eq i32 %34, 16
  %36 = select i1 %35, i32 0, i32 %34
  store i32 %36, i32* @WINbuff_ptr, align 4
  tail call void @write_uint32(i8* getelementptr inbounds ([17 x i8]* @.str4, i32 0, i32 0), i32 %output.0.i) nounwind
  ret void
}
