#include<iostream>
using namespace std;
int main()
{
	int n = 0,m = 0;
	int a[100][100]={{0}};
	cin >> n >> m;
	for(int i = 1;i <= n;i++){
		for(int j = 1;j <= m;j++){
			cin >> a[i][j];
		}
	}
	for(int i = 1;i <= m;i++){
		for(int j = 1;j <= n;j++){
			cout << a[j][i] <<" ";
		}
		cout << endl;
	}
	return 0;
}
