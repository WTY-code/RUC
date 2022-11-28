#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
struct Stock_data{
	double index;
//	char Date[11];
	double Open;
	double High;
	double Low;
	double Close;
	double Adj_Close;
	double Volume;
//	char Code[10];
	double Change;
	double Pct_Change;
}*new_stock;
Stock_data stock[5] = {{0,4.5,4.8,3.5,4.2,4.18,10000,0.54,0.34},{1,4.5,4.9,4.3,4.4,4.44,9999,0.32,0.24},
            {2,4.3,4.7,3.8,4.5,4.56,12000,0.222,0.32},{3,4.4,4.7,2.4,3.8,3.87,8000,0.56,0.34},
			{4,4.2,5.0,3.2,4.7,4.87,13000,0.66,0.88}};
int GetFieldVal2(int index,char *name,void*pvalue) 
{
	if(strcmp(name,"index")==0){
		*(double*)pvalue = stock[index].index;
		return 1;
	}
/*	if(strcmp(name,"Date")==0){
		 
	    strcpy((char *)pvalue,new_stock[index].Date);
	    
	    return 1;
	}*/
	else if(strcmp(name,"Open")==0){
		*(double*)pvalue = stock[index].Open;
		return 1;
	}
	else if(strcmp(name,"High")==0){
		*(double*)pvalue = stock[index].High;
		return 1;
	}
	else if(strcmp(name,"Low")==0){
		*(double*)pvalue = stock[index].Low;
		return 1;
	}
	else if(strcmp(name,"Close")==0){
		*(double*)pvalue = stock[index].Close;
		return 1;
	}
	else if(strcmp(name,"Adj_Close")==0){
		*(double*)pvalue = stock[index].Adj_Close;
		return 1;
	}
	else if(strcmp(name,"Volume")==0){
		*(double*)pvalue = stock[index].Volume;
		return 1;	
	}
/*	else if(strcmp(name,"Code")==0){
		strcpy((char*)pvalue,new_stock[index].Code);
		return 0;
	}*/
	else if(strcmp(name,"Change")==0){
		*(double*)pvalue = stock[index].Change;
		return 1;
 	}
 	else if(strcmp(name,"Pct_Change")==0){
 		*(double*)pvalue = stock[index].Pct_Change;
 		return 1;
	 }
	else{
		printf("Sorry! You input a wrong name!\n");
		return 0;
	};
}
int main()
{
	//参数
	int reco_index[5] = {0,1,2,3,4}, n = 5;
	printf("following is your canshu:\n");
	printf("your n is 5\n");
	char order_by[100] = "Open";
	printf("your orderby is Open\n"); 
	//拆字符串 
	printf("\ndeal with s:\n");
	int key_num = 0;
	char order[][11] = {"index","Date","Open","High","Low","Close","Adj_Close",
	                                   "Volume","Code","Change","Pct_Change"};
	char name[][11] = {"index","Date","Open","High","Low","Close","Adj_Close",
	                                   "Volume","Code","Change","Pct_Change"};
	char *tmp;
//	gets(order_by);
	tmp = strtok(order_by,",");
	strcpy(order[key_num],tmp);
	key_num++;
	while((tmp = strtok(NULL,",")) != NULL){
		strcpy(order[key_num],tmp);
		key_num++;
	}
	for(int j = 0; j < key_num; j++){
		printf("order[%d] is %s\n",j,order[j]);
	}
	printf("total order is %d\n",key_num);
	//提取给定索引的数据，组成一个新的结构体数组 
	new_stock = (Stock_data*)calloc(key_num,sizeof(Stock_data));//dont forget to free
	if(new_stock==NULL){
		printf("new_stock's room is error!\n");
		exit(1);
	}
	printf("\ncheck GetFieldVal2:\n");
	for(int i = 0; i < n; i++){
		GetFieldVal2(reco_index[i],"index",&new_stock[i].index);
		printf("index: %lf ",new_stock[i].index );
//		GetFieldVal2(reco_index[i],"Date",&new_stock[i].Date);
		GetFieldVal2(reco_index[i],"Open",&new_stock[i].Open);
		printf("open:  %lf ",new_stock[i].Open );
		GetFieldVal2(reco_index[i],"High",&new_stock[i].High);
		printf("high: %lf ",new_stock[i].High );
		GetFieldVal2(reco_index[i],"Low",&new_stock[i].Low);
		printf("low: %lf ",new_stock[i].Low );
		GetFieldVal2(reco_index[i],"Close",&new_stock[i].Close);
		printf("close: %lf ",new_stock[i].Close );
		GetFieldVal2(reco_index[i],"Adj_Close",&new_stock[i].Adj_Close);
		printf("adj_close: %lf ",new_stock[i].Adj_Close);
		GetFieldVal2(reco_index[i],"Volume",&new_stock[i].Volume);
		printf("volume: %lf ",new_stock[i].Volume);
//		GetFieldVal2(reco_index[i],"Code",&new_stock[i].Code);
		GetFieldVal2(reco_index[i],"Change",&new_stock[i].Change);
		printf("change: %lf ",new_stock[i].Change);
		GetFieldVal2(reco_index[i],"Pct_Change",&new_stock[i].Pct_Change);
		printf("pct_change: %lf \n",new_stock[i].Pct_Change);
	}
	//大型分配内存现场 
	double ***p;
	p = (double***)calloc(11,sizeof(double(**)));//给每行分配内存 
	p[0] = (double**)calloc(n,sizeof(double*));//给指向index的指针分配内存
	printf("p[0]:%x ",p[0]);
	printf("sizeof(double*):%d\n",sizeof(double(*)));
	if(p[0]==NULL){
		printf("p[0]'s room error!\n");
		exit(1);
	}
	p[1] = (double**)calloc(n,sizeof(double*));//给指向Date的指针分配内存
	if(p[1]==NULL){
		printf("p[1]'s room error!\n");
		exit(1);
	}
	for(int i = 2; i <= 6; i++){
		p[i] = (double**)calloc(n,sizeof(double*));
		if(p[i]==NULL){
			printf("p[%d]'s room error!\n");
			exit(1);
		}
	}
	p[7] = (double**)calloc(n,sizeof(double*));//给指向Volume的指针分配内存
	if(p[7]==NULL){
		printf("p[7]'s room error!\n");
		exit(1);
	}
	p[8] = (double**)calloc(n,sizeof(double*));//给指向Code的指针分配内存
	if(p[8]==NULL){
		printf("p[8]'s room error!\n");
		exit(1);
	}
	p[9] = (double**)calloc(n,sizeof(double*));//给指向Change的指针分配内存
	if(p[9]==NULL){
		printf("p[9]'s room error!\n");
		exit(1);
	}
	p[10] = (double**)calloc(n,sizeof(double*));//给指向Pct_Change的指针分配内存 
	if(p[10]==NULL){
		printf("p[10]'s room error\n");
		exit(1);
	}
	printf("p[10]:%x\n",p[10]);
	
	//将指针p与结构体中的成员建立对应关系
	printf("\ncheck p[i][j] -> stock's member\n"); 
	for(int j = 0; j < n; j++){
		p[0][j] = &new_stock[j].index;
		printf("*p[0][%d](index):%lf ",j,*p[0][j]);
		p[1][j] = NULL;
//		p[1][j] = &new_stock[j].Date;
		p[2][j] = &new_stock[j].Open;
		printf("*p[2][%d](open):%lf ",j,*p[2][j]);
		p[3][j] = &new_stock[j].High;
		printf("*p[3][%d](high):%lf ",j,*p[3][j]);
		p[4][j] = &new_stock[j].Low;
		printf("*p[4][%d](low):%lf ",j,*p[4][j]);
		p[5][j] = &new_stock[j].Close;
		printf("*p[5][%d](close): %lf",j,*p[5][j]);
		p[6][j] = &new_stock[j].Adj_Close;
		printf("*p[6][%d](adj_close):%lf ",j,*p[6][j]);
		p[7][j] = &new_stock[j].Volume;
		printf("*p[7][%d](volume):%lf ",j,*p[7][j]);
		p[8][j] = NULL;
//		p[8][j] = &new_stock[j].Code;
		p[9][j] = &new_stock[j].Change;
		printf("*p[9][%d](change):%lf\n",j,*p[9][j]);
		p[10][j] = &new_stock[j].Pct_Change;
		
	}
	
	//建立关键词key和p的对应关系
	printf("\ncheck key -> p[i]\n"); 
	double***key[11];
	for(int i = 0; i < 11; i++){
		key[i] = &p[i];
		printf("key[i] and &p[i] should be the same:\n");
		printf("key[%d]:%x ",i,key[i]);
		printf("&p[%d]:%x\n",i,&p[i]);
		printf("p[i] and p[i][j] should be the same:\n");
		printf("p[%d]:%x\n",i,p[i]);
		printf("p[%d][0]:%x\n",i,p[i][0]);
	}
	//将关键词与指向结构体成员的指针p匹配 
	for(int i = 0; i < key_num; i++){
		for(int j = 1; j < 11; j++){
			if(strcmp(order[i],name[j])==0){
//				swap(&key[j],&key[i])
				double***tmp = key[j];
				key[j] = key[i];
				key[i] = tmp;
			}
		}
	}
	printf("\ncheck the order of key:\n");
	for(int i = 0; i < 11; i++){
		printf("key[%d]:%x ",i,key[i]);
	}
	printf("should be %x %x %x %x %x %x %x %x %x",p[2],p[0],p[3],p[4],p[5],p[6],p[7],p[9],p[10]);
	for(int i = 0; i < n; i++){
		int k = i; 
		int comp = 0;
		for(int j = i+1; j < n; j++){
			for(int l = 0; l < 11; l++){
			
			    if(**key[0][j]> **key[0][k]){
				    comp = 1;
				    break;
			    }
	            else if(**key[0][j] < **key[0][k]){
	        	    comp = -1;
	        	    break;
			    }
			    comp = 0;
            }
        
/*	if(*kkey[1][x] > *kkey[1][y]) return 1;
	else if(*kkey[1][x] < *kkey[1][y]) return -1;
	
	if(*kkey[2][x] > *kkey[2][y]) return 1;
	else if(*kkey[2][x] < *kkey[2][y]) return -1;
	
	if(*kkey[3][x] > *kkey[3][y]) return 1;
	else if(*kkey[3][x] < *kkey[3][y]) return -1;

	if(*kkey[4][x] > *kkey[4][y]) return 1;
	else if(*kkey[4][x] < *kkey[4][y]) return -1;
	
	if(**kkey[6][x] > **kkey[6][y]) return 1;
	else if(**kkey[6][x] < kkey[6][y]) return -1;
	
	if(**kkey[7][x] > **kkey[7][y]) return 1;
	else if(**kkey[7][x] < kkey[7][y]) return -1;
	
	if(**kkey[8][x] > **kkey[8][y]) return 1;
	else if(**kkey[8][x] < kkey[8][y]) return -1;
	
	if(**kkey[9][x] > **kkey[9][y]) return 1;
	else if(**kkey[9][x] < kkey[9][y]) return -1;
	
	if(**kkey[10][x] > **kkey[10][y]) return 1;
	else if(**kkey[10][x] < kkey[10][y]) return -1;
	
	if(**kkey[11][x] > **kkey[11][y]) return 1;
	else if(**kkey[11][x] < kkey[11][y]) return -1;
	
	return 0;*/
			if(comp < 0) k = j;
		}
		if(k!=i){
			int tmp = reco_index[k];
			reco_index[k] = reco_index[i];
			reco_index[i] = tmp;
		}
	}
	
	
	
	for(int i = 10; i <= 11; i++){
		free(p[i]);
	}
	for(int i = 3; i <= 7; i++){
		free(p[i]);
	}
	free(p[1]);
	free(p[2]);
	free(p[8]);
	free(p[9]);
	free(p);
	free(new_stock);
	for(int i = 0; i < n; i++){
		printf("%d ",reco_index[i]);
	}
	return 0;
} 

