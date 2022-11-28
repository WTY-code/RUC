#include<iostream>
using namespace std;
int like[5][5] = {{0,0,1,1,0},{1,1,0,0,1},{0,1,1,0,1},{0,0,0,1,0},{0,1,0,0,1}};
int book[5] = {0}, take[5];
int n;
void Try(int i);
int main()
{
	n = 0;
	Try(0);
	return 0;
}
void Try(int i)
{
	int j = 0, k = 0;
	for(j = 0; j <= 4; j++){
		if((like[i][j] > 0) && (book[j] == 0)){
			take[i] = j;
			book[j] = 1;
			if(i == 4){
				n++;
				cout << "NO. "<< n <<" "<<endl;
			    for(k = 0; k <= 4; k++){
			        cout <<" give book "<< take[k] <<" to "<< char(65+k) << endl; 
                }
                cout << endl;
			}
			else{
				Try(i+1);
			}
			book[j] = 0;
			take[i] = -1;
		}
	}
}
