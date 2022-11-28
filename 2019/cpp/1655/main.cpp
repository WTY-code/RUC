#include <iostream>
#include <cstdlib>   //stdlib.h库里常用的函数malloc()等
#include <cmath>
using namespace std;
//填入代码

class CShape{
public:
    virtual void PrintInfo() const=0;
    virtual double Area() const=0;
    virtual ~CShape();
};

CShape::~CShape(){}

class Rectangle : public CShape{
public:
    Rectangle(double w, double h);
    void PrintInfo() const;
    double Area() const;

    ~Rectangle();

private:
    double _area;
};

Rectangle::Rectangle(double w, double h)
{
    _area=w*h;
}

void Rectangle::PrintInfo() const
{
    cout<<"Rectangle:"<<_area<<endl;
}

double Rectangle::Area() const
{
    return _area;
}

Rectangle::~Rectangle(){}

class Circle : public CShape{
public:
    Circle(double r);
    void PrintInfo() const;
    double Area() const;
    ~Circle();

private:
    double _area;
};

Circle::~Circle(){}

Circle::Circle(double r){
    _area=3.14*r*r;
}

void Circle::PrintInfo() const
{
    cout<<"Circle:"<<_area<<endl;
}

double Circle::Area() const
{
    return _area;
}

class Triangle : public CShape{
public:
    Triangle(double a,double b,double c);
    void PrintInfo() const;
    double Area() const;
    ~Triangle();

private:
    double _area;
};

Triangle::~Triangle(){}

Triangle::Triangle(double a,double b,double c){
    double p=(a+b+c)*0.5;
    _area=sqrt(p*(p-a)*(p-b)*(p-c));
}

double Triangle::Area() const
{
    return _area;
}

void Triangle::PrintInfo() const
{
    cout<<"Triangle:"<<_area<<endl;
}

int MyCompare(const void * s1, const void * s2) {
    double a1, a2;
    CShape ** p1; // s1, s2 是 void * ，不可写 “* s1”来取得s1指向的内容
    CShape ** p2;
    p1 = (CShape **) s1; //s1,s2指向pShapes数组中的元素，数组元素的类型是CShape *
    p2 = (CShape **) s2; // 故 p1,p2都是指向指针的指针，类型为 CShape **
    a1 = (*p1)->Area(); // * p1 的类型是 Cshape * ,是基类指针，故此句为多态
    a2 = (*p2)->Area();
    if( a1 < a2 )
        return -1;
    else if ( a2 < a1 )
            return 1;
        else
            return 0;
}

int main() {
    int n;
    CShape ** pShapes;

    cin >> n;    //输入几何体的数目n

    pShapes = new CShape*[n];
    for(int i=0; i<n; i++ ) {
        char c;
        cin >> c;
        switch(c) {
        case 'R':
        //若 c 是 ‘R’，则代表一个矩形，本行后面跟着两个整数，分别是矩形的宽和高；
        //填入代码
        {
            double w,h;
            cin >> w >> h;
            Rectangle *R = new Rectangle(w,h);
            pShapes[i]=R;
        }
        break;
        case 'C':
        //若 c 是 ‘C’，则代表一个圆，本行后面跟着一个整数代表其半径；
        //填入代码
        {
            double r;
            cin>>r;
            Circle *C = new Circle(r);
            pShapes[i]=C;
        }
        break;
        case 'T':
        //若 c 是 ‘T’，则代表一个三角形，本行后面跟着三个整数，代表三条边的长度。
        //填入代码
        {
            double a,b,c;
            cin >>a>>b>>c;
            Triangle *T = new Triangle(a,b,c);
            pShapes[i]=T;
        }
        break;
        }
    }

    qsort(pShapes, n, sizeof(CShape*), MyCompare);
    //1 待排序数组首地址 2 数组中待排序元素数量 3 各元素的占用空间大小 4 指向函数的指针

    for(int i=0; i<n; i++)
        pShapes[i]->PrintInfo();
    /*输出格式

    按面积从小到大依次输出每个几何形体的种类及面积。
    每行一个几何形体，输出格式为：
    形体名称：面积*/

    for(int i=0; i<n; i++)
        delete pShapes[i];
    delete [] pShapes;
    return 0;
}
