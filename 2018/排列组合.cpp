#include<iostream>
using namespace std;
int c(int,int);
int main()
{
	int m,n;
	cin >> m >> n ;
	cout << c(m,n);
	return 0;
}
int c(int a,int b)
{
	if(a==b) return 1;
	if(b==1) return a;
	return c(a-1,b)+c(a-1,b-1);
}
