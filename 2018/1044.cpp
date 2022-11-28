#include<iostream>
using namespace std;
int main()
{
	int light[10010] = {0}, n = 0, i = 0, j = 0, k = 0;
	cin >> n;
	//for(i = 1; i <= n; i++){
		for(j = 2; j <= n; j++){
			for(k = 1; j*k <= n; k++){
				if(light[j*k]==0){
					light[j*k] = 1;
				}
				else if(light[j*k] ==1){
					light[j*k] = 0;
				}
			}
		}
	//}
	for(int m = 1; m <= n; m++){
		if(light[m]==1){
			cout << m <<" ";
		}
	}
}
