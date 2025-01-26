; ModuleID = '6.c'
source_filename = "6.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n64-S128"
target triple = "riscv64-unknown-linux-gnu"

@c = dso_local global float 0x400F0A3D80000000, align 4
@.str = private unnamed_addr constant [13 x i8] c"a[2]=%d c=%f\00", align 1

; Function Attrs: noinline nounwind optnone
define dso_local signext i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [3 x i32], align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 2, i32* %3, align 4
  %5 = getelementptr inbounds [3 x i32], [3 x i32]* %2, i64 0, i64 2
  store i32 1, i32* %5, align 4
  store i32 0, i32* %4, align 4
  br label %6

6:                                                ; preds = %28, %0
  %7 = load i32, i32* %4, align 4
  %8 = icmp slt i32 %7, 15
  br i1 %8, label %9, label %31

9:                                                ; preds = %6
  %10 = load float, float* @c, align 4
  %11 = fcmp oeq float %10, 1.000000e+00
  br i1 %11, label %12, label %13

12:                                               ; preds = %9
  br label %31

13:                                               ; preds = %9
  %14 = getelementptr inbounds [3 x i32], [3 x i32]* %2, i64 0, i64 2
  %15 = load i32, i32* %14, align 4
  %16 = icmp eq i32 %15, 10
  br i1 %16, label %17, label %18

17:                                               ; preds = %13
  br label %28

18:                                               ; preds = %13
  %19 = getelementptr inbounds [3 x i32], [3 x i32]* %2, i64 0, i64 2
  %20 = load i32, i32* %19, align 4
  %21 = add nsw i32 %20, 1
  store i32 %21, i32* %19, align 4
  %22 = load float, float* @c, align 4
  %23 = load i32, i32* %3, align 4
  %24 = sitofp i32 %23 to float
  %25 = fdiv float %22, %24
  %26 = load float, float* @c, align 4
  %27 = fadd float %26, %25
  store float %27, float* @c, align 4
  br label %28

28:                                               ; preds = %18, %17
  %29 = load i32, i32* %4, align 4
  %30 = add nsw i32 %29, 1
  store i32 %30, i32* %4, align 4
  br label %6, !llvm.loop !7

31:                                               ; preds = %12, %6
  %32 = getelementptr inbounds [3 x i32], [3 x i32]* %2, i64 0, i64 2
  %33 = load i32, i32* %32, align 4
  %34 = load float, float* @c, align 4
  %35 = fpext float %34 to double
  %36 = call signext i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i64 0, i64 0), i32 noundef signext %33, double noundef %35)
  ret i32 0
}

declare signext i32 @printf(i8* noundef, ...) #1

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+64bit,+a,+c,+d,+f,+m" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+64bit,+a,+c,+d,+f,+m" }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!6}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64d"}
!2 = !{i32 7, !"PIC Level", i32 2}
!3 = !{i32 7, !"PIE Level", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{i32 1, !"SmallDataLimit", i32 8}
!6 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!7 = distinct !{!7, !8}
!8 = !{!"llvm.loop.mustprogress"}
