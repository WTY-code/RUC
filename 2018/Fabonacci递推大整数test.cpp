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
	printf("�������һ��\n");
	gets(charf[1]);
	printf("������ڶ���\n");
	gets(charf[2]);
	printf("��ڼ��\n");
	scanf("%d",&n);
	if((n==1)||(n==2)){
		printf("%c",charf[1]);
	}
	else{
		for(int j = 3; j <= n; j++){
			len[j-2] = strlen(charf[j-2]);//��n-2�ַ������� 
			halflen[j-2] = floor(len[j-2]/2);//Ϊ��ת�ַ�����׼������������λ����ż��λ����ȡ���� 
			len[j-1] = strlen(charf[j-1]);//��n-1�ַ������� 
			halflen[j-1] = floor(len[j-1]/2);//ͬ�� 
			for(int i = 0; i < halflen[j-2]; i++){// ��ѭ��������תn-2�ַ��� 
				char tmp = charf[j-2][i];
				charf[j-2][i] = charf[j-2][len[j-2]-1-i];
				charf[j-2][len[j-2]-1-i] = tmp;
			}
			for(int i = 0; i < halflen[j-1]; i++){//��ѭ��������תn-1�ַ��� 
				char tmp = charf[j-1][i];
				charf[j-1][i] = charf[j-1][len[j-1]-1-i];
				charf[j-1][len[j-1]-1-i] = tmp;
			}
			for(int i = 0; i < len[j-2]; i++){//��n-2�ַ�ǿ��ת�������� 
				intf[j-2][i] = (int)charf[j-2][i];
				intf[j-2][i] = intf[j-2][i] - 48;
			}
			for(int i = 0; i < len[j-1]; i++){//��n-1�ַ�ǿ��ת�������� 
				intf[j-1][i] = (int)charf[j-1][i];
				intf[j-1][i] = intf[j-1][i] - 48;
			}
			for(int i = 0; i < len[j-1]; i++){//ÿһλ�����������n,��ʱ���� 
				intf[j][i] = intf[j-2][i] + intf[j-1][i] + enter[j][i];
				if(intf[j][i] >= 10){//�ж��Ƿ���Ҫ��λ 
					intf[j][i] = intf[j][i] - 10;//����Ҫ��λ����λ��10 
					enter[j][i+1]++;//����Ҫ��λ����һλ�Ľ�λ�����1 
				}
			}
			if(enter[j][len[j-1]] != 0){//�ж�ǰ������Ӻ��Ƿ��һλ 
				len[j] = len[j-1] + 1;//����һλҪ����һλ��λ���� ��1 
				intf[j][len[j-1]] = enter[j][len[j-1]];//��n��ĵ�һλΪ���һ����λ���� 
			}
			else{
				len[j] = len[j-1];//�������λ����n�����n-1��ĳ��ȣ�λ������ͬ 
			}
			for(int i = 0; i < len[j]; i++){//ͨ��ascii�뽫����ת�����ַ� 
				intf[j][i] = intf[j][i] + 48;
				charf[j][i] = (char)intf[j][i];
			}
			halflen[j] = floor(len[j]/2);
			for(int i = 0; i < halflen[j]; i++){//������ĵ�n�ת 
				int tmp = charf[j][i];
				charf[j][i] = charf[j][len[j]-1-i];
				charf[j][len[j]-1-i] = tmp;
			}
			for(int i = 0; i < halflen[j-2]; i++){// ��ѭ��������תn-2�ַ��� 
				char tmp = charf[j-2][i];
				charf[j-2][i] = charf[j-2][len[j-2]-1-i];
				charf[j-2][len[j-2]-1-i] = tmp;
			}
			for(int i = 0; i < halflen[j-1]; i++){//��ѭ��������תn-1�ַ��� 
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
