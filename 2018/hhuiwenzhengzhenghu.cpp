#include<iostream>
#include<stdio.h>
#include<cstdlib>
#include<cstring>
#define m 200
#define max 100000000004
using namespace std;
int main()
{
	long long i = 0;
	int k = 0,count = 0,len = 0;
	char s[m],ss[m];
	cin>>k;
//	cout<<k<<endl;
	for(i=1; i<max; i++){
		itoa(i,s,10);
//		printf("i is %d\ns is %s\n",i,s);
		len = strlen(s);
//		printf("len is %d\n",len);
		for(int j=len-1; j>=0; j--){
			ss[len-1-j] = s[j];
//			printf("s[%d] is %c\n",j,s[j]);
//			printf("ss[%d] is %c\n",len-1-j,ss[len-1-j]);
		}
		if(strcmp(ss,s)==0) count++;
		if(count==k){
			cout<<i;
			break;
		}
		memset(s,'\0',sizeof(s));
		memset(ss,'\0',sizeof(ss));
		len = 0;
	}
	return 0;
}
