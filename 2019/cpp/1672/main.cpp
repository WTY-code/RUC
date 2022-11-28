#include <iostream>
using namespace std;
//添加代码
#include<stdlib.h>

template <class T>
class CArray3D
{
public:
    CArray3D(T a, T b, T c);
    ~CArray3D();
    T**& operator[] (int i);

private:
    T ***list;
    int _a,_b,_c;

};

template<class T>
CArray3D<T>::CArray3D(T a, T b, T c):_a(a),_b(b),_c(c){
    list=new T **[a];
    for (int i=0;i<a;i++) {
        list[i]=new T*[b];
        for (int j=0;j<b;j++) {
            list[i][j]=new T[c];
        }
    }
}

template<class T>
T**&CArray3D<T>::operator[](int i){
    return list[i];
}

template<class T>
CArray3D<T>::~CArray3D<T>(){
    delete[]list;
}

int main()
{
    CArray3D<int> a(3,4,5);
    int No = 0;
    for( int i = 0; i < 3; ++ i )
        for( int j = 0; j < 4; ++j )
            for( int k = 0; k < 5; ++k )
                a[i][j][k] = No ++;
    for( int i = 0; i < 3; ++ i )
        for( int j = 0; j < 4; ++j )
            for( int k = 0; k < 5; ++k )
                cout << a[i][j][k] << ",";

    CArray3D<char> b(3,4,5);
    char c = 'A';
    for( int i = 0; i < 3; ++i )
        for( int j = 0; j < 4; ++j )
            for( int k = 0; k < 5; ++k )
                b[i][j][k] = c++;
    for( int i = 0; i < 3; ++ i )
        for( int j = 0; j < 4; ++j )
            for( int k = 0; k < 5; ++k )
                cout << b[i][j][k] << ",";

    return 0;
}
