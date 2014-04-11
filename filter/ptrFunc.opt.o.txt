; ModuleID = 'ptrFunc.opt.o'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32"
target triple = "i386-pc-linux-gnu"

define i32 @meanCalc(i32* nocapture %buffer) nounwind readonly {
  %scevgep.1 = getelementptr i32* %buffer, i32 1
  %1 = load i32* %scevgep.1, align 4
  %2 = load i32* %buffer, align 4
  %scevgep.2 = getelementptr i32* %buffer, i32 2
  %scevgep.3 = getelementptr i32* %buffer, i32 3
  %3 = add nsw i32 %1, %2
  %4 = load i32* %scevgep.2, align 4
  %5 = load i32* %scevgep.3, align 4
  %6 = add nsw i32 %4, %3
  %scevgep.4 = getelementptr i32* %buffer, i32 4
  %scevgep.5 = getelementptr i32* %buffer, i32 5
  %7 = add nsw i32 %5, %6
  %8 = load i32* %scevgep.4, align 4
  %9 = load i32* %scevgep.5, align 4
  %10 = add nsw i32 %8, %7
  %scevgep.6 = getelementptr i32* %buffer, i32 6
  %scevgep.7 = getelementptr i32* %buffer, i32 7
  %11 = add nsw i32 %9, %10
  %12 = load i32* %scevgep.6, align 4
  %13 = load i32* %scevgep.7, align 4
  %14 = add nsw i32 %12, %11
  %15 = add nsw i32 %13, %14
  %phitmp = ashr i32 %15, 3
  ret i32 %phitmp
}
