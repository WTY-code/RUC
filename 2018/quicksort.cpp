#include<iostream>
using namespace std;
int partition(int a[], int z , int y)
{
	int k = a[z];
	do{
		while((z < y)&&(a[y] >= k)){
			y--;
		}
		if(z < y){
			a[z] = a[y];
			z++;
		}
		while((z < y)&&(a[z] < k)){
			z++;
		}
		if(z < y){
			a[y] = a[z];
			y--;
		}
	}while(z != y);
	a[z] = k;
	return z;
}
void quicksort(int a[], int z, int y)
{
	if(z < y){
		int m = partition(a , z , y);
		quicksort(a , z , m - 1);
		quicksort(a , m + 1 , y);
	}
	return;
}
int main()
{
	int arry[9] = {3,6,1,3,7,9,0,5,4};
	quicksort(arry, 0, 8);
	for(int i = 0; i < 9; i++){
		printf("%d ", arry[i]);
	};
	return 0;
}
