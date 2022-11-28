#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	int n = 0, m = 0, p1 = 0, p2 = 0, s1 = 0, s2 = 0, least;
	int xuan = 0, kai = 0, c[100] = {0}, distance[100] = {0};
	cin>>n;
	for(int i = 1; i <= n; i++){
		cin>>c[i];
	}
	cin>>m>>p1>>s1>>s2;
	c[p1] = c[p1]+s1;
	for(p2 = 1; p2 <= n; p2++){
		c[p2] = c[p2]+s2;
		xuan = 0;
		kai = 0;
		for(int i = 1; i < m; i++){
			xuan = xuan+c[i]*(m-i);
		}
		for(int j = m+1; j <= n; j++){
			kai = kai+c[j]*(j-m);
		}
		distance[p2] = abs(xuan-kai);
		c[p2] = c[p2]-s2;
	}
	least = distance[1];
	p2  = 1;
	for(int i = 1; i <= n; i++){
		if(distance[i] < least){
			least = distance[i];
			p2 = i;
		}
	}
	cout << p2;
	return 0;
}
