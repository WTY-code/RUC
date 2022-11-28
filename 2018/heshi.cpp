#include<iostream>
using namespace std;
int fact(int n)
{
	if(n==1) return 1;
	else return n*fact(n-1);
}
int main()
{
	int n = 0,s = 0;
	cin>>n;
	for(int i = 1; i <= n; i++){
		s = s+fact(i);
	}
	cout<<s;
	return 0;
}
