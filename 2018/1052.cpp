#include<iostream>
using namespace std;
int jishu(int);
int sushu(int);
int main()
{
	int a = 0, n = 0, b = 0;
	cin>>n;
	for(int a = 2; a <= 1000; a++){
		if((jishu(a)+sushu(a))==2){
			b = n-a;
			{
				if((jishu(b)+sushu(b))==2){
					cout<<n<<"="<<a<<"+"<<b<<endl;
					break;
				}
			}
		}
	}
}
int jishu(int x)
{
	if((x%2)!= 0)return 1;
	else return 0;
}
int sushu(int y)
{
	int m = 0, g = 0;
	for(int i=2;i<y;i++)    
            {    
                m=y%i;    
                if(m==0)    
                    g=g+1;    
            }    
        if(g!=0) return 0;
		else return 1; 
}
