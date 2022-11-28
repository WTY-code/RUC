#include<iostream>
using namespace std;
int main()
{
	int a[16]={8,13,21,28,35,41,52,63,71,76,81,95,101,150,164};
    int i=0,j=14,aim=21;
    do{
    	if(i>j) cout << -1;
    	if(a[(i+j)/2]==aim) 
		{
		    cout << (i+j)/2;
    	else if(a[(i+j)/2]>aim)  j=(i+j)/2-1;
    	else i=(i+j)/2+1;
	}while(a[(i+j)!=aim]);
	return 0; 
}
