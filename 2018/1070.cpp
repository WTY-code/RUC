#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	int n = 0, a[102] = {0};
	cin>>n;
	for(int i = 1; i <= n+1; i++){
		cin>>a[i];
	}
	if(n==1){
		if(a[1]>0){
			cout<<a[1]<<"x";
			if(a[2]>0)cout<<"+"<<a[2];
			else if(a[2]<0)cout<<a[2];
		}
	}
	if(n>=2){
	    if(a[1]<0){
		    cout<<"-"<<abs(a[1])<<"x"<<"^"<<n;
 	    }
	    else if(a[1]>0){
		    cout<<a[1]<<"x"<<"^"<<n;
	    }
	    for(int i = n-1; i >= 2; i--){
		    if(a[n+1-i]==0) continue;
		    else{
			    if(a[n+1-i]>0){
				    if(a[n+1-i]==1){
					    cout<<"+"<<"x"<<"^"<<i;
				    }
				    else{
					    cout<<"+"<<a[n+1-i]<<"x"<<"^"<<i;
				    }
			    }
			    else{
	  			    if(a[n+1-i]==-1){
					    cout<<"-"<<"x"<<"^"<<i;
				    }
				    else{
					    cout<<"-"<<abs(a[n+1-i])<<"x"<<"^"<<i;
				    }
			    }
		    }
	    }
	    if(a[n]>0){
	    	cout<<"+"<<a[n]<<"x";
		}
		else if(a[n]<0){
			cout<<"-"<<abs(a[n])<<"x";
		}
    }
	if(a[n+1]<0){
		cout<<"-"<<abs(a[n+1]);
	}
	else if(a[n+1]>0){
		cout<<"+"<<a[n+1];
	}
	return 0;
}
