#include <iostream>//QT运行结果错误

#include <math.h>
using namespace std;

class Complex{
public:
    Complex(double a , double b);//a = 0,b = 0出错
    Complex(double a);
    void add(Complex cc);
    void show();
    void sub(Complex cc);
private:
    double _a, _b;
};
Complex::Complex(double a, double b){
    _a = a;
    _b = b;
}
Complex::Complex(double a){
    _a = a;
    _b = 0;
}
void Complex::add(Complex cc){
    if(cc._b == 0){
        this->_a += cc._a;
    }
    else{
        this->_a += cc._a;
        this->_b += cc._b;
    }
}
void Complex::show(){
    if(this->_a!=0){
        if(this->_b>0) cout<<this->_a<<"+"<<this->_b<<"i"<<endl;
        else if(this->_b==0) cout<<this->_a<<endl;
        else cout<<this->_a<<this->_b<<"i"<<endl;
    }
    else{
        if(this->_b>0) cout<<this->_b<<"i"<<endl;
        else if(this->_b==0) cout<<"0"<<endl;
        else cout<<this->_b<<"i"<<endl;
    }
}
void Complex::sub(Complex cc){
    if(cc._b == 0){
        this->_a -= cc._a;
    }
    else{
        this->_a -= cc._a;
        this->_b -= cc._b;
    }
}
int main()
{
    double re, im;
    cin >> re >> im;
    Complex c1(re, im);        // 用re, im初始化c1
    cin >> re;
    Complex c2 = re;             // 用实数re初始化c2

    c1.show();
    c2.show();
    c1.add(c2);         // 将C1与c2相加，结果保存在c1中
    c1.show();          // 将c1输出
    c2.sub(c1);            // c2-c1，结果保存在c2中
    c2.show();            // 输出c2

    return 0;
}
