#include<iostream>
#include<cmath>
using namespace std;
int pow(int,int);
int main()
{
	double x = 0.0, e = 0.0, sin = 0.0, cos = 0.0;
	double s[100] = {0.0}, c[100] = {0.0};
	int j = 0, i = 0;
	cin >> x >> e;
	x = fmod(x,6.283185);
	s[1] = x;
	c[1] = 1.0;
	i = 2;
	do{
		s[i] = s[i-1] * ((x * x)/(2*(i-1)*(2*i-1)));
		i++;
	}while(s[i-1] >= e);
	i = 2;
	do{
		c[i] = c[i-1]*((x*x)/((2*i-3)*(2*i-2)));
		i++;
	}while(c[i-1] >= e);
	j = 1;
	do{
		sin = sin + pow(-1,j-1) * s[j];
		j++;
	}while(s[j-1] != 0);
	j = 1;
	do{
		cos = cos + pow(-1,j-1) * c[j];
		j++;
	}while(c[j-1] != 0);
	cout << sin << endl << cos;
	return 0;
}
int pow(int x,int y){
	int s = 1;
	for(int i = 1; i <= y; i++){
		s = s*x;
	}
	return s;
}
