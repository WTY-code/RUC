#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
/*int comp(const void*a,const void*b)
{
	return *(int*)a-*(int*)b;
}
*/
int main()
{
	char s[200][11];
	int a[200][10] ={{0}} ,longnum[200] = {0},sum[200] = {0},go[200]={0};
	int n,k,count = 0;
	scanf("%d %d",&n,&k);
	for(int i=0; i<n; i++){
		scanf("%s",s[i]);//×Ö·û´®ÊäÈëÑ§ºÅ 
//		printf("s[%d] is %s\n",i,s[i]);
		}
	for(int i=0; i<n; i++){
		longnum[i]=atoi(s[i]);//×Ö·û´®×ªÊý×Ö 
//		printf("longnum[%d] is %d\n",i,longnum[i]);
	}
//	qsort(longnum, n, sizeof(int), comp);//ÅÅÐò 
   
	for(int i = 0; i <n; i++){
//		printf("longnum[%d] is %d\n",i,longnum[i]);
	}
	for(int i=0; i< n;i++){
		for(int j = 0; j<10; j++){
			a[i][j] = s[i][j]-48;//°Ñ×Ö·û´®Ã¿¸ö×Ö·û×ª³ÉÊý×Ö
//			printf("s[%d][%d] is %c\n",i,j,s[i][j]);
//			printf("a[%d][%d] is %d\n",i,j,a[i][j]); 
		}
	}
	for(int i = 0; i < n;i++){
		for(int j = 0; j<10;j++){
			sum[i] = sum[i]+a[i][j];
//			printf("s[%d] is %d\n",i,sum[i]);
		}
//		printf("total sum[%d] is %d\n",i,sum[i]);
		if((sum[i]%k)==0){
			go[i] = 1;
//			printf("go[%d] is %d\n",i,go[i]);
			count++;
		}
	}
	 for(int i=0;i<n;i++){
    	int k=i;
    	for(int j=i+1;j<n;j++){
    		if(longnum[j]<longnum[k]) k=j;
		}
		if(k!=i){
			int tmp1=longnum[i];
			longnum[i]=longnum[k];
			longnum[k]=tmp1;
			int tmp2 = go[i];
			go[i]=go[k];
			go[k] = tmp2;
		}
	}
	printf("%d\n",count);
	for(int i =0;i<n;i++){
		if(go[i]==1){
			printf("%d\n",longnum[i]);
		}
	}
	return 0;
}
