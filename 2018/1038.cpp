#include<iostream>
using namespace std;
int m = 0, n = 0, x = 1, a = 0, b = 0;
int main()
{
	scanf("%d %d",&a,&b);
	do{
		m = x % a;
		n = x % b;
		x++;
	}while((m + n) != 0);
	printf("%d",x-1);
	return 0;
}

