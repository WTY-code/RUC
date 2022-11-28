#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	int d = 0, h = 0, i = 1;
	int s[100] = {0},y[100] = {0};
	cin>>d>>h;
	s[0] = (int)d/h;
//	cout<<s[0]<<endl;
	y[0] = d%h;
//	cout<<y[0]<<endl;
	while(1){
//		cout<<i<<" ";
		s[i] = (int)s[i-1]/h;
//		cout<<s[i]<<" ";
		y[i] = s[i-1]%h;
//		cout<<y[i]<<endl;
		if(s[i]==0){
			for(int j=i; j>=0; j--){
				if((y[j]==0)&&(j==i)) continue;
				if(y[j]<=9) cout<<y[j];
				else if(y[j]==10) cout<<'A';
				else if(y[j]==11) cout<<'B';
				else if(y[j]==12) cout<<'C';
				else if(y[j]==13) cout<<'D';
				else if(y[j]==14) cout<<'E';
				else if(y[j]==15) cout<<'F';
			}
			break;
		}
		i++;
	}
	return 0;
}
