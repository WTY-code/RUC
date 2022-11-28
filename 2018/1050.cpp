#include<iostream>
#include<cstring>
using namespace std;
int main()
{
	char s[110], letter[110];
	char *p = NULL;
	int len = 0, a[15] = {0};
	p = letter;
	for(int i = 0; i < 110; i++){
		cin>>s[i];
		if(s[i]=='#')break;
	}
	len = strlen(s);
	for(int i = 0; i < len; i++){
		for(int j = 48; j <= 57; j++){
			if(s[i]==j){
				a[j-48]++;
				cout<<s[i];
			} 
		}
	}
	cout<<endl;
	for(int i = 0; i < len-1; i++){
		if((s[i]<48)||(s[i]>57)){
			cout<<s[i];
			*p = s[i];
			p++;
		}
	}
	cout<<endl;
	for(int j = 0; j <= 9; j++){
		if(a[j]!=0){
			cout << j <<" "<< a[j] <<endl;
		}
	}
	return 0;
}
