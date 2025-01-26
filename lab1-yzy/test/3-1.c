#include<stdio.h>
float pi=0;
int main(){
	float i=1;
	int t;
	scanf("%d",&t);
	for(int a=0;a<t;a++){
		if(a%2) pi-=1/i;
		else pi+=1/i;
		i+=2;
	}
	pi*=4;
	printf("pi=%.5f\n",pi);
	return 0;
}
