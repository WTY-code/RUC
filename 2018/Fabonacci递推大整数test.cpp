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
	//scanf("%d",&n);
	len[1] = 1, len[2] = 1;
	printf("请输入第一项\n");
	gets(charf[1]);
	printf("请输入第二项\n");
	gets(charf[2]);
	printf("求第几项？\n");
	scanf("%d",&n);
	if((n==1)||(n==2)){
		printf("%c",charf[1]);
	}
	else{
		for(int j = 3; j <= n; j++){
			len[j-2] = strlen(charf[j-2]);//求n-2字符串长度 
			halflen[j-2] = floor(len[j-2]/2);//为翻转字符串做准备，无论奇数位还是偶数位向下取整对 
			len[j-1] = strlen(charf[j-1]);//求n-1字符串长度 
			halflen[j-1] = floor(len[j-1]/2);//同上 
			for(int i = 0; i < halflen[j-2]; i++){// 此循环用来翻转n-2字符串 
				char tmp = charf[j-2][i];
				charf[j-2][i] = charf[j-2][len[j-2]-1-i];
				charf[j-2][len[j-2]-1-i] = tmp;
			}
			for(int i = 0; i < halflen[j-1]; i++){//此循环用来翻转n-1字符串 
				char tmp = charf[j-1][i];
				charf[j-1][i] = charf[j-1][len[j-1]-1-i];
				charf[j-1][len[j-1]-1-i] = tmp;
			}
			for(int i = 0; i < len[j-2]; i++){//将n-2字符强制转换成整数 
				intf[j-2][i] = (int)charf[j-2][i];
				intf[j-2][i] = intf[j-2][i] - 48;
			}
			for(int i = 0; i < len[j-1]; i++){//将n-1字符强制转换成整数 
				intf[j-1][i] = (int)charf[j-1][i];
				intf[j-1][i] = intf[j-1][i] - 48;
			}
			for(int i = 0; i < len[j-1]; i++){//每一位数字相加来求n,此时倒序 
				intf[j][i] = intf[j-2][i] + intf[j-1][i] + enter[j][i];
				if(intf[j][i] >= 10){//判断是否需要进位 
					intf[j][i] = intf[j][i] - 10;//若需要进位，改位减10 
					enter[j][i+1]++;//若需要进位，下一位的进位数组加1 
				}
			}
			if(enter[j][len[j-1]] != 0){//判断前两项相加后是否多一位 
				len[j] = len[j-1] + 1;//若多一位要在下一位进位数组 加1 
				intf[j][len[j-1]] = enter[j][len[j-1]];//第n项的第一位为最后一个进位数组 
			}
			else{
				len[j] = len[j-1];//如果不进位，第n项与第n-1项的长度（位数）相同 
			}
			for(int i = 0; i < len[j]; i++){//通过ascii码将整数转换成字符 
				intf[j][i] = intf[j][i] + 48;
				charf[j][i] = (char)intf[j][i];
			}
			halflen[j] = floor(len[j]/2);
			for(int i = 0; i < halflen[j]; i++){//将算出的第n项翻转 
				int tmp = charf[j][i];
				charf[j][i] = charf[j][len[j]-1-i];
				charf[j][len[j]-1-i] = tmp;
			}
			for(int i = 0; i < halflen[j-2]; i++){// 此循环用来翻转n-2字符串 
				char tmp = charf[j-2][i];
				charf[j-2][i] = charf[j-2][len[j-2]-1-i];
				charf[j-2][len[j-2]-1-i] = tmp;
			}
			for(int i = 0; i < halflen[j-1]; i++){//此循环用来翻转n-1字符串 
				char tmp = charf[j-1][i];
				charf[j-1][i] = charf[j-1][len[j-1]-1-i];
				charf[j-1][len[j-1]-1-i] = tmp;
			}
		}
	}
	for(int i = 0; i < len[n]; i++){
		printf("%c",charf[n][i]);
	}
	return 0;
 } 
