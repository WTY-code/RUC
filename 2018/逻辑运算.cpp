#include<iostream>        
#include<cstring>        
#include<cstdio>        
#include<cctype>        
#include<stack>        
using namespace std;        
stack<char>opter;        
stack<int>opval;        
char condition[100] = "1|!!!1 & 0", new_condition[100];        
//change the condition        
void change_order(char old[], char neww[])        
{        
    int i = 0, j = 0;        
    char a;        
    a = old[i];        
    while(a!='\0'){        
        if(a==32){        
            i++;        
            a = old[i];        
        }        
        if(isdigit(a)){        
            neww[j] = a;        
            j++;        
            i++;        
            a = old[i];        
        }        
        if(a=='&'){        
            neww[j] = a;        
            j++;        
            i++;        
            a = old[i];        
        }        
        if(a=='|'){        
            neww[j] = a;        
            j++;        
            i++;        
            a = old[i];        
        }        
        if(a=='!'){        
            neww[j] = a;        
            j++;        
            i++;        
            a = old[i];        
        }        
    }      
    int len = strlen(neww);      
      neww[len] = '#';      
      neww[len+1] = '\0';    
      neww[len+2] = '\0';      
    return;        
}        
        
int getindex(char theta)        
{        
    int index = 0;        
    switch(theta)        
    {        
        case '!':        
            index = 0;        
            break;        
        case '&':        
            index = 1;        
            break;        
        case '|':        
            index = 2;        
            break;        
        case '#':        
            index = 3;        
        default: break;        
    }        
    return index;        
}        
        
char getPriority(char theta1, char theta2)        
{        
    const char priority[][4] =         
    {        
        {'>','>','>','>'},        
        {'<','>','>','>'},        
        {'<','<','>','>'},        
        {'<','<','<','='},        
    };        
    int index1 = getindex(theta1);        
    int index2 = getindex(theta2);        
    return priority[index1][index2];        
}        
        
int calculate(int b, char theta, int a)        
{        
    switch(theta)        
    {        
        case '!':        
            return (!a)&&b;        
        case '&':        
            return b&&a;        
        case '|':        
            return b||a;        
        default: break;        
    }        
}        
        
int getAnswer(char order[])//new_condition        
{        
    opter.push('#');        
    int counter = 0,i = 0,x = 0;        
    char c = order[i];        
    while(c!='#'||opter.top()!='#'){        
        if(isdigit(c)){        
            if(counter==1&&x==0){        
                int t = opval.top();        
                opval.pop();        
                opval.push(t*10+(c-'0'));        
                counter = 1;        
            }        
            if(counter==0){        
                opval.push(c-'0');        
                counter++;        
            }        
            i++;        
            c = order[i];        
        }        
        else{        
            counter = 0;        
            x = 0;        
            switch(getPriority(opter.top(),c))        
            {        
                case '<':        
                    opter.push(c);        
                    i++;        
                    c = order[i];        
                    break;        
                case '=':        
                    opter.pop();        
                    i++;        
                    c = order[i];        
                    break;        
                case '>':        
                    char theta = opter.top();        
                    opter.pop();        
                    int a = opval.top();        
                    opval.pop();      
                    if(theta=='!'){      
                        opval.push(1);      
                    }        
                    int b = opval.top();        
                    opval.pop();        
                    opval.push(calculate(b,theta,a));        
            }        
        }        
    }        
    return opval.top();         
}        
int main()        
{        
    int n = 0, ans = -1;        
    cin >> n;    
    getchar();          
    for(int i = 0; i < n; i++){    
//      getchar();    
        while(!opter.empty()) opter.pop();        
        while(!opval.empty()) opval.pop();    
            
        gets(condition);    
//      getchar();    
        change_order(condition, new_condition);     
//      cout<<condition<<"  "<<new_condition<<" ";       
        int ans = getAnswer(new_condition);        
        cout<<ans<<endl;        
        getchar();        
    }        
    return 0;        
}        
