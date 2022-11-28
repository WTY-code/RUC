#include<stdio.h>
int a[51][51];
int r = 0, c = 0, max = 0;
int snow(int i, int j, int max, int tmp)
{
	printf("i:%d j:%d\n",i,j);
	if(i<0||i>(r-1)||j<0||j>(c-1)) return max;
	if(a[i][j]>a[i+1][j]){
		tmp++;
		if(tmp>max) max = tmp;
		printf("(%d,%d) tmp: %d  max: %d\n",i+1,j,tmp,max);
		snow(i+1,j,max,tmp);
	}
	if(a[i][j]>a[i-1][j]){
		tmp++;
		if(tmp>max) max = tmp;
		printf("(%d,%d) tmp: %d  max: %d\n",i-1,j,tmp,max);
		snow(i-1,j,max,tmp);
	}
	if(a[i][j]>a[i][j+1]){
		tmp++;
		if(tmp>max) max = tmp;
		printf("(%d,%d) tmp: %d  max: %d\n",i,j+1,tmp,max);
		snow(i,j+1,max,tmp);
	}
	if(a[i][j]>a[i][j-1]){
		tmp++;
		if(tmp>max) max = tmp;
		printf("(%d,%d) tmp: %d  max: %d\n",i,j-1,tmp,max);
		snow(i,j-1,max,tmp);
	}
	if((a[i][j]<a[i-1][j])&&(a[i][j]<a[i+1][j])&&(a[i][j]<a[i][j+1])&&(a[i][j]<a[i][j-1])){
		if(tmp>max) max = tmp;
		return max;
	}
}
int main()
{
	int len_max = 0,imax = 0,jmax = 0;
	scanf("%d %d",&r,&c);
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			scanf("%d",&a[i][j]);
		//	printf("%d")
		}
	}
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			if(a[imax][jmax]<a[i][j]){
				imax = i;
				jmax = j;
			}
		}
	}
	printf("imax:%d jmax:%d\n",imax,jmax);
	len_max = snow(imax,jmax,max,0);
	printf("%d",len_max);
	return 0;
}
