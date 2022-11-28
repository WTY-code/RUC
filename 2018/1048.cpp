#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	int n = 0, s = 0, a[5010] = {0}, min[5010] = {0};
	cin >> n;
	for(int i = 1; i <= n; i++){
		cin >> a[i];
	}
	s = a[1];
	for(int i = 2; i<= n; i++){
		min[i] = abs(a[i]-a[i-1]);
		for(int j = i-1; j >= 1; j--){
			if(abs(a[i]-a[j]) < min[i]){
				min[i] = abs(a[i]-a[j]);
			}
		}
	}
	for(int i = 2; i <= n; i++){
		s = s + min[i];
	}
	cout << s;
	
	return 0;
}
