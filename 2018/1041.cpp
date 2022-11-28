#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	int h = 0, l = 0, n = 0;
	int a[210] = {0};
	float b[210] = {0}, c[210] = {0}, d[210] = {0}, t[210] = {0};
	cin >> n;
	for(int i = 0; i < n; i++){
		cin >> a[i] >> b[i] >> c[i] >> d[i];
		t[i] = 0.2*b[i] + 0.1*c[i] + 0.7*d[i];
		t[i] = round(t[i]);
	}
	for(int i = 0; i < n; i++){
		if(t[i] >= 90){
			h = h + 1;
		}
		if(t[i] < 60){
			l = l + 1;
		}
	}
	for(int i = 0; i < n; i++){
		cout << a[i] <<" "<< int(t[i])<<endl;
	}
	cout << h << endl << l;
	return 0;
}
