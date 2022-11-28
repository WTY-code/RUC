//ÏÂ±êË÷Òý 
#include<iostream>
#include<cmath>
#include<ctime>
#define m 10000
using namespace std;
int main()
{
	clock_t t;
	int person_thing[21][102] = {{0}}, thing_positive[m] = {0}, thing_negative[m] = {0};
	int thing_quantity[21] = {0}, n = 0, g = 0;
	cin >> n;
	for(int i = 0; i < n; i++){
		cin >> thing_quantity[i];
		for(int j = 0; j < thing_quantity[i]; j++){
			cin >> person_thing[i][j];
		}
	}
	for(int i = 0; i < n; i++){
		for(int j = 0; j < thing_quantity[i]; j++){
			if(person_thing[i] >= 0){
				thing_positive[person_thing[i][j]]++;
			}
			else{
				person_thing[i][j] = abs(person_thing[i][j]);
				thing_negative[person_thing[i][j]]++;
			}
		}
	}
	for(int i = 0; i < m; i++){
		if(thing_positive[i]==n){
			cout<<i<<" ";
			g++;
		}
		if(thing_negative[i]==n){
			cout<<"-"<<i<<" ";
			g++;
		}
	}
	if(g==0) cout<<"NO";
	t = clock();
	cout <<t;
	return 0;
 } 
