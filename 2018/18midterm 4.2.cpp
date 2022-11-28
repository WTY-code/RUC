//±éÀú·¨ 
#include<iostream>
using namespace std;
int main()
{
	int n = 0, m = 0;
	int thing_quantity[25] = {0}, g[110] = {0};
	int person_thing[25][110] = {{0}};
	cin >> n;
	for(int i = 1; i <= n; i++){
		cin >> thing_quantity[i];
		for(int j = 1; j <= thing_quantity[i]; j++){
			cin >> person_thing[i][j];
		}
	}
	for(int i = 1; i <= thing_quantity[1]; i++){
		int k = i;
		for(int j = i + 1; j <= thing_quantity[1]; j++){
			if(person_thing[1][j] < person_thing[1][k]){
				k = j;
			}
		}
		if(k != i){
			int tmp = person_thing[1][i];
			person_thing[1][i] = person_thing[1][k];
			person_thing[1][k] = tmp;
		}
	}
	for(int i = 2; i <= n; i++){
		for(int k = 1; k <= thing_quantity[1]; k++){
			for(int j = 1; j <= thing_quantity[i]; j++){
				if(person_thing[1][k] == person_thing[i][j]){
					g[k]++;
				}
			}
		}
	}
	for(int k = 1; k <= thing_quantity[1]; k++){
		if(g[k] == n - 1 ){
			m++;
			cout << person_thing[1][k] <<" ";
		}
	}
	if(m == 0) cout << "NO";
	return 0;
}
