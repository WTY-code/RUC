#include<iostream>
using namespace std;
int main()
{
	int a[110] = {0}, b[110] = {0}, money[110] = {0}, s = 0, n = 0;
	cin >> n;
	for(int i = 0; i < n; i++){
		cin >> a[i] >> b[i];
	}
	for(int i = 0; i < n; i++){
		if(a[i] == 0){
			if((b[i] >= 12) && (b[i] <= 16)){
				money[i] = 8;
			}
			else if((b[i] >= 10) && (b[i] <= 20)){
				money[i] = 10;
			}
			else{
				money[i] = 0;
			}
		}
		if(a[i] == 1){
			if((b[i] >= 10) && (b[i] <= 20)){
				money[i] = 30;
			}
			else{
				money[i] = 0;
			}
		}
	}
	for(int i = 0; i < n; i++){
		s = s + money[i];
	} 
	cout << s;
}
