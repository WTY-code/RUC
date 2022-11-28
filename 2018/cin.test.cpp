#include<iostream>
#include<cstring>
using namespace std;
int main()
{
	int n = 0, len[10] = {0};
	char s[10][16] = {0};
	cin>>n;
	for(int i = 0; i < n; i++){
		cin>>s[i];
	}
	for(int i = 0; i < n; i++){
		cout<<s[i]<<endl;
	}
	for(int i = 0; i < n; i++){
		len[i] = strlen(s[i]);
	}
	for(int i = 0; i < n; i++){
		for(int j = 0; j < len[i]; j+=5){
			if(s[i][j]=='0'){
				s[i][j]='1';
			}
			else{
				s[i][j]='0';
			}
		}
	}
	for(int i = 0; i < n; i++){
		cout<<s[i]<<endl;
	}
	return 0;
}
