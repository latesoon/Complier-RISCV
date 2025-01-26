@a = dso_local global i32 0, align 4
@.str = private unnamed_addr constant [8 x i8] c"out:%d\0A\00", align 1

define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [3 x i32], align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4

  store i32 0, i32* %1, align 4
  store i32 2, i32* %3, align 4
  store i32 3, i32* %4, align 4

  %7 = load i32, i32* %3, align 4
  %8 = load i32, i32* %4, align 4
  %9 = mul nsw i32 %7, %8
  store i32 %9, i32* %5, align 4

  %10 = load i32, i32* %3, align 4
  %11 = load i32, i32* %4, align 4
  %12 = icmp ne i32 %10, %11
  br i1 %12, label %13, label %25

13:                                               ; preds = %0
  %14 = load i32, i32* %3, align 4
  %15 = icmp ne i32 %14, 0
  br i1 %15, label %16, label %19

16:                                               ; preds = %13
  %17 = load i32, i32* %4, align 4
  %18 = icmp ne i32 %17, 0
  br label %19

19:                                               ; preds = %16, %13
  %20 = phi i1 [false, %13], [%18, %16]
  %21 = zext i1 %20 to i32
  %22 = add nsw i32 %21, 4
  store i32 %22, i32* %6, align 4

  %23 = load i32, i32* %6, align 4
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i64 0, i64 0), i32 %23)
  br label %25

25:                                               ; preds = %19, %0
  ret i32 0
}

declare dso_local i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" }
