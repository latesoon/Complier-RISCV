
declare i32 @printf(i8*, ...)
@.str = private unnamed_addr constant [16 x i8] c"The sum is: %d\0A\00", align 1
define i32 @add(i32 %0, i32 %1) #0 {
    %3 = alloca i32, align 4
    %4 = alloca i32, align 4
    store i32 %0, i32* %3, align 4
    store i32 %1, i32* %4, align 4
    %5 = load i32, i32* %3, align 4
    %6 = load i32, i32* %4, align 4
    %7 = add nsw i32 %5, %6
    ret i32 %7
}


define i32 @main() #0 {
   
    %1 = alloca i32, align  4
    %2 = call i32 @add(i32 3, i32 5)
    store i32 %2, i32* %1, align 4
    %3 = load i32, i32* %1, align 4
    %4 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([16 x i8], [16 x i8]* @.str, i64 0, i64 0), i32 noundef %3)
    ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable }
