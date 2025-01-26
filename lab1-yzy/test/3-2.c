#include<stdio.h>
int nums[10];
void sort(int num){
	for(int i=0;i<num;i++){
		for(int j=i+1;j<num;j++){
			if(nums[i]<=nums[j])
				continue;
			int temp=nums[i];
			nums[i]=nums[j];
			nums[j]=temp;
		}
	}
}
int main(){
	int t;
	scanf("%d",&t);
	for(int i=0;i<t;i++)
		scanf("%d",&nums[i]);
	sort(t);
	for(int i=0;i<t;i++)
		printf("%d ",nums[i]);
	printf("\n");
	return 0;
}
