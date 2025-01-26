#include<stdio.h>
float c=3.88;
int main(){
	int a[3],b=2;
	a[2]=1;
	for(int i=0;i<15;i++){
		if(c==1)break;
		if(a[2]==10)continue;
		a[2]++;
		c+=c/b;
	}
	printf("a[2]=%d c=%f",a[2],c)
	return 0;
}
	
