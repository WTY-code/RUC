#include<iostream>
using namespace std;
int main()
{
	int n = 0, m = 0, subject[15] = {0}, number[55] = {0}, total[55] = {0};
	int sub_get[55][15] = {{0}}, grade[55][15] = {{0}};
	cin >> n >> m;
	for(int i = 1; i <= m; i++){
		cin >> subject[i];
	}
	for(int i  = 1; i <= n; i++){
		cin >> number[i];
		for(int j = 1; j <= m; j++){
			cin >> sub_get[i][j];
		}
	}
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			if(sub_get[i][j] >= 60){
				grade[i][j] = subject[j];
			}
		}
	}
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			total[i] = total[i] + grade[i][j];
		}
	}
	for(int i = 1; i <= n; i++){
		int k = i;
		for(int j = i+1; j <= n; j++){
			if(total[j] > total[k]){
				k = j;
			}
		}
		if(k!= i){
			int tmp1 = total[i];
			total[i] = total[k];
			total[k] = tmp1;
			int tmp2 = number[i];
			number[i] = number[k];
			number[k] = tmp2;
		} 
	}
	for(int i = 1; i <= n; i++){
		int k = i;
		for(int j = i+1; j <= n; j++){
			if((total[j]==total[i])&&(number[j]<number[k])){
				k = j;
			}
		}
		if(k!=i){
			int tmp = number[k];
			number[k] = number[i];
			number[i] = tmp;
		}
	}
	for(int i = 1; i <= n; i++){
		cout << number[i] <<" "<< total[i] <<endl;
	}
	return 0;
}
