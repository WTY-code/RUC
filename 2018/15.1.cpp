#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	double t[201] = {0};
	int n = 0;
	double max = 0.0,min = 0.0,sum = 0.0,general = 0.0;
	cin>>n;
	for(int i=0; i<n; i++){
		cin>>t[i];
	}
	max = t[0];
	for(int i=1; i<n; i++){
		if(t[i]>max){
			max = t[i];
		}
	}
	min = t[0];
	for(int i=1; i<n; i++){
		if(t[i]<min){
			min = t[i];
		}
	}
	for(int i=0; i<n; i++){
		sum = sum+t[i];
	}
	general = sum/n;
	printf("%.0f %.0f %.0f",max,min,general);
	return 0;
}
