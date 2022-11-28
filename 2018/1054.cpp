#include<iostream>
using namespace std;
int main()
{
	int x = 0, y = 0, z = 0, n = 0, g = 0;
	cin>>n;
	//for(x = 0; x <= 10*n; x++){
	for(y = 0; y <= 5*n; y++){
		for(z = 0; z <= 2*n; z++){
			if((10*n-2*y-5*z)>=0){
				g++;
				cout<<10*n-5*y-2*z<<" "<<y<<" "<<z<<endl;
				}
			}
		}
	//}
	cout<<g;
	return 0;
}
