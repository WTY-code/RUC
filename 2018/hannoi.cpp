#include<iostream>
using namespace std;
int count = 1;
void move(int n, char a, char b,char c)
{
	if(n==1){
		cout<<"[step"<<count<<"] move plate "<<"1# from "<<a<<" to "<<c<<endl;
		count++;
	}
	else{
		move(n-1,a,c,b);
		cout<<"[step"<<count<<"] move plate "<<n<<"# from "<<a<<" to "<<c<<endl;
		count++;
		move(n-1,b,a,c);
	}
}
int main()
{
	int n = 0;
	cin>>n;
	move(n,'a','b','c');
	cout<<count-1;
	return 0;
}
