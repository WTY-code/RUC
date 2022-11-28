#include<iostream>
using namespace std;
int main()
{
	int n = 0;
	int a[25][110] = {{0}}, b[2100] = {0}, nn[25] = {0};
	cin >> n;
	for(int i = 1; i <= n; i++){
		cin >> nn[i];
		for(int j = 1; j <= nn[i]; j++){
			cin >> a[i][j];
		}
	}
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= nn[i]; j++){
			b[a[i][j]]++;
		}
	}
	for(int i = 1; i <= 2050; i++){
		if(b[i] >= n) cout << i <<" ";
	}
}
