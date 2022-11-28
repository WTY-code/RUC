#include<iostream>
#include<cstring>
#include<cstdlib>
using namespace std;
struct answer
{
	char letter;
	int a;
}ans[125];
int main()
{
	char s[255];
	int len = 0;
	gets(s);
	len = strlen(s);
	for(int i = 0; i < len; i++){
		for(int j = 65; j <= 90; j++){
			if(s[i]==j){
				ans[j+32].a ++;
				ans[j+32].letter = j+32;
				break;
			}
		}
			for(int j = 97; j <= 122; j++){
				if(s[i]==j){
					ans[j].a ++;
					ans[j].letter = j;
					break;
				}
			}
	}
	for(int i = 97; i <= 122; i++){
		int k = i;
		for(int j = i+1; j <= 122; j++){
			if(ans[j].a > ans[k].a ){
				k = j;
			}
		}
		if(k!=i){
			swap(ans[i].a,ans[k].a);
			swap(ans[i].letter,ans[k].letter);
		}
	}
	for(int i = 97; i <= 122; i++){
		int k = i;
		for(int j = i+1; j <= 122; j++){
			if((ans[j].a == ans[k].a )&&(ans[j].letter < ans[k].letter )){
				k = j;
			}
		}
		if(k!=i){
			swap(ans[i].letter,ans[k].letter);
		}
	}
	for(int i = 0; i <= 122; i++){
		if(ans[i].a!=0){
	        cout<<ans[i].letter<<" "<<ans[i].a <<endl;
	    }
	}
	return 0;
 } 
