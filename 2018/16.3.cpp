#include<iostream>
#include<cmath>
#include<stdio.h>
using namespace std;
struct S{
	int id;
	int class_num;
	double grade[100];
	double general;
}stu[10000];
int main()
{
	int n = 0,g = 0;
	double temp[10000] = {0.0};
	cin>>n;
	for(int i = 0; i < n; i++){
		cin>>stu[i].id>>stu[i].class_num;
		for(int j = 0; j< stu[i].class_num; j++){
			cin>>stu[i].grade[j];
		}
	}
	for(int i= 0;i<n;i++){
		for(int j=0; j<stu[i].class_num; j++){
			temp[i] = (temp[i]+stu[i].grade[j])*100.0;
		}
		stu[i].general = temp[i]/stu[i].class_num;
	//	double die1 = (stu[i].general-(int)stu[i].general )*100;
	//	double die2 = die1-(int)die2;
	//	stu[i].general = stu[i].general - die2;
//		cout<<i<<stu[i].general <<endl;
	}
	for(int i = 0; i < n; i++){
		int k = i;
		for(int j=i+1;j<n;j++){
			if(stu[j].general>stu[k].general) k = j;
		}
		if(k!=i){
			swap(stu[k].general,stu[i].general);
			swap(stu[k].id,stu[i].id);
			swap(stu[k].class_num,stu[i].class_num);
		}
	}
	for(int i = 0; i < n; i++){
		int k = i;
		for(int j=i+1;j<n;j++){
			if((stu[j].general==stu[k].general)&&(stu[j].id<stu[k].id)) k = j;
		}
		if(k!=i){
		//	swap(stu[k].general,stu[i].general);
			swap(stu[k].id,stu[i].id);
			swap(stu[k].class_num,stu[i].class_num);
		}
	}
	for(int i=0; i<n; i++){
		if(stu[i].class_num>=2){
		//	cout<<stu[i].id <<" "<<stu[i].general <<endl;
		//	printf("%d %.2f\n",stu[i].id,stu[i].general);
		g++;
		}
	}
	if(n==0||g==0){
		cout<<"NO";
	}
	else{
	
	    for(int i=0; i<n; i++){
		    if(stu[i].class_num>=2){
		//	cout<<stu[i].id <<" "<<stu[i].general <<endl;
			    printf("%d %.2f\n",stu[i].id,stu[i].general/100.0);
		    }
	    }
    }
	return 0;
}
