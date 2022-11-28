 #include<iostream>
 #include<iomanip>
 using namespace std;
 int main()
 {
 	int n = 0;
 	cin>> n;
 	for(int i = 1; i <= n; i++){
 		for(int j = 1; j <= i-1; j++){
 			cout<<"    ";
		 }
		for(int j = i; j<=n; j++){
			cout<<setw(4)<<i*j;
		}
		 cout<<endl;
	 }
	 return 0;
 }
