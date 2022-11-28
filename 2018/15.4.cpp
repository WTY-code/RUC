#include<iostream>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h> 
using namespace std;
//int *r[100];
int k=0;
int key[100] = {0},a[100][100]={{0}};
int comp(const void*a, const void*b){
	for(int i=0; i<k; i++){
		if(*((int*)a+key[i]-1) != *((int*)b+key[i]-1)){
			return *((int*)a+key[i]-1) - *((int*)b+key[i]-1);
		}
	}
//	if(r[j][0]<r[k][0]) return 1;
//	else if(r[j][0]<r[k][0]) return -1;
	return 0;
}
int main()
{	
    
	int k=0,m=0,n=0;
	//,a[100][100] = {{0}};
	cin>>m>>n;
//	for(int i=0; i<m; i++){
//		r[i]= a[i];
//	}
	cin>>k;
	for(int i=0; i<k; i++){
		cin>>key[i];
	}
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			scanf("%d",a[i][j]);
		}
	}
/*	for(int i=0; i<k; i++){
		if(r[])
	}
	
*/	
/*	for(int i=0; i<m; i++){
		int kk=i;
		for(int j=i+1; j<m; j++){
			if(comp(j,kk,k)>0) kk=j;
		}
		if(kk!=i){
			swap(r[k],r[i]);
		}
	}
	*/
	qsort(a,m,sizeof(int)*n,comp);
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			cout<<a[i][j]<<" ";
		}
		cout<<endl;
	}

	
	return 0;
}
