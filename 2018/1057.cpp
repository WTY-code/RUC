#include<iostream>
#include<cmath>
using namespace std;
int pow(int x, int y)
{
	int s = 1;
	for(int i = 1; i <= y; i++){
		s = s*x;
	}
	return s;
}
int pa[10000002] = {0}, qa[10000002] = {0}, ra[10000002] = {0};
int main()
{
	int t = 0, b = 0;
	int p[102] = {0}, q[102] = {0}, r[102] = {0};
	int pn[102] = {0}, qn[102] = {0}, rn[102] = {0};
	int pamax[102] = {0}, qamax[102] = {0}, ramax[102] = {0};
	int pb[102] = {0}, qb[102] = {0}, rb[102] = {0};
	cin>>t;
	for(int i = 1;i <= t; i++){
		cin>>p[i]>>q[i]>>r[i];
	}
	for(int i = 1; i <= t; i++){
		for(int j = 7; j >= 0; j--){
			if((p[i]/pow(10,j))!=0){
				pn[i] = j+1;
				break;
			}
		}
	}
	for(int i = 1; i <= t; i++){
		for(int j = 7; j >= 0; j--){
			if((q[i]/pow(10,j))!=0){
				qn[i] = j+1;
				break;
			}
		}
	}
	for(int i = 1; i <= t; i++){
		for(int j = 7; j >= 0; j--){
			if((r[i]/pow(10,j))!=0){
				rn[i] = j+1;
				break;
			}
		}
	}
	for(int i = 1; i <= t; i++){
		for(int j = 1; j <= pn[i]; j++){
			pa[j] = 0;
		}
		for(int j = 1; j <= qn[i]; j++){
			qa[j] = 0;
		}
		for(int j = 1; j <= rn[i]; j++){
			ra[j] = 0;
		}
		pb[i] = 0;
		qb[i] = 0;
		rb[i] = 0;
		for(int j = 1; j <= pn[i]; j++){
			pa[j] = (p[i]/pow(10,j-1))%10;
		}
	    pamax[i] = pa[1];
		for(int j = 1; j <= pn[i]; j++){
			if(pa[j]>pamax[i]){
				pamax[i] = pa[j];
			}
		}
		for(int j = 1; j <= qn[i]; j++){
			qa[j] = (q[i]/pow(10,j-1))%10;
		}
		qamax[i] = qa[1];
		for(int j = 1; j <= qn[i]; j++){
			if(qa[j]>qamax[i]){
				qamax[i] = qa[j];
			}
		}
	    for(int j = 1; j <= rn[i]; j++){
			ra[j] = (r[i]/pow(10,j-1))%10;
		}
		ramax[i] = ra[1];
		for(int j = 1; j <= rn[i]; j++){
			if(ra[j]>ramax[i]){
			    ramax[i] = ra[j];
		    }
		}
		for(b = 2; b <= 16; b++){
			pb[i] = 0;
			qb[i] = 0;
			rb[i] = 0;
			for(int j = 1; j <= pn[i]; j++){
				pb[i] = pb[i]+pa[j]*pow(b,j-1);
			}
			for(int j = 1; j <= qn[i]; j++){
				qb[i] = qb[i]+qa[j]*pow(b,j-1);
			}
			for(int j = 1; j <= rn[i]; j++){
				rb[i] = rb[i]+ra[j]*pow(b,j-1);
			}
			if(((pb[i]*qb[i])==rb[i])&&(b>pamax[i])&&(b>qamax[i])&&(b>ramax[i])){
				cout<<b<<endl;
	            break;
			}
			else if(b==16) cout<<"0"<<endl;
		}
    }
	return 0;	
 } 
