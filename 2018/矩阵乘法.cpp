#include<iostream>
using namespace std;
int main()
{
	int n = 0,p = 0,m = 0;
	int a[10][10] = {{0}};
	int b[10][10] = {{0}};
	int c[10][10] = {{0}};
	cin >> n >> p >> m;
	for(int i = 1;i <= n;i++){
		for(int j = 1;j <= p;j++){
			cin >> a[i][j];
		}
	}
	for(int i = 1;i <= p;i++){
		for(int j = 1;j <= m;j++){
			cin>> b[i][j];
		}
	}
	//for(int i = 1;i <= n;i++){
	//	for(int j = 1;j <= m;j++){
	//		c[i][j] = {{0}};
	//	}
	//}
	for(int i = 1;i <= n;i++){
		for(int j = 1;j <= m;j++){
			for(int k = 1;k <= p;k++){
				c[i][j] = c[i][j] + a[i][k] * b[k][j]; 
			}
		}
	}
	for(int i = 1;i <= n;i++){
		for(int j = 1;j <= m;j++){
			cout << c[i][j]<<" ";
		}
	cout << endl;
	}
	return 0;
}
