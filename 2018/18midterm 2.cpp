#include<iostream>
#include<cmath>
using namespace std;
int f(int,int,int,int,int,int);
int main()
{
	int a[10000] = {0}, b[10000] = {0}, c[10000] = {0}, d[10000] = {0};
	int e[10000] = {0}, x[10000] = {0}, ans1[10000] = {0}, ans2[10000] = {0};
	int n = 0, k = 0, g1 = 0, g2 = 0, score1 = 0, score2 = 0;
	cin >> n >> k;
	for(int i = 1; i <= n; i++){
		cin >> a[i] >> b[i] >> c[i] >> d[i] >> e[i] >> x[i] >> ans1[i] >> ans2[i];
	}
	for(int i = 1; i <= n; i++){
		if(ans1[i] == f(a[i], b[i], c[i] ,d[i] , e[i], x[i])){
			g1 = g1 + 1;
			score1 = score1 + 1;
		}
		else{
			g1 = 0;
		}
		if(g1 >= k){
			score1 = score1 + g1 - k;
		}
		if(ans2[i] == f(a[i], b[i], c[i] ,d[i] , e[i], x[i])){
			g2 = g2 + 1;
			score2 = score2 + 1;
		}
		else{
			g2 = 0;
		}
		if(g2 >= k){
			score2 = score2 + g2 - k;
		}
	}
	cout << score1 <<" "<< score2;
	return 0;
 }
 int f(int a, int b, int c, int d, int e, int x){
 	return a * x * x * x * x + b * x * x * x + c * x * x + d * x + e;
 } 
