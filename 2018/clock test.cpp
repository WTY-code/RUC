#include<iostream>
#include<cmath>
#include<ctime>
using namespace std;
int main()
{
	clock_t start, end;
	start = clock();
	int t;
	int g = 0;
	for(int i = 1; i <= 1000; i++){
		for(int j = 1; j <= 1000; j++){
			g = g+1;
		}
	}
	end = clock();
	cout << g <<" "<< end-start;
 } 
