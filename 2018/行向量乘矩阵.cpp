#include<iostream>
using namespace std;
int main()
{
	int n = 0,m = 0;
	int a[25][25] = {{0}};
	int b[25] = {0};
	int c[25] = {0};
	cin >> n >> m;
	for(int i = 1;i <= n;i++){
		cin >> b[i];
	}
	for(int i = 1;i <= n;i++){
		for(int j = 1;j <= m;j++){
			cin >> a[i][j];
		}
	}
	for(int j = 1;j <= m;j++){
		for(int k = 1;k <= n;k++){
			c[j] = c[j] + b[k] * a[k][j];
		}
	}
	for(int i = 1;i <= m;i++){
		cout << c[i] <<" ";
	}
	return 0;
}
