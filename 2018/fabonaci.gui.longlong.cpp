#include<stdio.h>//?ݹ??棬int?????㵽46?1836311903 
#include<iostream>
#include<time.h>
using namespace std;
long long f(int n)
{
	if((n == 1)||(n == 2)){
		return 1;
	}
	if(n > 2){
		return f(n-1) + f(n-2);
	}
}
int main()
{
	clock_t t;
	int m = 0;
	long long y = 0;
	scanf("%d", &m);
	y = f(m);
	printf("%lld\n",y);
	t = clock();
	cout << t;
	return 0;
}
