#include<iostream>
using namespace std;
int find(int a[],int,int,int);
int main()
{
	int a[16]={8,13,21,28,35,41,52,63,71,76,81,95,101,150,164};
	int l=0,r=15,m=21;
	cout << find(a,l,r,21);
	return 0;
}
int find(int ary[],int left,int right,int aim)
{
	//int left,right,aim;
	if(left>right) return -1;
	if(ary[(left+right)/2]==aim) return (left+right)/2;
	else if(ary[(left+right)/2]>aim)
	    return find(ary,left,(left+right)/2-1,aim);
    else
        return find(ary,(left+right)/2+1,right,aim);
}
