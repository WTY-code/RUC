#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
int main()
{
	char *p = NULL;
	char s1[] = "Volume",s2[10];
	p = s1;
	//cin>>s2;
	int n = strcmp(p,"Volumg");
	cout << n;
	return 0;
}
