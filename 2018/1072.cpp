#include<iostream>
#include<cmath>
#include<cstring>
#include<stdio.h>
using namespace std;
int pow(int x, int y)
{
	int s = 1; 
	for(int i = 0; i < y; i++){
		s = s*x;
	}
	return s;
}
int main()
{
    int n = 0,len_alien_number[100]={0},len_target_number[100]={0};//��ת��������Ŀ�����ֵ�λ�� 
	int source_n[100]={0},target_n[100]={0};//ԭ��������Ŀ����� 
	int alien_number_a[100]={0},target_number_a[100]={0};//��ת��������Ŀ������ÿһλ��Ӧ��ʮ�������� 
	int alien_number10[100]={0};//��ת�����ֵ�ʮ����ֵ 
	char alien_number[100][100]={{0}},source_language[100][100]={{0}},target_language[100][100]={{0}};
	// ��ת��������ԭ�����ơ�Ŀ������ַ���
	int shang[100]={0},yu[100]={0};//������ת��ʱ��Ҫ���̺�����
	cin>>n;
	getchar();//����n����ʾ���м���ֵ 
//	cout<<"n is "<<n<<endl; 
	for(int i = 0; i < n; i++){
		cin.getline(alien_number[i],98,' ');//�����ת������ 
//		printf("alien_number[%d] is %s\n",i,alien_number[i]);
		cin.getline(source_language[i],98,' ');//����ԭ�������� 
//		printf("source_language [%d] is %s\n",i,source_language);
		cin.getline(target_language[i],98,'\n');
//		printf("target_language [%d] is %s\n",i,target_language);
	}//����Ŀ�����
	//cout<<alien_number[0]<<"hahaha";
	for(int i = 0; i < n; i++){
	    cout<<"Case #"<<i+1<<":  ";//�ȴ�ӡǰ�����ʾ��Ϣ 
		len_alien_number[i]=strlen(alien_number[i]);//�����ת�����ֵ�λ�� 
//		printf("len_alien_number[%d] is %d\n",i,len_alien_number[i]);
		source_n[i]=strlen(source_language[i]);//�����ת�������Ǽ�����
//		printf("source_n[%d] is %d",i,source_n[i]); 
		target_n[i]=strlen(target_language[i]);//����Ŀ������Ǽ����� 
//		printf("target_n[%d] is %d",i,target_n[i]);
		for(int j = 0; j < len_alien_number[i]; j++){//����ת�����ֵ�ÿһλ�ó�0����ֵֹ�ۻ����� 
			alien_number_a[j]=0;
		}
		for(int j = 0; j < len_alien_number[i]; j++){//��ת�����ֵ�ÿһλ������ԭ�������е����֣����ж�Ӧ 
			for(int k = 0; k < source_n[i]; k++){
				if(alien_number[i][j]==source_language[i][k]){
					alien_number_a[j] = k;//��ÿһλ��Ӧԭ�������е����֣�������ʮ����ʾ�� 
//					printf("alien_number_a[%d] is %d ",j,k);
				}
			}
		}
//		cout<<endl;
		for(int j = 0; j < len_alien_number[i]; j++){//����ת���ֱ�Ϊʮ������ 
			alien_number10[i]=alien_number10[i]+alien_number_a[j]*pow(source_n[i],len_alien_number[i]-j-1);
//			printf("source_n[%d] is %d\n",i,source_n[i]);
//			printf("len_alien_number[%d]-%d-1 is %d\n",i,j,len_alien_number[i]-j-1);
//			printf("pow is %d\n",pow(10,len_alien_number[i]-j-1));
//			printf("alien_number10[%d] is %d\n",i,alien_number10[i]);
		}
		for(int j = 0; j <100; j++){//��������Ҫ�õ��̺�������Ϊ0 
			shang[j] = 0;
			yu[j] = 0;
		}
		shang[0] = alien_number10[i]/target_n[i];//����һ���̺������ó�ֵ����ֹj-1Խ��
//		cout<<"shang[0] is "<<shang[0]<<" "; 
		yu[0] = alien_number10[i]%target_n[i];
//		cout<<"yu[0] is "<<yu[0]<<endl;
		
		for(int j = 1; j < 100; j++){//����ת������ʮ����תΪĿ����� 
		//	len_target_number[i] = 1;
			shang[j] = shang[j-1]/target_n[i];
//			cout<<"shang["<<j<<"] is "<<shang[j]<<" ";
			yu[j] = shang[j-1]%target_n[i];
//			cout<<"yu["<<j<<"] is "<<yu[j]<<endl;
			if((shang[j]==0)&&(shang[0]!=0)){//��������Ϊ0ֹͣ 
				len_target_number[i] = j+1;//����Ŀ�����ֵ�λ�� 
//				cout<<"len_target_number["<<i<<"] is "<<len_target_number[i]<<endl;
				break;
			}
			if(shang[0]==0){
				len_target_number[i] = 1;
				break;
			}
		}
		for(int j = 0; j < len_target_number[i]; j++){//��Ŀ��������Ŀ������µ�ÿһλ��ֵ0����ֹ�ۻ����� 
			target_number_a[j] = 0;
		} 
		for(int j = 0; j < len_target_number[i];j++){//�õ�Ŀ������µ�Ŀ���������ÿһλ������ţ�ÿһλ������Ϊʮ���ƣ� 
			target_number_a[j] = yu[len_target_number[i]-1-j];
//			cout<<"target_number_a["<<j<<"] is "<<target_number_a[j]<<endl;
		}
		for(int j = 0; j < len_target_number[i]; j++){//�����ҵ���ÿһλ��Ӧ��Ŀ������µķ��� 
			for(int k = 0; k < target_n[i]; k++){
			/*	int zero = 0;
				if((target_number_a[j]==target_n[0])){
					zero++;
			
				}
			*/
				if(target_number_a[j]==k){
					cout<<target_language[i][k]; //��Ӧ�ɹ�������÷��� 
				}
			}
		}
	    cout<<endl;
	}//��i�����ݴ�����ɣ����� 
	return 0;
} 
	 
	
	
