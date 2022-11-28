#include<iostream>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
using namespace std;
char charf[1010][250] = {{0}};
int len[1010] = {0}, halflen[1010] = {0};
int intf[1010][250] = {{0}}, enter[1010][250] = {{0}};
int main()
{
	int n = 0;
	scanf("%d",&n);
	len[1] = 1, len[2] = 1;
	printf("请输入第一项\n");
	gets(charf[1]);
	printf("请输入第二项\n");
	gets(charf[2]);
	if((n==1)||(n==2)){
		printf("1");
	}
	else{
		for(int j = 3; j <= n; j++){
			len[j-2] = strlen(charf[j-2]);
			halflen[j-2] = floor(len[j-2]/2);
			len[j-1] = strlen(charf[j-1]);
			halflen[j-1] = floor(len[j-1]/2);
			for(int i = 0; i < halflen[j-2]; i++){
				char tmp = charf[j-2][i];
				charf[j-2][i] = charf[j-2][len[j-2]-1-i];
				charf[j-2][len[j-2]-1-i] = tmp;
			}
			for(int i = 0; i < halflen[j-1]; i++){
				char tmp = charf[j-1][i];
				charf[j-1][i] = charf[j-1][len[j-1]-1-i];
				charf[j-1][len[j-1]-1-i] = tmp;
			}
			for(int i = 0; i < len[j-2]; i++){
				intf[j-2][i] = atoi(charf[j-2][i]);
			}
			for(int i = 0; i < len[j-1]; i++){
				intf[j-1][i] = atoi(charf[j-1][i]);
			}
			for(int i = 0; i < len[j-1]; i++){
				intf[j][i] = intf[j-2][i] + intf[j-1][i] + enter[j][i];
				if(intf[j][i] >= 10){
					intf[j][i] = intf[j][i] - 10;
					enter[j][i+1]++;
				}
			}
			if(enter[j][len[j-1]] != 0){
				len[j] = len[j-1] + 1;
				intf[j][len[j-1]] = enter[j][len[j-1]];
			}
			else{
				len[j] = len[j-1];
			}
			for(int i = 0; i < len[j]; i++){
				itoa(intf[j][i], charf[j][i], 10);
			}
			halflen[j] = floor(len[j]/2);
			for(int i = 0; i < halflen[j]; i++){
				int tmp = charf[j][i];
				charf[j][i] = char[j][len[j]-1-i];
				charf[j][len[j]-1-i] = tmp;
			}
		}
	}
	for(int i = 0; i < len[n]; i++){
		printf("%c",charf[n][i]);
	}
	return 0;
 } 
