#include<iostream>
#include<stdio.h>
int ski[51][51],path[2500];
int snow(int row, int col, int index, int maxl){
	int h;
	h = ski[row][col];
	printf("just in to function: \n");
	printf("ski[%d][%d] is %d\n",row,col,ski[row][col]);
	if(h <= ski[row][col-1]
	   &&h <= ski[row][col+1]
	   &&h <= ski[row-1][col]
	   &&h <= ski[row+1][col]){
	   	path[index] = h;
	   	printf("path[%d] is %d\n",index,path[index]);
	   	if(index+1>maxl){
	   		maxl = index + 1;
	   		printf("maxl is %d\n",maxl);
		}
    }
	else{
	    printf("this is 'else':\n");
	    printf("index(before++) is %d\n",index);
    	path[index++] = h;
    	
    	printf("h is %d",h);
    	printf("path[%d] is %d\n",index,path[index]);
    	if(h > ski[row][col-1])
    	   maxl = snow(row,col-1,index,maxl);
    	if(h > ski[row][col+1])
    	   maxl = snow(row,col+1,index,maxl);
    	if(h > ski[row-1][col])
    	   maxl = snow(row-1,col,index,maxl);
    	if(h > ski[row+1][col])
    	   maxl = snow(row+1,col,index,maxl);
	}
	return maxl;

} 
int main()
{
	int ski_l;
	int max = 0;
	int m = 5, n = 5;
	scanf("%d%d",&m, &n);
	for(int i=1; i<=m; i++){
		for(int j=1; j<=n; j++){
			scanf("%d",&ski[i][j]);
		}
	}
	for(int i=0; i<= m+1; i++){
		ski[i][0] = ski[i][n+1] = 10001;
	}
	for(int i=0; i<= n+1; i++){
		ski[0][i] = ski[m+1][i] = 10001;
	}
	for(int i=1;i<=m; i++){
		for(int j=1; j<=n;j++){
			ski_l=snow(i,j,0,0);
			if(ski_l>max) max = ski_l;
		}
	}
	printf("%d\n",max);
	return 0;
}
