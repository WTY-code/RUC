#include<iostream>
using namespace std;
const int helper = 9;
bool C[9];
bool R[17],L[17];
int q[9],n = 0;
void Try(int i)
{
	for(int j = 1; j <= 8; j++){
		if((C[j]==true)&&(R[i+j]==true)&&(L[i-j+9]==true)){
			q[i] = j;
			C[j] = false;
			R[i+j] = false;
			L[i-j+helper] = false;
			if(i==8){
				n++;
				cout<<"第 "<<n<<" 种方案是 \n";
				for(int k = 1; k <= 8; k++){
					cout<<"第 "<<k<<" 行 "<<q[k]<<endl; 
				} 
			}
			else{
				Try(i+1);
			}
			q[i] = 0;
			C[j] = true;
			R[i+j] = true;
			L[i-j+helper] = true;
		}
	}
}
int main()
{
	q[9] = {0};
	for(int i = 0; i <= 17; i++){
		R[i] = true;
		L[i] = true;
	}
	for(int i = 0; i <= 8; i++){
		C[i] = true;
	}
	Try(1);
	return 0;
}
