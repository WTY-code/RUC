#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	int a[26][26] = {{0}};
	int n = 0, m = 0;
	cin >> n >> m;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			if(j >= i){
				a[i][j] = 65 + (j-i);
			}
			else{
				a[i][j] = 65 + (i-j);
			}
		}
	}
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			printf("%c",a[i][j]);
		}
		cout << endl;
	}
	return 0;
}
