#include<iostream>
using namespace std;
int helper,n,num = 0;
bool c[13];
bool l[27];
bool r[27];
void Try(int i, int n)
{
	for(int j = 1; j <= n; j++){
		if((c[j]==true)&&(r[i+j]==true)&&(l[i-j+helper]==true)){
			c[j] = false;
			r[i+j] = false;
			l[i-j+helper] = false;
			if(i==n){
				num++;
		//		cout<<num<<endl;
			}
			else{
				Try(i+1,n);
			}
			c[j] = true;
			l[i-j+helper] = true;
			r[i+j] = true;
		}
	}
}
int main()
{
	cin>>n;
	helper = n+1;
	for(int i = 0; i <= 2*n+1; i++){
		r[i] = true;
		l[i] = true;
	}
	for(int i = 1; i <= n; i++){
		c[i] = true;
	}
	Try(1,n);
	cout<<num;
	return 0;
}
