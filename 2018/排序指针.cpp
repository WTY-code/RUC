//����ָ�� 
#include<iostream>
#include<stdio.h>
#include<ctime>
using namespace std;
int main()
{
	clock_t t; 
	int person_thing[21][101] = {0.0}, pmax = 0;//��ά�����ÿ���˹��ﳵ��Ʒ���࣬pmaxΪÿ��ָ���ƶ���ɵ������ 
	int *p[21]  = {NULL};//ָ��ָ��ÿ���ˣ�һά���飩��p[i]ָ��person_thing[i] 
	int thing_quantity[21] = {0}, g[21] = {0};//thing_quantityΪÿ���˹��ﳵ��Ʒһ�����֣�g�������� 
	int n = 0, m = 0, s = 0, min_thing_quantity = 0;//n��ʾһ�����ˣ�min_thing_quantity��ʾ��̼��ϵĳ���,m����ҵ����ٹ�ͬԪ�� 
	cin >> n;
	for(int i = 0; i < n; i++){//���� 
		cin >> thing_quantity[i];
		for(int j = 0; j < thing_quantity[i]; j++){
			cin >> person_thing[i][j];
		}
	}
	for(int i = 0; i < n; i++){//��ÿ�������е�Ԫ���������� 
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
	for(int i = 0; i < n; i++){//��ָ�븳ֵ����ָ����ÿ�����϶�Ӧ 
		p[i] = person_thing[i];
	}
	min_thing_quantity = thing_quantity[0];//�ҳ���̼��ϵĳ��� 
	for(int i = 0; i < n; i++){
		if(thing_quantity[i] < min_thing_quantity){
			min_thing_quantity = thing_quantity[i];
		}
	}
	for(int i = 0; i < min_thing_quantity; i++){//�ҳ���ǰָ����ָ�ĸ����ĸ������pmax��¼ 
		pmax = *p[0];
		for(int j = 0; j < n; j++){
			if(*p[j] > pmax){
				pmax = *p[j];
			}
		}
		for(int j = 0; j < n; j++){//����ÿһ��ָ�룬��ָ����С�����ֵʱָ�����,ֱ����ָ����>=pmax 
			while((*p[j]<person_thing[j][thing_quantity[j]-1])&&(*p[j] < pmax)){//��ָ�벻Խ���������ƶ�ָ�룬����Ѿ��Ƶ�ĩβ��ָ�벻���ƶ� 
				p[j]++;
				if(*p[j]==person_thing[j][thing_quantity[j]-1]){//�����ǰָ���Ѿ�������ĩβ��׼����һ������ѭ�� 
					i = i+min_thing_quantity;
					break;
				}
			}
		}
		for(int j = 0; j < n; j++){//�ж�ָ��ͣ��λ����ָ�����Ƿ����pmax 
			if(*p[j] != pmax){//������ǣ���Ǹü���û�����Ԫ�� 
				g[j] = 0;
			}
			else{
				g[j] = 1;//����ǣ���Ǹü��������Ԫ�� 
			}
		}
		s = 0;
		for(int j = 0; j < n; j++){//ͳ��n���������м������Ԫ�� 
			s = s + g[j];
		}
		if(s==n){//�����n����֤���������м��Ϲ��е� 
			cout << pmax <<" ";//������Ԫ�غͿո� 
			m++;//���� 
			for(int j = 0; j < n; j++){
				if(*p[j]==person_thing[j][thing_quantity[j]-1]){//�����ǰָ���Ѿ��Ѿ��ƶ�������ĩβ��׼������ѭ�� 
					i = i+min_thing_quantity;
				}
			}
			for(int j = 0; j < n; j++){//�ҵ�һ����ͬԪ�غ�����ָ��ͬʱ����һλ 
				p[j]++;
			}
		}
	}
	if(m==0) cout<<"no";
	t = clock();
	cout <<t;
	return 0;
}

