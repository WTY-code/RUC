#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
   
struct Student {  
    int id;//ѧ��  
    char name[20];//����  
    char classname[20];//�༶  
    int gpa;//ѧ�ּ�  
}; 
void getData(Student *ary)
{
	int class_num = 0,total_xue = 0,total_temp = 0;//ѡ���������пγ���ѧ�֡��ɼ���ѧ�ֳ˻��ĺ� 
	int class_grade[10], class_xue[10];//ÿ�ֿεĳɼ���ѧ�� 
	scanf("%d %s %s %d",&ary->id,ary->name,ary->classname,&class_num);//����ѧ�š��������༶��ѡ���� 
	printf("scanf 1\n");
	for(int i = 0; i < class_num; i++){//����ÿ�ſγ̵ĳɼ���ѧ�� 
		scanf("%d %d",&class_grade[i],&class_xue[i]);
		printf("scanf %d\n",i);
	}
	for(int i=0; i<class_num; i++){//
		total_xue = total_xue+class_xue[i];// �������п���ѧ�� 
		total_temp = total_temp+class_grade[i]*class_xue[i];//����ɼ���ѧ�ֳ˻��ĺ� 
	}
	printf("total_xue is %d\n",total_xue);
	printf("total_temp is %d\n",total_temp);
/*	for(int i = 0; i < class_num; i++){
		total_temp = total_temp+temp[i];
	}
*/
	ary->gpa = total_temp/total_xue;//��gpa 
	ary->gpa = (int)ary->gpa;//ȡ�� 
	printf("ary->gpa is %d\n",ary->gpa);
	return;
 } 
void mySort( Student *ary,int n, char str[] )
{
	if(strcmp(str,"MIN")==0){//�ж������� 
		for(int i = 0; i < n; i++){//ѡ������ 
			int k = i;
			for(int j = i+1; j < n; j++){
				if(ary[j].gpa < ary[k].gpa){//gpaС����ǰ 
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
				if((ary[j].gpa = ary[k].gpa)&&(ary[j].id<ary[j].id)){//gpa��ͬѧ��С����ǰ 
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
        //��ʵ�ֵĺ���������һ��ѧ�������ݼ���GPA������ŵ��ṹ�������
        getData( &ary[i] );  
    }  

    //��ʵ�ֵ�������������ֵΪ��
    //��n��Ԫ�ؽṹ������ary����str�����Ĺ�����н������������±��0��ʼ
    mySort( ary, n, str );

    for ( i = 0; i < n; i ++ )  
    {  
        printf("%d %s %s %d\n", ary[i].id, ary[i].name, ary[i].classname, ary[i].gpa );  
    }  
      
    return 0;  
  
}  
