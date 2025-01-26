#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int sum = add(3, 5);
    printf("The sum is: %d\n", sum);
    return 0;
}
