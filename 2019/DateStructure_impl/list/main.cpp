#include <stdlib.h>
#include <iostream>
using namespace std;

#define DEFAULT_SIZE 50
#define INCREMENTAL_SIZE 5
#define ElemType int

typedef struct{
    int size;//线性表中元素个数
    int capacity;//当前线性表最多能容纳的元素个数
    ElemType * elements;
}List;

//初始化一个线性表，将 size 置为0，根据capacity分配数据空间
// 若分配成功，返回 true；否则，返回 false
bool InitList(List & L){
    //cout<<"InitList"<<endl;
    L.size=0;
    L.capacity=DEFAULT_SIZE;
    L.elements=(ElemType*)malloc(L.capacity*sizeof(ElemType));
    if(L.elements==NULL){
       //cout<<"malloc failed!"<<endl;
        return false;
    }
    else{
        for(int i=0; i<L.capacity; i++){
            L.elements[i]=0;
        }

        //cout<<"malloc succeed"<<endl;
        return true;


    }
}

//将数据空间收回
void DestroyList(List & L){
    //cout<<"destroy"<<endl;
    if(L.elements==NULL){return;}
    else{
        free(L.elements);
        L.elements=NULL;
        if(L.elements!=NULL){
            cout<<"destroy the list failed"<<endl;
        }

    }
    return;
}

//将线性表清空
void ClearList(List & L){
    for(int i=0; i<L.size; i++){
        L.elements[i]=0;
    }
    L.size=0;
}

//获取线性表L中第i个位置的元素, i= 1,..., size
//若 1<=i<=size, 将elements[i-1] 赋给e 指向 的空间并返回true
//否则 返回false
bool GetElem(List L, int i, ElemType & e){
    if(i>=1&&i<=L.size){
        e=L.elements[i-1];
        return true;
    }
    else{
        return false;
    }
}

//若 1<=i<=size, 将e 放到L的第i个位置, return true
//否则，return  false
bool PutElem(List &L, int i, ElemType e){
    if(i>=1&&i<=L.size){
        L.elements[i-1]=e;
        return true;
    }
    else{
        return false;
    }
}

//在线性表中的第i个位置插入元素 e
//若 i<=0  返回 false
//若 i > size ，则 elements[size + 1]= e
//否则，elements[i-1] = e, 将线性表中原有的从 i-1 到 size 的元素顺序后移一个位置
//注意： 1 size++   2 若线性表长度不够，须动态增加
bool ListInsert(List & L, int i, ElemType e){
    if(i<=0){return false;}
    if(L.size<L.capacity){
        if(i<=L.size){
            for(int j=L.size; j>=i; j--){
                L.elements[j]=L.elements[j-1];
            }
            L.elements[i-1]=e;
            L.size++;
            return true;
        }
        if(i>L.size){
            if(i<=L.capacity){
                L.elements[L.size]=e;
                L.size++;
                return true;
            }
            if(i>L.capacity){
                //L.elements=(ElemType*)realloc(L.elements,(L.capacity+INCREMENTAL_SIZE)*sizeof(ElemType));
                //L.capacity+=INCREMENTAL_SIZE;
                L.elements=(ElemType*)realloc(L.elements,(i-L.size)*sizeof(ElemType));
                L.capacity+=(i-L.size);
                L.elements[L.size]=e;
                L.size++;
                return true;
            }
        }

    }
    if(L.size==L.capacity){
        if(i<=L.size){
            L.elements=(ElemType*)realloc(L.elements,(L.size+1)*sizeof(ElemType));
            L.capacity+=1;
            for(int j=L.size; j>=i; j--){
                L.elements[j]=L.elements[j-1];
            }
            L.elements[i-1]=e;
            L.size++;
            return true;
        }
        if(i>L.size){
            L.elements=(ElemType*)realloc(L.elements,(i-L.size)*sizeof(ElemType));
            L.capacity+=(i-L.size);
            L.elements[L.size]=e;
            L.size++;
            return true;
        }
    }
}

//返回L中第1个与e满足关系compare的数据元素的位置；若不存在这样的数据元素，则返回0
// compare为一个函数指针，其返回值为bool，第一个参数线性表中的某个元素,第二个参数为e;
//若满足compare关系，返回true；否则返回false
bool compare(ElemType list_e, ElemType e){
    if(list_e==e){
        return true;
    }
    return false;
}
int LocateElem(List L, ElemType e, bool(* compare)(ElemType, ElemType)){
    for(int i=0; i<L.size; i++){
        if(compare(L.elements[i],e)){
            return i+1;
        }
    }
    return 0;
}

//若 i<1 或 i>size, 返回 false
//否则，删除 第 i 个位置元素(elements[i-1])，将其值存入e指向的空间，并返回true
//注意： 1. size--; 2.须将 i+1, ..., size 位置的元素前移一位
bool ListDelete(List &L, int i, ElemType & e){
    if(i<1||i>L.size){return false;}
    else{
        e=L.elements[i-1];
        //L.size--;
        for(int j=i; j<L.size-1; j++){
            L.elements[j-1]=L.elements[j];
        }
        L.size--;
        return true;
    }
}

//依次对L的每个数据元素调用函数 visit，若任意的visit返回false，则本函数返回false；否则，返回true
bool visit(ElemType list_e){
    if(list_e>=0){
        return true;
    }
    return false;
}
bool ListTraverse(List L, bool(* visit)(ElemType)){
    for(int i=0; i<L.size; i++){
        if(!visit(L.elements[i])){
            return false;
        }
    }
    return true;
}

//若线性表为空，返回true；否则返回false
bool ListEmpty(List L){
    if(L.size==0){return true;}
    else{return false;}
}

//返回线性表中元素个数
int ListLength(List L){
    return L.size;
}

//若cur_e之前存在一个元素，则将其值赋给 pre_e, 并返回 true;
//否则，返回 false
bool PriorElem(List L, ElemType cur_e, ElemType &pre_e){
    if(cur_e==L.elements[0]){return false;}
    else{
        for(int i=1; i<L.size; i++){
            if(L.elements[i]==cur_e){
                pre_e=L.elements[i-1];
                return true;
            }
        }
    }
}

//若cur_e之后存在一个元素，则将其值赋给 next_e, 并返回 true;
//否则，返回 false
bool NextElem(List L, ElemType cur_e, ElemType &next_e){
    if(cur_e==L.elements[L.size-1]){return false;}
    else{
        for(int i=0; i<L.size-1; i++){
            if(L.elements[i]==cur_e){
                next_e=L.elements[i+1];
                return true;
            }
        }
    }
}


//打印线性表中的内容
void PrintList(List L){
    //cout<<"PrintList"<<endl;
    for(int i=0; i<L.size; i++){
        cout<<L.elements[i]<<" ";
    }
    cout<<endl;
}
//test
int main()
{
    //bool InitList(List & L);
    //create a list and initiate
    cout<<"bool InitList(List & L);"<<endl;
    List L;
    InitList(L);
    cout<<endl;

    cout<<"input size"<<endl;
    cin>>L.size;
    //cout<<"L.size is "<<L.size<<endl<<endl;

    //input elements
    cout<<"input elements"<<endl;
    for(int i=0; i<L.size; i++){
        cin>>L.elements[i];
    }
    cout<<endl;
/*
    //check elements right or not
    for(int i=0; i<L.size; i++){
        cout<<L.elements[i]<<" ";
    }
    cout<<endl;
*/
    //bool GetElem(List L, int i, ElemType & e);
    cout<<"bool GetElem(List L, int i, ElemType & e);"<<endl;
    int choice1=0;
    ElemType e1;
    cout<<"which element do you want to get? Input a int from 1 to size"<<endl;
    cin>>choice1;
    GetElem(L,choice1,e1);
    cout<<"your choice is "<<choice1<<" and the element is "<<e1<<endl<<endl;

    //bool PutElem(List &L, int i, ElemType e);
    cout<<"bool PutElem(List &L, int i, ElemType e);"<<endl;
    cout<<"input your choice"<<endl;
    int choice2=0;
    ElemType e2;
    cin>>choice2;
    cout<<"which int do you want to put"<<endl;
    cin>>e2;
    PutElem(L,choice2,e2);
    PrintList(L);
    cout<<"you put "<<e2<<" in L.elements["<<choice2-1<<"], L.elements["<<choice2-1<<"] is "<<L.elements[choice2-1]<<endl;
    cout<<endl;

    //bool ListInsert(List & L, int i, ElemType e);
    cout<<"bool ListInsert(List & L, int i, ElemType e);"<<endl;
    int choice3=0;
    ElemType e3;
    cout<<"input choice"<<endl;
    cin>>choice3;
    cout<<"input element"<<endl;
    cin>>e3;
    ListInsert(L,choice3,e3);
    cout<<"now list is:"<<endl;
    PrintList(L);
    cout<<endl;

    //int LocateElem(List L, ElemType e, bool(* compare)(ElemType, ElemType))
    cout<<"int LocateElem(List L, ElemType e, bool(* compare)(ElemType, ElemType))"<<endl;
    cout<<"in this version, compare is choosing the first elements equal to e"<<endl;
    ElemType e4;
    cout<<"input e"<<endl;
    cin>>e4;
    cout<<LocateElem(L,e4,compare)<<endl<<endl;

    //bool ListTraverse(List L, bool(* visit)(ElemType))
    cout<<"in this version, visit is to check every element>=0,if so return true,or false"<<endl;
    cout<<ListTraverse(L,visit)<<endl<<endl;

    //bool ListEmpty(List L)
    cout<<"bool ListEmpty(List L)"<<endl;
    cout<<"check if list is empty :"<<ListEmpty(L)<<endl<<endl;

    //int ListLength(List L)
    cout<<"int ListLength(List L)"<<endl;
    cout<<"get listlength: "<<ListLength(L)<<endl<<endl;

    //bool PriorElem(List L, ElemType cur_e, ElemType &pre_e)
    cout<<"bool PriorElem(List L, ElemType cur_e, ElemType &pre_e)"<<endl;
    ElemType cur_e,pre_e;
    cout<<"input cur_e"<<endl;
    cin>>cur_e;
    PriorElem(L,cur_e,pre_e);
    cout<<"before "<<cur_e<<" is "<<pre_e<<endl<<endl;

    //bool NextElem(List L, ElemType cur_e, ElemType &next_e)
    cout<<"bool NextElem(List L, ElemType cur_e, ElemType &next_e)"<<endl;
    ElemType next_e;
    cout<<"input cur_e"<<endl;
    cin>>cur_e;
    NextElem(L,cur_e,next_e);
    cout<<"after "<<cur_e<<" is "<<next_e<<endl<<endl;

    //void ClearList(List & L)
    cout<<"void ClearList(List & L)"<<endl;
    ClearList(L);
    cout<<endl;

    //bool ListEmpty(List L)
    cout<<"bool ListEmpty(List L)"<<endl;
    cout<<ListEmpty(L)<<endl;
    cout<<endl;

    //cout<<"now list is: "<<endl;
    //PrintList(L);
    DestroyList(L);
    cout<<"END"<<endl;
}

