#include<iostream>
using namespace std;
int main()
{
	int a = 0, b = 0, c = 0, i = 0, j = 0, k = 0, g = 0;
	cin >> a >> b >> c;
	for(i = -100; i <= 100; i++){
		for(j = -100; j <= 100; j++){
			for(k = -100; k <= 100; k++){
				if(((a*i + b*j + c*k)==0)&&((i!=0)||(j!=0)||(k!=0))){
					cout << i <<" "<< j <<" "<< k <<endl;
					g = g + 1; 
				}
			}
		}
	}
	if(g==0){
		cout <<"no solution";
	}
	return 0;
}
