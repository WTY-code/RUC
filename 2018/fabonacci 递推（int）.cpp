#include<stdio.h>
#include<time.h>
#include<iostream>
using namespace std;
int main()
{
	clock_t t;
	int n = 0;
	int f[1000] = {0};
	scanf("%d",&n);
	if((n==1)||(n==2)){
	//	printf("1 ");
	    cout << "1";
	}
	if(n > 2){
	//	int f[1000] = {0};
		f[1] = 1;
		f[2] = 1;
		for(int i = 3; i <= n; i++){
			f[i] = f[i-1] + f[i-2];
		}
	}
	printf("%d\n",f[n]);
	t = clock();
	cout << t;
	return 0;
}
