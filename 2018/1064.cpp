#include<iostream>
#include<cstring>
using namespace std;
char ch[52][10002] = {0};
int a[52][10002] = {0}, len[52] = {0};
int pow(int x,int y){
	int s = 1;
	for(int i = 0; i < y; i++){
		s = s*x;
	}
	return s;
}
int main()
{
	int K = 0, tmp = 0, n = 0;
	cin>>K;
	cin>>n;
	for(int i = 0; i < n; i++){
		cin>>ch[i];
	}
	for(int i = 0; i < n; i++){
		len[i] = strlen(ch[i]);
		for(int j = 0; j < len[i]; j+=5){
			if((ch[i][j]=='0')&&(ch[i][j+1]=='0')&&(ch[i][j+2]=='0')
			&&(ch[i][j+3]=='0')&&ch[i][j+4]=='0'){
				cout<<" ";
			}
			else{
				if(ch[i][j]=='0'){
					ch[i][j] = '1';
					a[i][j] = ch[i][j]-48;
				}
				else{
					ch[i][j] = '0';
					a[i][j] = ch[i][j]-48;
				}
				if(ch[i][j+1]=='0'){
					ch[i][j+1] = '1';
					a[i][j+1] = ch[i][j+1]-48;
				}
				else{
					ch[i][j+1] = '0';
					a[i][j+1] = ch[i][j+1]-48;
				}
				if(ch[i][j+2]=='0'){
					ch[i][j+2] = '1';
					a[i][j+2] = ch[i][j+2]-48;
				}
				else{
					ch[i][j+2] = '0';
					a[i][j+2] = ch[i][j+2]-48;
				}
				if(ch[i][j+3]=='0'){
					ch[i][j+3] = '1';
					a[i][j+3] = ch[i][j+3]-48;
				}
				else{
					ch[i][j+3] = '0';
					a[i][j+3] = ch[i][j+3]-48;
				}
				if(ch[i][j+4]=='0'){
					ch[i][j+4] = '1';
					a[i][j+4] = ch[i][j+4]-48;
				}
				else{
					ch[i][j+4] = '0';
					a[i][j+4] = ch[i][j+4]-48;
				}
				tmp = a[i][j]*pow(2,4)+a[i][j+1]*pow(2,3)+a[i][j+2]*pow(2,2)+a[i][j+3]*2+a[i][j+4];
				for(int k = 65; k <= 90; k++){
					if(((k+K)%26)==tmp){
						cout<<(char)k;
						tmp = 0;
						break;
					}
				}
			}
		}
		cout<<endl;
	}
	return 0;
}
