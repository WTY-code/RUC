#include <iostream>
#include <stdlib.h>
using namespace std;

#define ElemType int//define后不加分号！！！

typedef struct LNode{
    ElemType data;
    struct LNode *next;
}*Link, *Position;

typedef struct{
    Link head,tail;
    int len;
}LinkList;

int ListLength(LinkList L){//right
    //获取表中元素个数
    int cnt=0;
    Link p=L.head->next;
    while(p){
        cnt++;
        p=p->next;
    }
    return cnt;
}
Position GetHead(LinkList L){//right
    //返回线性表头结点的位置
    return L.head;
}
Position GetLast(LinkList L){//right
    //返回线性表最后一个结点的位置
    Position p;
    p=L.head->next;
    while(p->next!=NULL){
        p=p->next;
    }
    //cout<<"p is "<<p->data<<endl;
    return p;
}
Position PriorPos(LinkList L, Link p){//right
    //返回p所指结点的直接前驱,若无前驱返回NULL
    Link pre=L.head;
    while(1){
        if(pre->next==p){
            return pre;
        }
        pre=pre->next;
        if(pre==NULL){return pre;}
    }
}
Position NextPos(LinkList L, Link p){//right
    //返回p所指结点的直接后继,若无后继
    if(p->next==NULL){return NULL;}
    return p->next;
}
Position LocatePos(LinkList L, int i){//right
    //获取第i个元素的位置
    //若i不合法报错
    if(i<1||i>ListLength(L)){
        cout<<"error i"<<endl;
    }
    Link p=L.head;
    for(int cnt=0; cnt<i; cnt++){
        p=p->next;
    }
    return p;
}
bool MakeNode(Link &p,ElemType e){//right
    //分配由p指向的值为e的结点，成功返回true,否则false
    p=(LNode*)malloc(sizeof(LNode));
    if(p){
        p->data=e;
        p->next=NULL;
        return true;
    }
    else{
        return false;
    }
}
void FreeNode(Link &p){//???有必要封成函数吗？？？
    //释放p所指结点
    free(p);
}
void InitList(LinkList &L){//right注意要给头结点分配空间
    //构造一个空的链表L
    L.head=(LNode*)malloc(sizeof(LNode));
    L.head->next=NULL;//头结点是否指针为空？
    L.tail=NULL;
    L.len=0;
    return;
}
void PrintList(LinkList L){//right
   Link p=L.head->next;
   while(p){
       cout<<p->data<<" ";
       p=p->next;
   }
   cout<<endl;
}

bool DestroyList(LinkList &L){//不知道为啥总返回0
    //销毁链表L，L 不再存在
    Link q,p=L.head->next;
    while(p){
        q=p->next;
        free(p);
        p=q;
    }
    free(L.head);
    free(L.tail);
    L.len=0;
    if(L.head||L.tail){return false;}
    else{return true;}
}

bool ClearList(LinkList &L){//right
    //将链表L置为空表，并释放原链表的结点空间
    Link q,p=L.head->next;
    L.head->next=NULL;
    while(p){
        q=p->next;
        free(p);
        p=q;
    }
    L.len=0;
    L.tail=p;
    if(L.tail!=NULL){return false;}
    else{return true;}
}
void InsFirst(Link h, Link s){//right
    //已知h指向链表的头结点，将s所指的结点插入在第一个节点之前
    //cout<<"进InsFirst"<<endl;
    s->next=h->next;
    h->next=s;
    return;
}
Link DelFirst(Link h, Link &q){//right
    //已知h指向链表的头结点，删除链表的第一个结点并以q返回
    q=h->next;
    h->next=q->next;
    //怎么用free()？
    return q;
}
void Append(LinkList &L, Link s){//right
    //将指针s所指（彼此以指针相链）的一串结点链接在链表L的最后
    //一个结点之后，并改变L的尾指针指向新的尾结点
    L.tail->next=s;
    while(L.tail->next!=NULL){
        L.tail=L.tail->next;
    }
    return;
}
void InsBefore(LinkList &L, Link &p, Link s){//right
    //已知p指向L中的一个结点，将s所指结点插入在p所指结点之前
    //并修改指针p指向新插入的节点
    //cout<<"进InsBefore"<<endl;
    Link pre;
    pre=PriorPos(L,p);
    s->next=p;
    pre->next=s;
    p=s;
    return;
}
void InsAfter(LinkList &L, Link &p, Link s){//right
    //cout<<"进InsAfter"<<endl;
    //已知p指向L中的一个结点，将s所指结点插入在p所指结点之后
    //并修改指针p指向新插入的节点
    s->next=p->next;
    p->next=s;
    p=s;
    L.tail=GetLast(L);
    return;
}
void InputList(int n, LinkList &L){//right
    ElemType *data;
    data=(ElemType*)malloc((n+1)*sizeof(ElemType));
    data[0]=0;
    cout<<"input elements"<<endl;
    for(int i=1; i<=n; i++){
        cin>>data[i];
    }
    Link s,p=L.head;
    for(int i=1; i<=n; i++){
        MakeNode(s,data[i]);
        InsAfter(L,p,s);
    }
    L.tail=p;
}
bool InsNode(LinkList &L, int i, Link s){//right
    //在第i个结点(不包括头结点)前插入s结点
    //若i不合法返回false
    if(i<1){return false;}
    if(i>=1&&i<=ListLength(L)){
        Link q,pre;
        q=LocatePos(L,i);
        pre=PriorPos(L,q);
        s->next=q;
        pre->next=s;
        return true;
    }
    if(i>ListLength(L)){
        L.tail->next=s;
        L.tail=s;
    }
}
void SetCurElem(Link &p, ElemType e){//right
    //已知p指向L中的一个结点，用e更新p所指结点中数据元素的值
    p->data=e;
    return;
}
ElemType GetCurElem(Link p){//right
    //已知p指向L中的一个结点，返回p所指结点中数据元素的值
    return p->data;
}
bool GetElem(LinkList L, int i, ElemType &e){//right
    //获取链表中第i个元素,用e返回
    //若i<1或大于表长返回false
    if(i<0||i>ListLength(L)) return false;
    Link p=L.head->next;
    for(int cnt=1; cnt<i; cnt++){
        p=p->next;
    }
    e=p->data;
    return true;
}
bool ListEmpty(LinkList L){//right
    //若链表为空返回true否则返回false
    if(L.head->next==NULL){return true;}
    return false;
}
Position LocateElem(LinkList L, ElemType e, bool(* compare)(ElemType,ElemType)){//right
    //返回L中第一个与e满足函数compare()
    //判定关系的元素的位置
    //若不存在这样的元素返回NULL
    Link p=L.head->next;
    while(p){
        if(compare(p->data,e)) return p;
        p=p->next;
    }
    return NULL;
}
bool compare(ElemType e1, ElemType e2){//right
    //判定是否相等
    if(e1==e2)return true;
    else return false;
}
bool ListTraverse(LinkList L, bool(*visit)(ElemType)){//right
    //依次对L的每个元素调用visit()，一旦调用失败则操作失败
    Link p=L.head->next;
    while(p){
        if(!visit(p->data)) return false;
        p=p->next;
    }
    return true;
}
bool visit(ElemType e){//right
    //判断是否非负
    if(e>=0) return true;
    return false;
}


int main()
{
    LinkList L;
    InitList(L);
    int n=0;
    cout<<"input elements number"<<endl;
    cin>>n;
    InputList(n,L);
    PrintList(L);
    cout<<"tail is "<<L.tail->data<<endl;
    cout<<"test InsFirst,InsBefore,InsAfter"<<endl;
    Link Fr,Bf,Af;
    MakeNode(Fr,-1);
    cout<<"Fr is "<<Fr->data<<endl;
    InsFirst(L.head,Fr);
    PrintList(L);
    cout<<"tail is "<<L.tail->data<<endl;
    MakeNode(Bf,0);
    cout<<"Bf is "<<Bf->data<<endl;
    InsBefore(L,L.head->next,Bf);
    PrintList(L);
    cout<<"tail is "<<L.tail->data<<endl;
    MakeNode(Af,4);
    cout<<"Af is "<<Af->data<<endl;
    LNode* p=L.head->next->next;
    //注意！！！如果函数参数直接写L.head->next->next后插入元素的前一个会丢失
    //原因是参数是引用，函数里的语句会改变L.head->next->next的地址指到后加入元素
    InsAfter(L,p,Af);
    PrintList(L);
    cout<<"tail is "<<L.tail->data<<endl;
    Link q;
    DelFirst(L.head,q);
    PrintList(L);
    cout<<"delete "<<q->data<<endl;
    cout<<"tail is "<<L.tail->data<<endl;
    Link ss1,ss2;
    MakeNode(ss1,5);//千万别忘了给节点分配空间呀！！！这只是一个指针，不是声明了就有空间哒！！！
    MakeNode(ss2,6);
    ss1->next=ss2;
    ss2->next=NULL;
    Append(L,ss1);
    cout<<"tail is "<<L.tail->data<<endl;
    PrintList(L);
    SetCurElem(ss1,-5);
    PrintList(L);
    cout<<"L.head->next->next->next is "<<GetCurElem(L.head->next->next->next)<<endl;
    cout<<"lenth is "<<ListLength(L)<<endl;
    ElemType e;
    GetElem(L,4,e);
    cout<<"NO.4 is "<<e<<endl;
    //cout<<"NO.4 is "<<GetElemPos(L,4)->data<<endl;

    cout<<"5's PriorPos"<<PriorPos(L,ss1)->data<<endl;
    cout<<"5's NextPos"<<NextPos(L,ss1)->data<<endl;
    q=LocatePos(L,4);
    cout<<"locatepos NO.4 "<<q->data<<endl;
    cout<<"LocateElem 3 is "<<LocateElem(L,3,compare)->data<<endl;
    cout<<"listtraverse"<<ListTraverse(L,visit)<<endl;
    Link s1,s2;
    MakeNode(s1,9);
    MakeNode(s2,10);
    InsNode(L,7,s1);
    PrintList(L);
    InsNode(L,20,s2);
    PrintList(L);
    cout<<"clearlist "<<ClearList(L)<<endl;
    cout<<"listempty "<<ListEmpty(L)<<endl;
    cout << "Hello World!" << endl;
    return 0;
}
