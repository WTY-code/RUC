#include<iostream>
using namespace std;
int main()
{
	int m = 0, a = 0, d = 0, g = 0, cc1 = 0, cc2 = 0, cc3 = 0, cc4 = 0, cc5 = 0, cc6 = 0, n = 0;
	cin >> n;
	for(m = 2; m <= 53; m++){
		for(a = 1; a <= 18; a++){
			for(d = 1; d <= 101; d++){
				if((m != a)&&(m != d)&&(a != d)){
					cc1 = 1;
				}
				if(((m > a)&&(m < d))||((m < a)&&(m > d))){
					cc2 = 1;
				}
				if((m % 2) ==0 ){
					cc3 = 1;
				}
				if((d == 8)||(d == 18)||(d == 28)||(d ==38)||(d ==48)
				||(d ==58)||(d ==68)||(d ==78)||(d == 88)||(d == 98)){
					cc4 = 1;
				} 
				if(((a <10)&&(m >= 10))||(a >= 10)){
					cc5 = 1;
				}
				if(((2*m + 6*a + d) > (0.9 * n)) && ((2*m + 6*a + d) <= n)){
					cc6 = 1;
				}
				if((cc1 + cc2 + cc3 + cc4 + cc5 + cc6)==6){
					g = g + 1;
					cout << m <<" "<< a <<" "<< d << endl;
					cc1 = 0;
					cc2 = 0;
					cc3 = 0;
					cc4 = 0;
					cc5 = 0;
					cc6 = 0;
				}
				else{
					cc1 = 0;
					cc2 = 0;
					cc3 = 0;
					cc4 = 0;
					cc5 = 0;
					cc6 = 0;
				}
			}
		}
	}
	if(g == 0){
		cout << "no answer";
	}
	return 0;
}
