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
    int n = 0,len_alien_number[100]={0},len_target_number[100]={0};//待转换数字与目标数字的位数 
	int source_n[100]={0},target_n[100]={0};//原来进制与目标进制 
	int alien_number_a[100]={0},target_number_a[100]={0};//待转换数字与目标数字每一位对应的十进制数字 
	int alien_number10[100]={0};//待转换数字的十进制值 
	char alien_number[100][100]={{0}},source_language[100][100]={{0}},target_language[100][100]={{0}};
	// 待转换数字与原来进制、目标进制字符串
	int shang[100]={0},yu[100]={0};//做进制转换时需要的商和余数
	cin>>n;
	getchar();//输入n，表示共有几组值 
//	cout<<"n is "<<n<<endl; 
	for(int i = 0; i < n; i++){
		cin.getline(alien_number[i],98,' ');//读入待转换数字 
//		printf("alien_number[%d] is %s\n",i,alien_number[i]);
		cin.getline(source_language[i],98,' ');//读入原计数进制 
//		printf("source_language [%d] is %s\n",i,source_language);
		cin.getline(target_language[i],98,'\n');
//		printf("target_language [%d] is %s\n",i,target_language);
	}//读入目标进制
	//cout<<alien_number[0]<<"hahaha";
	for(int i = 0; i < n; i++){
	    cout<<"Case #"<<i+1<<":  ";//先打印前面的提示信息 
		len_alien_number[i]=strlen(alien_number[i]);//计算待转换数字的位数 
//		printf("len_alien_number[%d] is %d\n",i,len_alien_number[i]);
		source_n[i]=strlen(source_language[i]);//计算待转换数字是几进制
//		printf("source_n[%d] is %d",i,source_n[i]); 
		target_n[i]=strlen(target_language[i]);//计算目标进制是几进制 
//		printf("target_n[%d] is %d",i,target_n[i]);
		for(int j = 0; j < len_alien_number[i]; j++){//将待转换数字的每一位置成0，防止值累积出错 
			alien_number_a[j]=0;
		}
		for(int j = 0; j < len_alien_number[i]; j++){//待转换数字的每一位都遍历原来进制中的数字，进行对应 
			for(int k = 0; k < source_n[i]; k++){
				if(alien_number[i][j]==source_language[i][k]){
					alien_number_a[j] = k;//将每一位对应原来进制中的数字（数字用十进表示） 
//					printf("alien_number_a[%d] is %d ",j,k);
				}
			}
		}
//		cout<<endl;
		for(int j = 0; j < len_alien_number[i]; j++){//将待转数字变为十进制数 
			alien_number10[i]=alien_number10[i]+alien_number_a[j]*pow(source_n[i],len_alien_number[i]-j-1);
//			printf("source_n[%d] is %d\n",i,source_n[i]);
//			printf("len_alien_number[%d]-%d-1 is %d\n",i,j,len_alien_number[i]-j-1);
//			printf("pow is %d\n",pow(10,len_alien_number[i]-j-1));
//			printf("alien_number10[%d] is %d\n",i,alien_number10[i]);
		}
		for(int j = 0; j <100; j++){//将接下来要用的商和余数置为0 
			shang[j] = 0;
			yu[j] = 0;
		}
		shang[0] = alien_number10[i]/target_n[i];//给第一个商和余数置初值，防止j-1越界
//		cout<<"shang[0] is "<<shang[0]<<" "; 
		yu[0] = alien_number10[i]%target_n[i];
//		cout<<"yu[0] is "<<yu[0]<<endl;
		
		for(int j = 1; j < 100; j++){//将待转换数从十进制转为目标进制 
		//	len_target_number[i] = 1;
			shang[j] = shang[j-1]/target_n[i];
//			cout<<"shang["<<j<<"] is "<<shang[j]<<" ";
			yu[j] = shang[j-1]%target_n[i];
//			cout<<"yu["<<j<<"] is "<<yu[j]<<endl;
			if((shang[j]==0)&&(shang[0]!=0)){//当除到商为0停止 
				len_target_number[i] = j+1;//记下目标数字的位数 
//				cout<<"len_target_number["<<i<<"] is "<<len_target_number[i]<<endl;
				break;
			}
			if(shang[0]==0){
				len_target_number[i] = 1;
				break;
			}
		}
		for(int j = 0; j < len_target_number[i]; j++){//将目标数字在目标进制下的每一位赋值0，防止累积出错 
			target_number_a[j] = 0;
		} 
		for(int j = 0; j < len_target_number[i];j++){//得到目标进制下的目标进制数（每一位单独存放，每一位的数字为十进制） 
			target_number_a[j] = yu[len_target_number[i]-1-j];
//			cout<<"target_number_a["<<j<<"] is "<<target_number_a[j]<<endl;
		}
		for(int j = 0; j < len_target_number[i]; j++){//遍历找到与每一位对应的目标进制下的符号 
			for(int k = 0; k < target_n[i]; k++){
			/*	int zero = 0;
				if((target_number_a[j]==target_n[0])){
					zero++;
			
				}
			*/
				if(target_number_a[j]==k){
					cout<<target_language[i][k]; //对应成功，输出该符号 
				}
			}
		}
	    cout<<endl;
	}//第i组数据处理完成，换行 
	return 0;
} 
	 
	
	
