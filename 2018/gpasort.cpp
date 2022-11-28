#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
   
struct Student {  
    int id;//学号  
    char name[20];//姓名  
    char classname[20];//班级  
    int gpa;//学分绩  
}; 
void getData(Student *ary)
{
	int class_num = 0,total_xue = 0,total_temp = 0;//选课数、所有课程总学分、成绩与学分乘积的和 
	int class_grade[10], class_xue[10];//每种课的成绩、学分 
	scanf("%d %s %s %d",&ary->id,ary->name,ary->classname,&class_num);//输入学号、姓名、班级、选课数 
	printf("scanf 1\n");
	for(int i = 0; i < class_num; i++){//输入每门课程的成绩、学分 
		scanf("%d %d",&class_grade[i],&class_xue[i]);
		printf("scanf %d\n",i);
	}
	for(int i=0; i<class_num; i++){//
		total_xue = total_xue+class_xue[i];// 计算所有课总学分 
		total_temp = total_temp+class_grade[i]*class_xue[i];//计算成绩与学分乘积的和 
	}
	printf("total_xue is %d\n",total_xue);
	printf("total_temp is %d\n",total_temp);
/*	for(int i = 0; i < class_num; i++){
		total_temp = total_temp+temp[i];
	}
*/
	ary->gpa = total_temp/total_xue;//算gpa 
	ary->gpa = (int)ary->gpa;//取整 
	printf("ary->gpa is %d\n",ary->gpa);
	return;
 } 
void mySort( Student *ary,int n, char str[] )
{
	if(strcmp(str,"MIN")==0){//判断升序降序 
		for(int i = 0; i < n; i++){//选择排序 
			int k = i;
			for(int j = i+1; j < n; j++){
				if(ary[j].gpa < ary[k].gpa){//gpa小的在前 
					k = j;
				}
			}
			if(k!=i){
				int tmp_gpa = ary[i].gpa;
				ary[i].gpa = ary[k].gpa;
				ary[k].gpa = tmp_gpa;
				int tmp_id = ary[i].id;
				ary[i].id = ary[k].id;
				ary[k].id = tmp_id;
				char tmp_name[20];
				strcpy(tmp_name,ary[i].name);
				strcpy(ary[i].name,ary[k].name);
				strcpy(ary[k].name,tmp_name);
				char tmp_classname[20];
				strcpy(tmp_classname,ary[i].classname);
				strcpy(ary[i].classname,ary[k].classname);
				strcpy(ary[k].classname,tmp_classname);
				
			}
		}
		for(int i = 0; i < n; i++){
			int k = i;
			for(int j = i+1; j < n; j++){
				if((ary[j].gpa = ary[k].gpa)&&(ary[j].id<ary[j].id)){//gpa相同学号小的在前 
					k = j;
				}
			}
			if(k!=i){
				int tmp = ary[i].id;
				ary[i].id = ary[k].id;
				ary[k].id = tmp;
				char tmp_name[20];
				strcpy(tmp_name,ary[i].name);
				strcpy(ary[i].name,ary[k].name);
				strcpy(ary[k].name,tmp_name);
				char tmp_classname[20];
				strcpy(tmp_classname,ary[i].classname);
				strcpy(ary[i].classname,ary[k].classname);
				strcpy(ary[k].classname,tmp_classname);
			}
		}
	}
	else{
		for(int i = 0; i < n; i++){
			int k = i;
			for(int j = i+1; j < n; j++){
				if(ary[j].gpa > ary[k].gpa){
					k = j;
				}
			}
			if(k!=i){
				int tmp = ary[i].gpa;
				ary[i].gpa = ary[k].gpa;
				ary[k].gpa = tmp;
				int tmp_id = ary[i].id;
				ary[i].id = ary[k].id;
				ary[k].id = tmp_id;
				char tmp_name[20];
				strcpy(tmp_name,ary[i].name);
				strcpy(ary[i].name,ary[k].name);
				strcpy(ary[k].name,tmp_name);
				char tmp_classname[20];
				strcpy(tmp_classname,ary[i].classname);
				strcpy(ary[i].classname,ary[k].classname);
				strcpy(ary[k].classname,tmp_classname);
			}
		}
		for(int i = 0; i < n; i++){
			int k = i;
			for(int j = i+1; j < n; j++){
				if((ary[j].gpa = ary[k].gpa)&&(ary[j].id<ary[j].id)){
					k = j;
				}
			}
			if(k!=i){
				int tmp = ary[i].id;
				ary[i].id = ary[k].id;
				ary[k].id = tmp;
				char tmp_name[20];
				strcpy(tmp_name,ary[i].name);
				strcpy(ary[i].name,ary[k].name);
				strcpy(ary[k].name,tmp_name);
				char tmp_classname[20];
				strcpy(tmp_classname,ary[i].classname);
				strcpy(ary[i].classname,ary[k].classname);
				strcpy(ary[k].classname,tmp_classname);
				
			}
		}
	}
}




int main()  
{  
    int n, i, j;  
    char str[10];  
    Student ary[110];  
    scanf("%d %s", &n, str);  
  
    for ( i = 0; i < n; i ++ )  
    {
        //你实现的函数，读入一个学生的数据计算GPA，并存放到结构体变量中
        getData( &ary[i] );  
    }  

    //你实现的排序函数，返回值为空
    //对n个元素结构体数组ary按照str描述的规则进行进行排序，数组下标从0开始
    mySort( ary, n, str );

    for ( i = 0; i < n; i ++ )  
    {  
        printf("%d %s %s %d\n", ary[i].id, ary[i].name, ary[i].classname, ary[i].gpa );  
    }  
      
    return 0;  
  
}  
