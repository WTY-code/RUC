#include<iostream>
using namespace std;
int main()
{
	int n = 0,temp = 0;
	int a[1000] = {0};
	cin>>n;
	for(int i = 1;i <= n;i++){
		cin>>a[i];
	}
	for(int i = 1;i <= n;i++){
		int k = i;
		for(int j = i + 1;j <= n;j++){
			if(a[j] < a[k]){
				k = j;
			}
	    }
		if(k != i){
			temp = a[i];
			a[i] = a[k];
			a[k] = temp;
		}
	}
	for(int i = 1;i <= n;i++){
		cout<<a[i]<<" ";
	}
	return 0;
}
