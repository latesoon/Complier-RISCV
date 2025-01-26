#include <stdio.h>  
int a; 
const int b = 4;
  
int main() {  
    int c[3]; 
    int m = 2, n = 3;  
    int i = m * n; 
    if (m != n) { 
        int temp = (m && n) + b; 
        printf("out:%d\n", temp);  
    }  
    return 0; }
