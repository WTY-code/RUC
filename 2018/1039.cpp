#include<iostream>
#include<stdio.h>
int main()
{
	int a = 0, b = 0, g = 0;
	scanf("%d %d",&a,&b);
	for(int i = a; i <= b; i++){
		if(((i % 400)==0)||(((i % 4) == 0) && ((i % 100) != 0))){
			g = g + 1;
		}
	}
	printf("%d",g);
}
