#include<iostream>
using namespace std;
int a[10][10] = {{0}};//1 base 
int m = 0, n = 0, g = 0;
void mi(int i,int j)
{
	if((i==n)&&(j==m)){
		g++;
		return;
	}
	else{
//		cout<<"here"<<endl;
		if(a[i][j+1]){
			cout<<i<<" "<<j<<endl;
			a[i][j] = 0;
			mi(i,j+1);
		//	a[i][j] = 1;
		}
		if(a[i+1][j]){
			cout<<i<<" "<<j<<endl;
			a[i][j] = 0;
			mi(i+1,j);
		//	a[i][j] = 1;
		}
		if(a[i-1][j]){
			cout<<i<<" "<<j<<endl;
			a[i][j] = 0;
			mi(i-1,j);
		//	a[i][j] = 1;
		}
		if(a[i][j-1]){
			cout<<i<<" "<<j<<endl;
			a[i][j] = 0;
			mi(i,j-1);
		//	a[i][j] = 1;
		}
	}
}
int main()
{
	cin>>n>>m;
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			cin>>a[i][j];
		}
	}
	mi(1,1);
	if(g!=0) cout<<"YES";
	
	else cout<<"NO";
	return 0;
}
