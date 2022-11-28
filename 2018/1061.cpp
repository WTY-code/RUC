#include<iostream>
#include<stdio.h>
#include<cmath>
#include<cstring>
using namespace std;
int main()
{
	char ch[5] = {0};
	int a[4] = {0};
	for(int i = 0; i < 4; i++){
		scanf("%c",&ch[i]);
	}
	for(int i = 0; i < 4; i++){
		a[i] = ch[i] - 48;
	}
	for(int i = 0; i < 4; i++){
		a[i] = a[i] + 5;
		a[i] = a[i]%10;
	}
	swap(a[0],a[3]);
	swap(a[1],a[2]);
	if(a[0]==0){
		printf("%d%d%d",a[1],a[2],a[3]);
	}
	else{
		printf("%d%d%d%d",a[0],a[1],a[2],a[3]);
	}
	return 0;
}
