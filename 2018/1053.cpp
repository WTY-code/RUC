#include<iostream>
using namespace std;
int main()
{
	int choice[10005] = {0};
	int n = 0, m = 0, ticket = 0, teamhead = 0;
	cin>>n>>m;
	for(int i = 0; i < m; i++){
		cin>>choice[i];
	}
	for(int i = 0; i < m; i++){
		int k = i; 
		for(int j = i+1; j < m; j++){
			if(choice[j]<choice[k]){
				k = j;
			}
		}
		if(k!=i){
			int tmp = choice[k];
			choice[k] = choice[i];
			choice[i] = tmp;
		}
	}
	for(int i = 0; i < m; i++){
		if(teamhead!=0) break;
		for(int j = i+1; j < m+1; j++){
			if(choice[i]==choice[j]){
				ticket++;
			}
			else {
					if((ticket+1)>(m/2)){
					teamhead = choice[i];
					cout<<teamhead;
					break;
				}
				else{
					i = i+ticket;
					ticket = 0;
				}
				break;
			}
		}
	}
	if(teamhead==0){
		cout<<"-1";
	}
	return 0;
 } 
