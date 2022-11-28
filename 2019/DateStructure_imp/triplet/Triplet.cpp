/*引用引用引用引用引用引用！！！！！！！ 
*
*
*/
#include <iostream>

using namespace std;

#define ElemType int

typedef ElemType * Triplet;

void InitTriplet(Triplet &T, int x, int y, int z){
    //cout<<"InitTriplet"<<endl;
    T=new ElemType(3);
    if(!T){
        cout<<"malloc failed!"<<endl;
        exit(1);
    }
    else{
        T[0]=x;
        T[1]=y;
        T[2]=z;
    }
    /*for(int i=0; i<3; i++){
    	cout<<T[i]<<" ";
	}
	//cout<<endl;
    //free(T);*/
    return;
}
void PrintTriplet(Triplet &T){
    for(int i=0; i<3; i++){
        //cout<<"T["<<i<<"]:"<<T[i]<<" ";
        cout<<T[i]<<" ";
    }
    cout<<endl;
    return;
}
bool IsAscending(Triplet T){
    if(T[0]>T[1]){
        return false;
    }
    else{
        if(T[1]>T[2]){
            return false;
        }
    }
    return true;
}
bool IsDescending(Triplet T){
    if(T[0]<T[1]){
        return false;
    }
    else{
        if(T[1]<T[2]){
            return false;
        }
    }
    return true;
}
void Get(Triplet &T, int i, ElemType &e){//e是否用引用？
    //cout<<"Get T["<<i-1<<"]="<<T[i-1]<<endl;
	 
    e=T[i-1];
    //cout<<e<<endl;
    return;
}
void Put(Triplet &T, int i, ElemType e){
    T[i-1]=e;
    return;
}
ElemType Max(Triplet &T, ElemType &e){//???用引用？？？
    ElemType max=-1000;
    for(int i=0; i<3; i++){
        if(T[i]>max){
            max=T[i];
        }
    }
    e=max;
    return e;
}
ElemType Min(Triplet &T, ElemType &e){
    ElemType min=1000;
    for(int i=0; i<3; i++){
    	//cout<<"T[i]="<<T[i]<<" "<<"min is "<<min<<endl;
        if(T[i]<min){
            min=T[i];
            //cout<<"after change min is "<<min<<endl; 
        }
    }
    e=min;
    return e;
}
void Add(Triplet &T1, Triplet &T2, Triplet &T){
    for(int i=0; i<3; i++){
        T[i]=T1[i]+T2[i];
    }
    return;
}
void Subtract(Triplet &T1, Triplet &T2, Triplet &T){
    for(int i=0; i<3; i++){
        T[i]=T2[i]-T1[i];
    }
    return;
}



int main()
{
    Triplet T;
    InitTriplet(T, 1, 2, 3);//
    PrintTriplet(T);//
    cout<<"Is T in ascending order:"<<IsAscending(T)<<endl;
    cout<<"Is T in descending order:"<<IsDescending(T)<<endl;
    ElemType e;
    Get(T, 2, e);//
    cout<<"The 2nd element of T is:"<<e<<endl;
    Put(T, 2, 10);
    Get(T, 2, e);
    cout<<"After executing Put(T, 2, 10), the 2nd element of T is:"<<e<<endl;
    Max(T, e);
    cout<<"max element of T is:"<<e<<endl;
    Min(T, e);
    cout<<"Min element of T is:"<<e<<endl;

    Triplet T1, T2;
    InitTriplet(T1, 1, 2, 3);
    InitTriplet(T2, 4, 5, 6);
    Add(T1, T2, T);
    cout<<"<1,2,3> + <4,5,6>="<<endl;
    PrintTriplet(T);
    Subtract(T1,T2, T);
    cout<<"<4,5,6> - <1,2,3>="<<endl;
    PrintTriplet(T);
    return 0;
}

