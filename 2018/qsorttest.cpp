#include<iostream>
#include<cstdlib>
using namespace std;
struct s{
	double aa;
	double bb;
	double cc;
}ary[5];
int cmp(const void*a,const void*b)
{
	struct s* c = (s*)a;
	struct s* d = (s*)b;
	if(c->aa != d->aa ) return c->aa - d->aa;
	if(c->bb != d->bb ) return c->bb - d->bb;
	if(c->cc != d->cc ) return c->cc - d->cc;
	return 0;
}
int main()
{
	struct s ary[5] = {{4.23,3.23,5.23},{5.23,4.23,2.23},{5.23,8.23,2.23},{7.23,6.23,5.23},{9.23,0.23,2.23}};
	qsort(ary,5,sizeof(ary[0]),cmp);
	for(int i = 0; i < 5; i++){
		cout<<ary[i].aa <<" "<<ary[i].bb <<" "<<ary[i].cc <<endl;
	}
	return 0;
}
