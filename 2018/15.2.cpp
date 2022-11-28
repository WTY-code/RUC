#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	char birth[10],major[100],s[200];
	int j=0,m=0,l=0;
	gets(s);
//	printf("%s",s);
	for(int i=0; i<200; i++){
		if(isdigit(s[i])){
			birth[j]=s[i];
			j++;
//			printf("%d %c\n",j,birth[j]);
			continue;
		}
		if(s[i]=='.'){
			m = i+13;
//			printf("s[%d] %c\n",m,s[m]);
			while(1){
				major[l] = s[m];
//				printf("major[%d] %c\n",l,major[l]);
				m++;
				l++;
				if(major[l-1]=='.'){
					major[l-1]='\0';
					break;
				}
			}
			break;
		}
	}
	printf("%s, %s\n",birth,major);
	return 0;
	
}
