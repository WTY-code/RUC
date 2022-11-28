#include<iostream>
using namespace std;
#include<cmath>
int f(int x,int y)
{
	int s=1;
	for(int i=1;i<=y;i++)
	    s=s*x;
	return s;
}
int main()
{
	int a,b,y;
	cin>>a>>b;
	y=f(a,b);
	cout<<y;
	return 0;
}
