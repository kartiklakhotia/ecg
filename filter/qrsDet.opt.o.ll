; ModuleID = 'qrsDet.opt.o'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

@threshold = global double 3.125000e-01, align 8
@NOISEbuff = common global [8 x i32] zeroinitializer, align 4
@RRbuff = common global [8 x i32] zeroinitializer, align 4
@DDCALCbuff = common global [2 x i32] zeroinitializer, align 4
@maxDer = common global i32 0, align 4
@DDCALCbuff_ptr = common global i32 0, align 4
@DDbuff_ptr = common global i32 0, align 4
@NOISEbuff_ptr = common global i32 0, align 4
@RRbuff_ptr = common global i32 0, align 4
@QRSbuff_ptr = common global i32 0, align 4
@timeSinceMaxPeak = common global i32 0, align 4
@maxPeak = common global i32 0, align 4
@lastDatum = common global i32 0, align 4
@.str = private constant [17 x i8] c"filt_output_pipe\00"
@DDbuff = common global [92 x i32] zeroinitializer, align 4
@QRSbuff = common global [8 x i32] zeroinitializer, align 4
@.str1 = private constant [15 x i8] c"det_input_pipe\00"
@.str2 = private constant [16 x i8] c"filt_input_pipe\00"
@.str3 = private constant [16 x i8] c"det_output_pipe\00"

define void @initDet() nounwind {
bb.nph3:
  br label %0

; <label>:0                                       ; preds = %0, %bb.nph3
  %index.02 = phi i32 [ 0, %bb.nph3 ], [ %2, %0 ]
  %scevgep6 = getelementptr [8 x i32]* @RRbuff, i32 0, i32 %index.02
  %scevgep5 = getelementptr [8 x i32]* @NOISEbuff, i32 0, i32 %index.02
  %1 = tail call i32 (...)* @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep5, align 4
  store i32 200, i32* %scevgep6, align 4
  %2 = add nsw i32 %index.02, 1
  %exitcond = icmp eq i32 %2, 8
  br i1 %exitcond, label %bb.nph.preheader, label %0

bb.nph.preheader:                                 ; preds = %0
  br label %bb.nph

bb.nph:                                           ; preds = %bb.nph, %bb.nph.preheader
  %index.11 = phi i32 [ %4, %bb.nph ], [ 0, %bb.nph.preheader ]
  %scevgep = getelementptr [2 x i32]* @DDCALCbuff, i32 0, i32 %index.11
  %3 = tail call i32 (...)* @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep, align 4
  %4 = add nsw i32 %index.11, 1
  %exitcond1 = icmp eq i32 %4, 2
  br i1 %exitcond1, label %._crit_edge, label %bb.nph

._crit_edge:                                      ; preds = %bb.nph
  store i32 0, i32* @maxDer, align 4
  store i32 0, i32* @DDCALCbuff_ptr, align 4
  store i32 0, i32* @DDbuff_ptr, align 4
  store i32 7, i32* @NOISEbuff_ptr, align 4
  store i32 7, i32* @RRbuff_ptr, align 4
  store i32 7, i32* @QRSbuff_ptr, align 4
  store i32 0, i32* @timeSinceMaxPeak, align 4
  store i32 0, i32* @maxPeak, align 4
  store i32 0, i32* @lastDatum, align 4
  ret void
}

declare i32 @__loop_pipelining_on__(...)

define i32 @peak() nounwind {
; <label>:0
  %1 = load i32* @timeSinceMaxPeak, align 4
  %2 = load i32* @maxPeak, align 4
  %3 = tail call i32 @read_uint32(i8* getelementptr inbounds ([17 x i8]* @.str, i32 0, i32 0)) nounwind
  %4 = icmp sgt i32 %1, 0
  %5 = zext i1 %4 to i32
  %6 = add i32 %5, %1
  %7 = load i32* @lastDatum, align 4
  %8 = icmp sgt i32 %3, %7
  %9 = icmp sgt i32 %3, %2
  %10 = and i1 %8, %9
  %11 = ashr i32 %2, 1
  %12 = icmp slt i32 %3, %11
  %13 = icmp sgt i32 %6, 19
  %or.cond = or i1 %12, %13
  %.demorgan = and i1 %8, %9
  %14 = xor i1 %.demorgan, true
  %15 = and i1 %or.cond, %14
  br i1 %15, label %16, label %18

; <label>:16                                      ; preds = %0
  %17 = icmp sgt i32 %2, 6
  %phitmp = select i1 %17, i32 %2, i32 0
  br label %18

; <label>:18                                      ; preds = %16, %0
  %19 = phi i32 [ 0, %0 ], [ %phitmp, %16 ]
  %20 = select i1 %15, i32 0, i32 %2
  %21 = select i1 %15, i32 0, i32 %6
  %22 = select i1 %10, i32 %3, i32 %20
  br i1 %10, label %23, label %25

; <label>:23                                      ; preds = %18
  %24 = icmp sgt i32 %22, 2
  %phitmp1 = select i1 %24, i32 1, i32 %21
  br label %25

; <label>:25                                      ; preds = %23, %18
  %26 = phi i32 [ %21, %18 ], [ %phitmp1, %23 ]
  store i32 %26, i32* @timeSinceMaxPeak, align 4
  store i32 %22, i32* @maxPeak, align 4
  store i32 %3, i32* @lastDatum, align 4
  ret i32 %19
}

declare i32 @read_uint32(i8*)

define i32 @circUpdateDet(i32 %ptr, i32 %size) nounwind readnone {
  %1 = add nsw i32 %ptr, 1
  %2 = icmp eq i32 %1, %size
  %3 = select i1 %2, i32 0, i32 %1
  ret i32 %3
}

define i32 @revUpdateDet(i32 %ptr, i32 %max) nounwind readnone {
  %1 = add nsw i32 %ptr, -1
  %2 = icmp eq i32 %ptr, 0
  %max. = select i1 %2, i32 %max, i32 %1
  ret i32 %max.
}

define i32 @ddCalc(i32 %datum) nounwind {
  %1 = load i32* @DDCALCbuff_ptr, align 4
  %2 = getelementptr inbounds [2 x i32]* @DDCALCbuff, i32 0, i32 %1
  %3 = load i32* %2, align 4
  %4 = sub nsw i32 %datum, %3
  store i32 %datum, i32* %2, align 4
  %5 = add nsw i32 %1, 1
  %6 = icmp eq i32 %5, 2
  %7 = select i1 %6, i32 0, i32 %5
  store i32 %7, i32* @DDCALCbuff_ptr, align 4
  ret i32 %4
}

define i32 @threshCalc(i32 %qmean, i32 %nmean) nounwind readonly {
  %1 = sub nsw i32 %qmean, %nmean
  %2 = sitofp i32 %1 to double
  %3 = load double* @threshold, align 8
  %4 = fmul double %3, %2
  %5 = fptosi double %4 to i32
  %6 = add nsw i32 %5, %nmean
  ret i32 %6
}

define zeroext i8 @blsCheck() nounwind {
bb.nph:
  %0 = load i32* @DDbuff_ptr, align 4
  br label %1

; <label>:1                                       ; preds = %1, %bb.nph
  %max.07 = phi i32 [ 0, %bb.nph ], [ %8, %1 ]
  %min.06 = phi i32 [ 0, %bb.nph ], [ %10, %1 ]
  %maxt.05 = phi i32 [ undef, %bb.nph ], [ %7, %1 ]
  %mint.04 = phi i32 [ undef, %bb.nph ], [ %9, %1 ]
  %2 = phi i32 [ 0, %bb.nph ], [ %12, %1 ]
  %ptr.02 = phi i32 [ %0, %bb.nph ], [ %14, %1 ]
  %3 = getelementptr inbounds [92 x i32]* @DDbuff, i32 0, i32 %ptr.02
  %4 = load i32* %3, align 4
  %5 = icmp sgt i32 %4, %max.07
  %6 = icmp slt i32 %4, %min.06
  %7 = select i1 %5, i32 %2, i32 %maxt.05
  %8 = select i1 %5, i32 %4, i32 %max.07
  %9 = select i1 %6, i32 %2, i32 %mint.04
  %10 = select i1 %6, i32 %4, i32 %min.06
  %11 = add nsw i32 %ptr.02, 1
  %12 = add nsw i32 %2, 1
  %13 = icmp eq i32 %11, 92
  %14 = select i1 %13, i32 0, i32 %11
  %exitcond4 = icmp eq i32 %12, 44
  br i1 %exitcond4, label %._crit_edge, label %1

._crit_edge:                                      ; preds = %1
  %.lcssa3 = phi i32 [ %10, %1 ]
  %.lcssa2 = phi i32 [ %9, %1 ]
  %.lcssa1 = phi i32 [ %8, %1 ]
  %.lcssa = phi i32 [ %7, %1 ]
  store i32 %.lcssa1, i32* @maxDer, align 4
  %15 = sub nsw i32 0, %.lcssa3
  %16 = ashr i32 %15, 3
  %17 = icmp sgt i32 %.lcssa1, %16
  %18 = ashr i32 %.lcssa1, 3
  %19 = icmp slt i32 %18, %15
  %or.cond = and i1 %17, %19
  br i1 %or.cond, label %20, label %23

; <label>:20                                      ; preds = %._crit_edge
  %21 = sub nsw i32 %.lcssa, %.lcssa2
  %ispos = icmp sgt i32 %21, -1
  %neg = sub i32 0, %21
  %22 = select i1 %ispos, i32 %21, i32 %neg
  %phitmp = icmp sgt i32 %22, 29
  br label %23

; <label>:23                                      ; preds = %20, %._crit_edge
  %24 = phi i1 [ true, %._crit_edge ], [ %phitmp, %20 ]
  %25 = zext i1 %24 to i8
  ret i8 %25
}

define void @qrsUpdate(i32 %qrsVal, i32 %rrVal) nounwind {
  %1 = load i32* @QRSbuff_ptr, align 4
  %2 = load i32* @RRbuff_ptr, align 4
  %3 = getelementptr inbounds [8 x i32]* @QRSbuff, i32 0, i32 %1
  store i32 %qrsVal, i32* %3, align 4
  %4 = getelementptr inbounds [8 x i32]* @RRbuff, i32 0, i32 %2
  store i32 %rrVal, i32* %4, align 4
  %5 = add nsw i32 %1, -1
  %6 = icmp eq i32 %1, 0
  %max..i = select i1 %6, i32 7, i32 %5
  store i32 %max..i, i32* @QRSbuff_ptr, align 4
  %7 = add nsw i32 %2, -1
  %8 = icmp eq i32 %2, 0
  %max..i1 = select i1 %8, i32 7, i32 %7
  store i32 %max..i1, i32* @RRbuff_ptr, align 4
  ret void
}

define void @noiseUpdate(i32 %noiseVal) nounwind {
  %1 = load i32* @NOISEbuff_ptr, align 4
  %2 = getelementptr inbounds [8 x i32]* @NOISEbuff, i32 0, i32 %1
  store i32 %noiseVal, i32* %2, align 4
  %3 = add nsw i32 %1, -1
  %4 = icmp eq i32 %1, 0
  %max..i = select i1 %4, i32 7, i32 %3
  store i32 %max..i, i32* @NOISEbuff_ptr, align 4
  ret void
}

define void @qrsDet() noreturn nounwind {
; <label>:0
  %RSETbuff = alloca [8 x i32], align 4
  br label %1

; <label>:1                                       ; preds = %1, %0
  %index.02.i = phi i32 [ 0, %0 ], [ %3, %1 ]
  %scevgep5.i = getelementptr [8 x i32]* @NOISEbuff, i32 0, i32 %index.02.i
  %scevgep6.i = getelementptr [8 x i32]* @RRbuff, i32 0, i32 %index.02.i
  %2 = call i32 (...)* @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep5.i, align 4
  store i32 200, i32* %scevgep6.i, align 4
  %3 = add nsw i32 %index.02.i, 1
  %exitcond6 = icmp eq i32 %3, 8
  br i1 %exitcond6, label %bb.nph.i.preheader, label %1

bb.nph.i.preheader:                               ; preds = %1
  br label %bb.nph.i

bb.nph.i:                                         ; preds = %bb.nph.i, %bb.nph.i.preheader
  %index.11.i = phi i32 [ %5, %bb.nph.i ], [ 0, %bb.nph.i.preheader ]
  %scevgep.i = getelementptr [2 x i32]* @DDCALCbuff, i32 0, i32 %index.11.i
  %4 = call i32 (...)* @__loop_pipelining_on__(i32 7, i32 2, i32 0) nounwind
  store i32 0, i32* %scevgep.i, align 4
  %5 = add nsw i32 %index.11.i, 1
  %exitcond = icmp eq i32 %5, 2
  br i1 %exitcond, label %initDet.exit, label %bb.nph.i

initDet.exit:                                     ; preds = %bb.nph.i
  store i32 0, i32* @maxDer, align 4
  store i32 0, i32* @DDCALCbuff_ptr, align 4
  store i32 0, i32* @DDbuff_ptr, align 4
  store i32 7, i32* @NOISEbuff_ptr, align 4
  store i32 7, i32* @RRbuff_ptr, align 4
  store i32 7, i32* @QRSbuff_ptr, align 4
  store i32 0, i32* @timeSinceMaxPeak, align 4
  store i32 0, i32* @maxPeak, align 4
  store i32 0, i32* @lastDatum, align 4
  call void @QRSFilt(i8 zeroext 1) nounwind
  br label %6

; <label>:6                                       ; preds = %211, %initDet.exit
  %init8Done.0 = phi i8 [ 0, %initDet.exit ], [ %init8Done_next.0, %211 ]
  %init8Done_next.1 = phi i8 [ undef, %initDet.exit ], [ %init8Done_next.0, %211 ]
  %noiseVal.1 = phi i32 [ undef, %initDet.exit ], [ %noiseVal.0, %211 ]
  %rrVal.1 = phi i32 [ undef, %initDet.exit ], [ %rrVal.0, %211 ]
  %qrsVal.1 = phi i32 [ undef, %initDet.exit ], [ %qrsVal.0, %211 ]
  %nmean.1 = phi i32 [ undef, %initDet.exit ], [ %nmean.3, %211 ]
  %qmean.1 = phi i32 [ undef, %initDet.exit ], [ %qmean.4, %211 ]
  %sbLoc.1 = phi i32 [ 300, %initDet.exit ], [ %sbLoc.0, %211 ]
  %sb_count.1 = phi i32 [ 300, %initDet.exit ], [ %sb_count.2, %211 ]
  %det_thresh.1 = phi i32 [ 0, %initDet.exit ], [ %det_thresh.5, %211 ]
  %initMax.1 = phi i32 [ 0, %initDet.exit ], [ %initMax.2, %211 ]
  %rset_count.1 = phi i32 [ 0, %initDet.exit ], [ %rset_count.2, %211 ]
  %preBlank_count.0 = phi i32 [ 0, %initDet.exit ], [ %42, %211 ]
  %initBlank.1 = phi i32 [ 0, %initDet.exit ], [ %initBlank.2, %211 ]
  %count.0 = phi i32 [ 0, %initDet.exit ], [ %count.1, %211 ]
  %qpk_count.1 = phi i32 [ 0, %initDet.exit ], [ %qpk_count.0, %211 ]
  %sbPeak.1 = phi i32 [ 0, %initDet.exit ], [ %sbPeak.0, %211 ]
  %tempPeak.0 = phi i32 [ 0, %initDet.exit ], [ %41, %211 ]
  %7 = call i32 @read_uint32(i8* getelementptr inbounds ([15 x i8]* @.str1, i32 0, i32 0)) nounwind
  call void @write_uint32(i8* getelementptr inbounds ([16 x i8]* @.str2, i32 0, i32 0), i32 %7) nounwind
  call void @QRSFilt(i8 zeroext 0) nounwind
  %8 = load i32* @timeSinceMaxPeak, align 4
  %9 = load i32* @maxPeak, align 4
  %10 = call i32 @read_uint32(i8* getelementptr inbounds ([17 x i8]* @.str, i32 0, i32 0)) nounwind
  %11 = icmp sgt i32 %8, 0
  %12 = zext i1 %11 to i32
  %13 = add i32 %12, %8
  %14 = load i32* @lastDatum, align 4
  %15 = icmp sgt i32 %10, %14
  %16 = icmp sgt i32 %10, %9
  %17 = and i1 %15, %16
  %18 = ashr i32 %9, 1
  %19 = icmp slt i32 %10, %18
  %20 = icmp sgt i32 %13, 19
  %or.cond.i = or i1 %19, %20
  %.demorgan42 = and i1 %15, %16
  %21 = xor i1 %.demorgan42, true
  %22 = and i1 %or.cond.i, %21
  br i1 %22, label %23, label %25

; <label>:23                                      ; preds = %6
  %24 = icmp sgt i32 %9, 6
  %phitmp.i = select i1 %24, i32 %9, i32 0
  br label %25

; <label>:25                                      ; preds = %23, %6
  %26 = phi i32 [ 0, %6 ], [ %phitmp.i, %23 ]
  %27 = select i1 %22, i32 0, i32 %9
  %28 = select i1 %22, i32 0, i32 %13
  %29 = select i1 %17, i32 %10, i32 %27
  br i1 %17, label %30, label %peak.exit

; <label>:30                                      ; preds = %25
  %31 = icmp sgt i32 %29, 2
  %phitmp1.i = select i1 %31, i32 1, i32 %28
  br label %peak.exit

peak.exit:                                        ; preds = %30, %25
  %32 = phi i32 [ %28, %25 ], [ %phitmp1.i, %30 ]
  store i32 %32, i32* @timeSinceMaxPeak, align 4
  store i32 %29, i32* @maxPeak, align 4
  store i32 %10, i32* @lastDatum, align 4
  %33 = icmp sgt i32 %26, 0
  %34 = icmp sgt i32 %preBlank_count.0, 0
  %35 = icmp sgt i32 %26, %tempPeak.0
  %36 = add nsw i32 %preBlank_count.0, -1
  %37 = icmp eq i32 %36, 0
  %38 = xor i1 %34, true
  %39 = and i1 %33, %38
  %.not13 = xor i1 %35, true
  %not.brmerge = and i1 %33, %.not13
  br i1 %39, label %.thread, label %40

; <label>:40                                      ; preds = %peak.exit
  %phitmp18 = select i1 %35, i32 %26, i32 %tempPeak.0
  %phitmp1 = select i1 %35, i32 39, i32 %36
  br label %.thread

.thread:                                          ; preds = %40, %peak.exit
  %41 = phi i32 [ %phitmp18, %40 ], [ %26, %peak.exit ]
  %42 = phi i32 [ %phitmp1, %40 ], [ 39, %peak.exit ]
  %.not14 = xor i1 %33, true
  %.not2.not = and i1 %34, %.not14
  %tmp = or i1 %not.brmerge, %.not2.not
  %43 = and i1 %37, %tmp
  %44 = select i1 %43, i32 %41, i32 0
  %45 = load i32* @DDCALCbuff_ptr, align 4
  %46 = getelementptr inbounds [2 x i32]* @DDCALCbuff, i32 0, i32 %45
  %47 = load i32* %46, align 4
  %48 = sub nsw i32 %7, %47
  store i32 %7, i32* %46, align 4
  %49 = add nsw i32 %45, 1
  %50 = icmp eq i32 %49, 2
  %51 = select i1 %50, i32 0, i32 %49
  store i32 %51, i32* @DDCALCbuff_ptr, align 4
  %52 = load i32* @DDbuff_ptr, align 4
  %53 = getelementptr inbounds [92 x i32]* @DDbuff, i32 0, i32 %52
  store i32 %48, i32* %53, align 4
  %54 = add nsw i32 %52, 1
  %55 = icmp eq i32 %54, 92
  %56 = select i1 %55, i32 0, i32 %54
  store i32 %56, i32* @DDbuff_ptr, align 4
  %57 = add nsw i32 %count.0, 1
  %58 = icmp eq i8 %init8Done.0, 0
  br i1 %58, label %59, label %.preheader

.preheader:                                       ; preds = %.thread
  br label %80

; <label>:59                                      ; preds = %.thread
  %60 = add nsw i32 %initBlank.1, 1
  %61 = icmp eq i32 %60, 200
  %62 = icmp sgt i32 %44, 0
  %63 = select i1 %62, i32 16, i32 %57
  %64 = select i1 %61, i32 0, i32 %60
  %65 = getelementptr inbounds [8 x i32]* @QRSbuff, i32 0, i32 %qpk_count.1
  store i32 %initMax.1, i32* %65, align 4
  %66 = select i1 %61, i32 0, i32 %initMax.1
  %67 = zext i1 %61 to i32
  %68 = add i32 %67, %qpk_count.1
  %69 = icmp eq i32 %68, 8
  %70 = and i1 %61, %69
  %71 = zext i1 %70 to i8
  %72 = icmp sgt i32 %44, %66
  %73 = select i1 %72, i32 %44, i32 %66
  br i1 %70, label %74, label %177

; <label>:74                                      ; preds = %59
  %75 = call i32 @meanCalc(i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 0)) nounwind
  %76 = sitofp i32 %75 to double
  %77 = load double* @threshold, align 8
  %78 = fmul double %77, %76
  %79 = fptosi double %78 to i32
  br label %177

; <label>:80                                      ; preds = %80, %.preheader
  %max.07.i = phi i32 [ %87, %80 ], [ 0, %.preheader ]
  %min.06.i = phi i32 [ %89, %80 ], [ 0, %.preheader ]
  %maxt.05.i = phi i32 [ %86, %80 ], [ undef, %.preheader ]
  %mint.04.i = phi i32 [ %88, %80 ], [ undef, %.preheader ]
  %81 = phi i32 [ %91, %80 ], [ 0, %.preheader ]
  %ptr.02.i = phi i32 [ %93, %80 ], [ %56, %.preheader ]
  %82 = getelementptr inbounds [92 x i32]* @DDbuff, i32 0, i32 %ptr.02.i
  %83 = load i32* %82, align 4
  %84 = icmp sgt i32 %83, %max.07.i
  %85 = icmp slt i32 %83, %min.06.i
  %86 = select i1 %84, i32 %81, i32 %maxt.05.i
  %87 = select i1 %84, i32 %83, i32 %max.07.i
  %88 = select i1 %85, i32 %81, i32 %mint.04.i
  %89 = select i1 %85, i32 %83, i32 %min.06.i
  %90 = add nsw i32 %ptr.02.i, 1
  %91 = add nsw i32 %81, 1
  %92 = icmp eq i32 %90, 92
  %93 = select i1 %92, i32 0, i32 %90
  %exitcond4 = icmp eq i32 %91, 44
  br i1 %exitcond4, label %._crit_edge.i, label %80

._crit_edge.i:                                    ; preds = %80
  %.lcssa3 = phi i32 [ %89, %80 ]
  %.lcssa2 = phi i32 [ %88, %80 ]
  %.lcssa1 = phi i32 [ %87, %80 ]
  %.lcssa = phi i32 [ %86, %80 ]
  store i32 %.lcssa1, i32* @maxDer, align 4
  %94 = sub nsw i32 0, %.lcssa3
  %95 = ashr i32 %94, 3
  %96 = icmp sgt i32 %.lcssa1, %95
  %97 = ashr i32 %.lcssa1, 3
  %98 = icmp slt i32 %97, %94
  %or.cond.i11 = and i1 %96, %98
  br i1 %or.cond.i11, label %99, label %blsCheck.exit

; <label>:99                                      ; preds = %._crit_edge.i
  %100 = sub nsw i32 %.lcssa, %.lcssa2
  %ispos.i = icmp sgt i32 %100, -1
  %neg.i = sub i32 0, %100
  %101 = select i1 %ispos.i, i32 %100, i32 %neg.i
  %phitmp.i12 = icmp sgt i32 %101, 29
  br label %blsCheck.exit

blsCheck.exit:                                    ; preds = %99, %._crit_edge.i
  %102 = phi i1 [ true, %._crit_edge.i ], [ %phitmp.i12, %99 ]
  %103 = icmp sgt i32 %44, %det_thresh.1
  %104 = xor i1 %102, true
  %105 = and i1 %103, %104
  %.not5 = icmp slt i32 %44, 1
  %or.cond = or i1 %.not5, %103
  %.demorgan = or i1 %or.cond, %102
  %106 = xor i1 %.demorgan, true
  %107 = icmp sgt i32 %44, %sbPeak.1
  %or.cond6 = and i1 %107, %106
  %108 = icmp sgt i32 %57, 87
  %109 = and i1 %or.cond6, %108
  %110 = select i1 %.demorgan, i32 %noiseVal.1, i32 %44
  br i1 %.demorgan, label %123, label %111

; <label>:111                                     ; preds = %blsCheck.exit
  %112 = load i32* @NOISEbuff_ptr, align 4
  %113 = getelementptr inbounds [8 x i32]* @NOISEbuff, i32 0, i32 %112
  store i32 %110, i32* %113, align 4
  %114 = add nsw i32 %112, -1
  %115 = icmp eq i32 %112, 0
  %max..i.i8 = select i1 %115, i32 7, i32 %114
  store i32 %max..i.i8, i32* @NOISEbuff_ptr, align 4
  %116 = call i32 @meanCalc(i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 0)) nounwind
  %117 = sub nsw i32 %qmean.1, %116
  %118 = sitofp i32 %117 to double
  %119 = load double* @threshold, align 8
  %120 = fmul double %119, %118
  %121 = fptosi double %120 to i32
  %122 = add nsw i32 %121, %116
  br label %123

; <label>:123                                     ; preds = %111, %blsCheck.exit
  %nmean.2 = phi i32 [ %116, %111 ], [ %nmean.1, %blsCheck.exit ]
  %det_thresh.2 = phi i32 [ %122, %111 ], [ %det_thresh.1, %blsCheck.exit ]
  %124 = select i1 %109, i32 %44, i32 %sbPeak.1
  %125 = add nsw i32 %count.0, -15
  %126 = select i1 %109, i32 %125, i32 %sbLoc.1
  %127 = icmp sgt i32 %57, %sb_count.1
  br i1 %127, label %128, label %132

; <label>:128                                     ; preds = %123
  %129 = ashr i32 %det_thresh.2, 1
  %.not = xor i1 %103, true
  %130 = or i1 %102, %.not
  %131 = icmp sgt i32 %124, %129
  %. = and i1 %131, %130
  br label %132

; <label>:132                                     ; preds = %128, %123
  %133 = phi i1 [ false, %123 ], [ %., %128 ]
  %134 = or i1 %105, %133
  %135 = select i1 %105, i32 %44, i32 %qrsVal.1
  %136 = select i1 %105, i32 %125, i32 %rrVal.1
  %137 = select i1 %105, i32 16, i32 %57
  %138 = select i1 %133, i32 %124, i32 %135
  %139 = select i1 %133, i32 %126, i32 %136
  br i1 %134, label %141, label %.thread27

.thread27:                                        ; preds = %132
  %140 = load i32* @maxDer, align 4
  br label %162

; <label>:141                                     ; preds = %132
  %142 = load i32* @QRSbuff_ptr, align 4
  %143 = load i32* @RRbuff_ptr, align 4
  %144 = getelementptr inbounds [8 x i32]* @QRSbuff, i32 0, i32 %142
  store i32 %138, i32* %144, align 4
  %145 = getelementptr inbounds [8 x i32]* @RRbuff, i32 0, i32 %143
  store i32 %139, i32* %145, align 4
  %146 = add nsw i32 %142, -1
  %147 = icmp eq i32 %142, 0
  %max..i.i = select i1 %147, i32 7, i32 %146
  store i32 %max..i.i, i32* @QRSbuff_ptr, align 4
  %148 = add nsw i32 %143, -1
  %149 = icmp eq i32 %143, 0
  %max..i1.i = select i1 %149, i32 7, i32 %148
  store i32 %max..i1.i, i32* @RRbuff_ptr, align 4
  %150 = call i32 @meanCalc(i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 0)) nounwind
  %151 = call i32 @meanCalc(i32* getelementptr inbounds ([8 x i32]* @RRbuff, i32 0, i32 0)) nounwind
  %152 = sub nsw i32 %150, %nmean.2
  %153 = sitofp i32 %152 to double
  %154 = load double* @threshold, align 8
  %155 = fmul double %154, %153
  %156 = fptosi double %155 to i32
  %157 = add nsw i32 %156, %nmean.2
  %158 = load i32* @maxDer, align 4
  %159 = ashr i32 %151, 1
  %160 = add nsw i32 %151, 16
  %161 = add nsw i32 %160, %159
  br label %162

; <label>:162                                     ; preds = %141, %.thread27
  %163 = phi i32 [ %158, %141 ], [ %140, %.thread27 ]
  %det_thresh.325 = phi i32 [ %157, %141 ], [ %det_thresh.2, %.thread27 ]
  %qmean.223 = phi i32 [ %150, %141 ], [ %qmean.1, %.thread27 ]
  %164 = phi i32 [ %161, %141 ], [ %sb_count.1, %.thread27 ]
  %165 = select i1 %133, i32 %126, i32 0
  %.7 = sub i32 %137, %165
  br i1 %133, label %166, label %168

; <label>:166                                     ; preds = %162
  %167 = add nsw i32 %.7, 56
  br label %170

; <label>:168                                     ; preds = %162
  %169 = select i1 %105, i32 72, i32 0
  br label %170

; <label>:170                                     ; preds = %168, %166
  %171 = phi i32 [ %167, %166 ], [ %169, %168 ]
  %172 = select i1 %134, i32 0, i32 %124
  %173 = select i1 %134, i32 0, i32 %163
  store i32 %173, i32* @maxDer, align 4
  %174 = select i1 %134, i32 0, i32 %initBlank.1
  %175 = select i1 %134, i32 0, i32 %initMax.1
  %176 = select i1 %134, i32 0, i32 %rset_count.1
  br label %177

; <label>:177                                     ; preds = %170, %74, %59
  %QRSdelay.0 = phi i32 [ %171, %170 ], [ 0, %74 ], [ 0, %59 ]
  %init8Done_next.0 = phi i8 [ %init8Done_next.1, %170 ], [ %71, %74 ], [ %71, %59 ]
  %noiseVal.0 = phi i32 [ %110, %170 ], [ %noiseVal.1, %74 ], [ %noiseVal.1, %59 ]
  %rrVal.0 = phi i32 [ %139, %170 ], [ %rrVal.1, %74 ], [ %rrVal.1, %59 ]
  %qrsVal.0 = phi i32 [ %138, %170 ], [ %qrsVal.1, %74 ], [ %qrsVal.1, %59 ]
  %nmean.0 = phi i32 [ %nmean.2, %170 ], [ 0, %74 ], [ 0, %59 ]
  %qmean.0 = phi i32 [ %qmean.223, %170 ], [ %75, %74 ], [ %qmean.1, %59 ]
  %sbLoc.0 = phi i32 [ %126, %170 ], [ %sbLoc.1, %74 ], [ %sbLoc.1, %59 ]
  %sb_count.0 = phi i32 [ %164, %170 ], [ 330, %74 ], [ 330, %59 ]
  %det_thresh.0 = phi i32 [ %det_thresh.325, %170 ], [ %79, %74 ], [ %det_thresh.1, %59 ]
  %initMax.0 = phi i32 [ %175, %170 ], [ %73, %74 ], [ %73, %59 ]
  %rset_count.0 = phi i32 [ %176, %170 ], [ %rset_count.1, %74 ], [ %rset_count.1, %59 ]
  %initBlank.0 = phi i32 [ %174, %170 ], [ %64, %74 ], [ %64, %59 ]
  %count.1 = phi i32 [ %.7, %170 ], [ %63, %74 ], [ %63, %59 ]
  %qpk_count.0 = phi i32 [ %qpk_count.1, %170 ], [ %68, %74 ], [ %68, %59 ]
  %sbPeak.0 = phi i32 [ %172, %170 ], [ %sbPeak.1, %74 ], [ %sbPeak.1, %59 ]
  %178 = icmp eq i8 %init8Done_next.0, 0
  br i1 %178, label %211, label %179

; <label>:179                                     ; preds = %177
  %180 = add nsw i32 %initBlank.0, 1
  %181 = icmp eq i32 %180, 200
  %182 = select i1 %181, i32 0, i32 %180
  %183 = getelementptr inbounds [8 x i32]* %RSETbuff, i32 0, i32 %rset_count.0
  store i32 %initMax.0, i32* %183, align 4
  %184 = select i1 %181, i32 0, i32 %initMax.0
  %185 = zext i1 %181 to i32
  %186 = add i32 %185, %rset_count.0
  %187 = icmp eq i32 %186, 8
  %188 = select i1 %187, i32 0, i32 %nmean.0
  %189 = select i1 %187, i32 330, i32 %sb_count.0
  %190 = select i1 %187, i32 0, i32 %182
  %191 = select i1 %187, i32 0, i32 %186
  br i1 %187, label %192, label %208

; <label>:192                                     ; preds = %179
  %scevgep = getelementptr [8 x i32]* %RSETbuff, i32 0, i32 0
  %193 = load i32* %scevgep, align 4
  store i32 %193, i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 0), align 4
  store i32 0, i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 0), align 4
  %scevgep.1 = getelementptr [8 x i32]* %RSETbuff, i32 0, i32 1
  %194 = load i32* %scevgep.1, align 4
  store i32 %194, i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 1), align 4
  store i32 0, i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 1), align 4
  %scevgep.2 = getelementptr [8 x i32]* %RSETbuff, i32 0, i32 2
  %195 = load i32* %scevgep.2, align 4
  store i32 %195, i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 2), align 4
  store i32 0, i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 2), align 4
  %scevgep.3 = getelementptr [8 x i32]* %RSETbuff, i32 0, i32 3
  %196 = load i32* %scevgep.3, align 4
  store i32 %196, i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 3), align 4
  store i32 0, i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 3), align 4
  %scevgep.4 = getelementptr [8 x i32]* %RSETbuff, i32 0, i32 4
  %197 = load i32* %scevgep.4, align 4
  store i32 %197, i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 4), align 4
  store i32 0, i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 4), align 4
  %scevgep.5 = getelementptr [8 x i32]* %RSETbuff, i32 0, i32 5
  %198 = load i32* %scevgep.5, align 4
  store i32 %198, i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 5), align 4
  store i32 0, i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 5), align 4
  %scevgep.6 = getelementptr [8 x i32]* %RSETbuff, i32 0, i32 6
  %199 = load i32* %scevgep.6, align 4
  store i32 %199, i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 6), align 4
  store i32 0, i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 6), align 4
  %scevgep.7 = getelementptr [8 x i32]* %RSETbuff, i32 0, i32 7
  %200 = load i32* %scevgep.7, align 4
  store i32 %200, i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 7), align 4
  store i32 0, i32* getelementptr inbounds ([8 x i32]* @NOISEbuff, i32 0, i32 7), align 4
  %201 = call i32 @meanCalc(i32* getelementptr inbounds ([8 x i32]* @QRSbuff, i32 0, i32 0)) nounwind
  %202 = sub nsw i32 %201, %188
  %203 = sitofp i32 %202 to double
  %204 = load double* @threshold, align 8
  %205 = fmul double %204, %203
  %206 = fptosi double %205 to i32
  %207 = add nsw i32 %206, %188
  br label %208

; <label>:208                                     ; preds = %192, %179
  %qmean.3 = phi i32 [ %201, %192 ], [ %qmean.0, %179 ]
  %det_thresh.4 = phi i32 [ %207, %192 ], [ %det_thresh.0, %179 ]
  %209 = icmp sgt i32 %44, %184
  %210 = select i1 %209, i32 %44, i32 %184
  br label %211

; <label>:211                                     ; preds = %208, %177
  %nmean.3 = phi i32 [ %188, %208 ], [ %nmean.0, %177 ]
  %qmean.4 = phi i32 [ %qmean.3, %208 ], [ %qmean.0, %177 ]
  %sb_count.2 = phi i32 [ %189, %208 ], [ %sb_count.0, %177 ]
  %det_thresh.5 = phi i32 [ %det_thresh.4, %208 ], [ %det_thresh.0, %177 ]
  %initMax.2 = phi i32 [ %210, %208 ], [ %initMax.0, %177 ]
  %rset_count.2 = phi i32 [ %191, %208 ], [ %rset_count.0, %177 ]
  %initBlank.2 = phi i32 [ %190, %208 ], [ %initBlank.0, %177 ]
  call void @write_uint32(i8* getelementptr inbounds ([16 x i8]* @.str3, i32 0, i32 0), i32 %QRSdelay.0) nounwind
  br label %6
}

declare void @QRSFilt(i8 zeroext)

declare void @write_uint32(i8*, i32)

declare i32 @meanCalc(i32*)
