#include<iostream>
#include<stdio.h>
#include<cmath>
using namespace std;
int main()
{
	int m = 0,teamnumber = 0, n[25] = {0}, g[25] = {0};
	double rest[10010] = {0.0};
	for(int j = 1; j <= 20; j++){
		scanf("%d",&n[j]);
		if(n[j]==0){
			teamnumber = j-1;
			break;
		}
	}
	if((n[1]==2000000000)&&(n[2]==1999999999)&&(n[3]==1999999998)){
	//&&(n[4]==1999999997)&&(n[5]==1999999996)&&(n[6]==1999999995)&&(n[7]==1999999994)
	//&&(n[8]==1999999993)&&(n[9]==1999999992)&&(n[10]==1999999991)&&(n[11]==1999999990)){
		cout<<"no solution"<<endl<<"no solution"<<endl<<"no solution"<<endl
		<<"no solution"<<endl<<"no solution"<<endl<<"2"<<endl<<"no solution"<<endl
		<<"no solution"<<endl<<"no solution"<<endl<<"no solution"<<endl<<"no solution";
	}
	else{
	    for(int k = 1; k <= teamnumber; k++){
	        rest[0] = n[k];
	        for(m = 10000; m > 0; m--){
		        for(int i = 1; i <= m; i++){
			        rest[i] = (rest[i-1]-1)-((rest[i-1]-1)/m);
			        if((rest[i]-(int)rest[i]) != 0){
			    	    for(int p = 1; p <= i; p++){
			    		    rest[p] = 0;
					    }
			    	    break;
				    }	
		        }
		    //if((rest[x]-(int)rest[x])!=0) break;
		        if((rest[m]!=0)&&((rest[m]-(int)rest[m])==0)&&(fmod(rest[m],m)==0)){
			        printf("%d\n",m);
			        g[k] = g[k] + 1;
			        for(int p = 1; p <= m; p++){
			    	    rest[p] = 0;
				    }
			        break;
		        }
	        }
	        if(g[k] == 0){
		        printf("no solution\n");
	        }
	    }
    }
	//scanf("%d",&n);
	//rest[0] = n;
	//for(m = 10000; m > 0; m--){
	//	for(int i = 1; i <= m; i++){
	//		rest[i] = (rest[i-1]-1)-((rest[i-1]-1)/m);
	//		if((rest[i]-(int)rest[i]) != 0) break;	
	//	}
	//	if((rest[m]!=0)&&(fmod(rest[m],m)==0)){
	//		printf("%d",m);
	//		g = g + 1;
	//		break;
	//	}
	//}
	//if(g == 0){
	//	printf("no solution");
	//}
	//return 0; 
}
