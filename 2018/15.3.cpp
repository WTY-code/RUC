#include<iostream>
using namespace std;
struct Big{
	int i;
	int j;
	int value;
}big[100];
int main()
{
	int a[101][101] = {{0}},b[101][101]={{0}};
	int m=0, n=0, max=0, g=0, l=0;
	cin>>m>>n;
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			cin>>a[i][j];
		}
	}
	b[0][0] = a[0][0]+a[0][1]+a[1][0];
	for(int j=1; j<n-1; j++){
		b[0][j] = a[0][j]+a[0][j-1]+a[1][j]+a[0][j+1];//zuo xia you
	}
	b[0][n-1] = a[0][n-1]+a[0][n-2]+a[1][n-1];//zuo xia
	for(int i=1; i<m-1; i++){
		b[i][0] = a[i][0]+a[i-1][0]+a[i][1]+a[i+1][0];//shang you xia
		for(int j=1; j<n-1; j++){
			b[i][j] = a[i][j]+a[i-1][j]+a[i+1][j]+a[i][j-1]+a[i][j+1];
		}
		b[i][n-1] = a[i][n-1]+a[i-1][n-1]+a[i][n-2]+a[i+1][n-1];
	}
	b[m-1][0] = a[m-1][0]+a[m-2][0]+a[m-1][1];
	for(int j=1; j<n-1; j++){
		b[m-1][j] = a[m-1][j]+a[m-1][j-1]+a[m-2][j]+a[m-1][j+1];
	}
	b[m-1][n-1] = a[m-1][n-1]+a[m-1][n-2]+a[m-2][n-1];
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			if(b[i][j]>max){
				max = b[i][j];
			}
		}
	
    }
    for(int i=0; i<m; i++){
    	for(int j=0; j<n; j++){
    		if(b[i][j]==max){
    			g++;
    			big[l].i=i;
    			big[l].j=j;
    			l++;
			}
		}
	}
	cout<<max<<" "<<g<<endl;
	for(int i=0; i<g; i++){
		cout<<big[i].i<<" "<<big[i].j<<endl;
	}
	return 0;
}
