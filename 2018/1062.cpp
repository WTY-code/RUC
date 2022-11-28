#include<iostream>
using namespace std;
int find(int a[], int, int, int);
int main()
{
	int n = 0, ai = 0, y = 0;
	int arry[100] = {0};
	cin >> n >> ai;
	for(int i = 0; i < n; i++){
		cin >> arry[i];
	} 
	y = find(arry,0,n-1,ai);
	cout << y;
	return 0;
 } 
int find(int ary[],int left,int right,int aim)
{
	//int left,right,aim;
	cout << "search "<< aim <<" from "<< left <<" to " << right <<endl;
	if(left>right) return -1; 
	if(ary[(left+right)/2]==aim) return (left+right)/2;
	else if(ary[(left+right)/2]>aim)
	    return find(ary,left,(left+right)/2-1,aim);
    else
        return find(ary,(left+right)/2+1,right,aim);
} 
