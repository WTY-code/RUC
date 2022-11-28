#include<iostream>
using namespace std;

int partition(int ary[],int z, int y)
{
	int k = ary[z];
	do{
		while((z<y)&&(ary[y]>=k)){
			y--;
		}
		if(z<y){
		    ary[z] = ary[y];
		    z++;
	    }
		while((z<y)&&(ary[z]<k)){
			z++;
		}
		if(z<y){
	    	ary[y] = ary[z];
		    y--;
	    }
	}while(z!=y);
	ary[z] = k;
	return z;
}
void quicksort(int ary[], int z, int y)
{
	int m;
	if(z>=y) return;
	m = partition(ary,z,y);
	quicksort(ary,z,m-1);
	quicksort(ary,m+1,y);
}
int main()
{
	int arry[10] = {4,9,1,0,6,8,56,34,23,-2};
	quicksort(arry,0,9);
	for(int i = 0; i < 10; i++){
		cout<<arry[i]<<" ";
	}
	return 0;
}
