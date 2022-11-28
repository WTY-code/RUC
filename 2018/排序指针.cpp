//排序指针 
#include<iostream>
#include<stdio.h>
#include<ctime>
using namespace std;
int main()
{
	clock_t t; 
	int person_thing[21][101] = {0.0}, pmax = 0;//二维数组存每个人购物车物品种类，pmax为每次指针移动完成的最大项 
	int *p[21]  = {NULL};//指针指向每个人（一维数组）即p[i]指向person_thing[i] 
	int thing_quantity[21] = {0}, g[21] = {0};//thing_quantity为每个人购物车物品一共几种，g用来计数 
	int n = 0, m = 0, s = 0, min_thing_quantity = 0;//n表示一共几人，min_thing_quantity表示最短集合的长度,m标记找到多少共同元素 
	cin >> n;
	for(int i = 0; i < n; i++){//输入 
		cin >> thing_quantity[i];
		for(int j = 0; j < thing_quantity[i]; j++){
			cin >> person_thing[i][j];
		}
	}
	for(int i = 0; i < n; i++){//对每个集合中的元素排序（升序） 
		for(int j = 0; j < thing_quantity[i]; j++){
		    int k = j;
		    for(int l = j+1; l < thing_quantity[i]; l++){
			    if(person_thing[i][l] < person_thing[i][k]){
				    k = l;
			    }
		    }
		    if(k!=j){
			    float tmp = person_thing[i][k];
			    person_thing[i][k] = person_thing[i][j];
			    person_thing[i][j] = tmp;
		    }
	    }
	}
	for(int i = 0; i < n; i++){//给指针赋值，将指针与每个集合对应 
		p[i] = person_thing[i];
	}
	min_thing_quantity = thing_quantity[0];//找出最短集合的长度 
	for(int i = 0; i < n; i++){
		if(thing_quantity[i] < min_thing_quantity){
			min_thing_quantity = thing_quantity[i];
		}
	}
	for(int i = 0; i < min_thing_quantity; i++){//找出当前指针所指的各项哪个最大，用pmax记录 
		pmax = *p[0];
		for(int j = 0; j < n; j++){
			if(*p[j] > pmax){
				pmax = *p[j];
			}
		}
		for(int j = 0; j < n; j++){//对于每一个指针，所指内容小于最大值时指针后移,直到所指内容>=pmax 
			while((*p[j]<person_thing[j][thing_quantity[j]-1])&&(*p[j] < pmax)){//在指针不越界的情况下移动指针，如果已经移到末尾，指针不再移动 
				p[j]++;
				if(*p[j]==person_thing[j][thing_quantity[j]-1]){//如果当前指针已经到集合末尾，准备下一次跳出循环 
					i = i+min_thing_quantity;
					break;
				}
			}
		}
		for(int j = 0; j < n; j++){//判断指针停留位置所指内容是否等于pmax 
			if(*p[j] != pmax){//如果不是，标记该集合没有这个元素 
				g[j] = 0;
			}
			else{
				g[j] = 1;//如果是，标记该集合有这个元素 
			}
		}
		s = 0;
		for(int j = 0; j < n; j++){//统计n个集合中有几个这个元素 
			s = s + g[j];
		}
		if(s==n){//如果有n个，证明这是所有集合共有的 
			cout << pmax <<" ";//输出这个元素和空格 
			m++;//计数 
			for(int j = 0; j < n; j++){
				if(*p[j]==person_thing[j][thing_quantity[j]-1]){//如果当前指针已经已经移动到集合末尾，准备跳出循环 
					i = i+min_thing_quantity;
				}
			}
			for(int j = 0; j < n; j++){//找到一个相同元素后所有指针同时后移一位 
				p[j]++;
			}
		}
	}
	if(m==0) cout<<"no";
	t = clock();
	cout <<t;
	return 0;
}

