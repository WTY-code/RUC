#include<iostream>
using namespace std;
int n = 0;
int like[5][5] = {{0,0,1,1,0},{1,1,0,0,1},{0,1,1,0,1},{0,0,0,1,0},{0,1,0,0,1}};
int take[5] = {-1,-1,-1,-1,-1};
int book[5] = {0};
void Try(int i)
{
	int j,k;
	for(j = 0; j <= 4; j++){
		if((book[j]==0)&&(like[i][j]==1)){
			take[i] = j;
			book[j] = 1;
			if(i==4){
				n++;
				cout<<"N0."<<n<<" ";
				for(k = 0; k <= 4; k++){
					cout<<"person "<<char(k+65)<<" "<<"book "<<take[k];
				}
				cout<<endl;
			}
			else Try(i+1);
			book[j] = 0;
			take[i] = -1;
		}
	}
}
int main()
{
	Try(0);
	return 0;
}
