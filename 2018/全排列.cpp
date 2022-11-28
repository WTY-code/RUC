#include<iostream>
using namespace std;
int n = 0,total = 0;
void perm(int ary[], int used[], int k)
{
	for(int i = 1; i <= n; i++){
		if(!used[i]){
			ary[k] = i;
			//cout<<"i is "<<i<<" k is "<<k<<" ary[k] is "<<ary[k]<<endl; 
			used[i] = 1;
			//cout<<"used["<<i<<"] is "<<used[i];
			if(k==n){
				for(int j = 1; j <= n; j++){
					cout <<ary[j];
					total++;
				}
				cout<<endl;
			}
			else{
				perm(ary,used,k+1);
			}
			used[i] = 0;
		}
	}
	return;
}
int main()
{
	int arry[10] = {0};
	int used[10] = {0};
	cin>>n;
	perm(arry,used,1);
	return 0;
 } 
