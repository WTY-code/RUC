#include <iostream>
#include <malloc.h>

using namespace std;
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
#define SElemType int

typedef struct{
    SElemType *base;//在栈构造之前和销毁之后为NULL
    SElemType *top;
    int stacksize;
}SqStack;

bool InitStack(SqStack &S){
    //构造一个空栈
    S.base=(SElemType * )malloc(STACK_INIT_SIZE*sizeof(SElemType));
    if(!S.base){
        exit(1);
    }
    S.top=S.base;
    S.stacksize=STACK_INIT_SIZE;
    return true;
}
bool DestroyStack(SqStack &S){
    //销毁S，S不再存在
    free(S.base);
    S.base=NULL;
    S.top=NULL;
    if(S.base) return false;
    return true;
}
bool ClearStack(SqStack &S){
    //把S置为空栈
    if(!S.base) return false;
    S.top=S.base;
    S.stacksize=0;
    return true;
}
bool StackEmpty(SqStack S){
    //若栈为空栈，返回true，否则返回false
    if(S.base==S.top) return true;
    return false;
}
int StackLength(SqStack S){
    //返回栈S中元素的个数，即栈的长度
    int len=0;
    len=S.top-S.base;
    return len;
}
bool GetTop(SqStack &S, SElemType &e){
    //若栈不空，则用e返回栈顶元素，并返回true，否则false
    if(S.base==S.top) return false;
    e= *(S.top-1);
    return true;
}
bool Push(SqStack &S, SElemType e){
    //插入元素e为新的栈顶元素
    if(S.top-S.base>=S.stacksize){
        S.base=(SElemType * )realloc(S.base, (S.stacksize+STACKINCREMENT)*sizeof(SElemType));
        if(!S.base) exit(1);
        S.top=S.base+S.stacksize;
        S.stacksize+=STACKINCREMENT;
    }
    *S.top=e;
    S.top++;
    return true;
}
bool Pop(SqStack &S, SElemType &e){
    //若栈不空，则删除S的栈顶元素，用e返回其值，并返回true，否则false
    if(S.base==S.top) return false;
    S.top--;
    e=*S.top;
}
void PrintStack(SqStack S){
    for(int i=0; i<StackLength(S); i++){
        cout<<S.base[i]<<" ";
    }
    cout<<endl;
}
int main()
{
    SqStack S;
    InitStack(S);
    for(int i=1; i<11; i++){
        Push(S,i);
    }
    PrintStack(S);
    cout<<"length is "<<StackLength(S)<<endl;
    SElemType e;
    GetTop(S,e);
    cout<<"GetTop: "<<e<<endl;
    Pop(S,e);
    cout<<"Pop: e is "<<e<<endl;
    cout<<"now stack is ";
    PrintStack(S);
    cout<<"length is "<<StackLength(S)<<endl;
    ClearStack(S);
    cout<<"empty: "<<StackEmpty(S)<<endl;
    cout<<"destroy stack: "<<DestroyStack(S)<<endl;
    cout << "Hello World!" << endl;
    return 0;
}
