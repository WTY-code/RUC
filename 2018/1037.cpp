#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	int i = 0, j = 0, k = 0, l = 0;
	for(i = 65; i <= 68; i++){
		for(j = 65; j <= 68; j++){
			for(k = 65; k <= 68; k++){
				for(l = 65; l <= 68; l++){
					printf("%c%c%c%c\n",i,j,k,l);
				}
			}
		}
	}
	return 0;
}
