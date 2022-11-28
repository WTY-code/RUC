#include<iostream>
using namespace std;
int main()
{
	int na =0, nb = 0, nc = 0, nd = 0, g = 0, a = 0, b = 0, c = 0, d = 0, s = 0;
	int cc1 = 0, cc2 = 0, cc3 = 0, cc4 = 0, cc5 = 0, cc6 = 0, cc7 = 0, cc8 = 0, cc9 = 0, cc10 = 0;
	int cr[8] = {120, 40, 85, 50, 100, 140, 70, 100};
	cin >> na >> nb >> nc >> nd;
	for(a = 0; a <= 7; a++){
		for(b = 0; b <= 7; b++){
		    for(c = 0; c <= 7; c++){
		    	for(d = 0; d <= 7; d++){
		    		if(na <= cr[a]){
		    			cc1 = 1;
					}
					if(nb <= cr[b]){
						cc2 = 1;
					}
					if(nc <= cr[c]){
						cc3 = 1;
					}
					if(nd <= cr[d]){
						cc4 = 1;
					}
					if(a != b){
						cc5 = 1;
					}
					if(a != c){
						cc6 = 1;
					}
					if(a != d){
						cc7 = 1;
					}
					if(b != c){
						cc8 = 1;
					}
					if(b != d){
						cc9 = 1;
					}
					if(c != d){
						cc10 = 1;
					}
					s = cc1 + cc2 + cc3 + cc4 + cc5 + cc6 + cc7 + cc8 + cc9 + cc10;
					cc1 = 0;cc2 = 0;cc3 = 0;cc4 = 0;cc5 = 0;cc6 = 0;cc7 = 0;cc8 = 0;cc9 = 0;cc10 = 0;
					if(s == 10){
						cout << a+1 <<" "<< b+1 <<" "<< c+1 <<" "<< d+1 << endl;
						g = g + 1;
					}
				}
			}
	    }
	}
	if(g == 0){
		cout << -1;
	}
	return 0;
}
