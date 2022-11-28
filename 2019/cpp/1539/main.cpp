#include <iostream>
#include <cmath>
using namespace std;


class Point
{
private:
    int x, y;

public:
    Point(){//???
    }
    Point( int a, int b ): x(a), y(b){
    }
    void SetX( int );           //  设置 X 值
    void SetY( int );           //  设置 Y 值
    void SetXY( int, int );     //  同时设置 X Y 值
    int  GetX()        const;   //   获得 X 值
    int  GetY()        const;   //   获得 Y 值
    void Print()       const;   //   打印点信息
};
// Point类的实现
void Point::SetX( int a )
{
    x= a;
}

void Point::SetY( int a )
{
    y= a;
}

void Point::SetXY( int a, int b )
{
    x= a, y= b;
}

int Point::GetX() const
{
    return x;
}

int Point::GetY() const
{
    return y;
}

void Point::Print() const
{
    cout << "(" << x << ", " << y << ")";
}




class Circle
{
private:
    int   radius;
    Point centre;
    static int nCircleCount;


public:
    Circle();//???
    Circle( int );
    Circle( int, Point );
    Circle( int, int, int );

    void SetRadius( int );
    void SetCentre( int, int );
    void SetCentre( Point );

    int   GetRadius()      const;
    Point GetCentre()      const;

    void Print()          const;
    int Relation( Circle& );        //判断圆与圆的关系
    static int getCount(){
        return nCircleCount;
    }
    // 此处填空，补充必要的成员函数
    ~Circle() {nCircleCount--;}



};



//--------------------------------------
// Circle类的实现,请补充缺少的类实现代码

int Circle::nCircleCount = 0;


Circle::Circle(int r){
    radius = r;
    nCircleCount++;
}


Circle::Circle(int r, int x, int y){
    radius = r;
    centre.SetXY(x,y);
    nCircleCount++;
}


Circle::Circle(int r, Point p) : centre(p){
    radius = r;
    nCircleCount++;
}


void Circle::SetRadius( int r){
    radius = r;
}


void Circle::SetCentre( int x, int y){
    centre.SetXY(x, y);
}


void Circle::SetCentre( Point p){
    centre.SetX(p.GetX());//???
    centre.SetY(p.GetY());//centre.y = p.GetY();?????
}


Point Circle::GetCentre()      const{
     return centre;//&centre;?????
}


//已提供的部分成员函数
//打印circle信息
void Circle::Print() const
{
    centre.Print();
    cout << " ";
    cout << "Radius is " << radius << endl;
}


//得到circle的半径
int Circle::GetRadius() const
{
    return radius;
}


// 判断2个circle的位置关系，相切、相离或者包含；
// 返回值1表示相切；2表示相离；3表示包含；4表示相交
int Circle::Relation ( Circle& t )
{
    double dis= sqrt( (double)(t.GetCentre().GetX()- centre.GetX())* (t.GetCentre().GetX()- centre.GetX())+
                      (t.GetCentre().GetY()- centre.GetY())* (t.GetCentre().GetY()- centre.GetY()) );

    double r1= radius+ t.GetRadius();
    double r2= abs( radius- t.GetRadius() );

    if( dis== r1 || r2== dis  )
        return 1; // 表示这两个圆相切
    else if( dis> r1 )
        return 2; // 表示两圆相离
    else if( dis< r2 )
        return 3; // 表示两圆包含
    else
        return 4; // 表示两圆相交
}


int main()
{
    int k, i, r, x, y;
    Circle *pAry[5]={NULL}, *pTmp;
    // 读入5个circle
     for ( i=0; i<5; i++ ){
        cin >> r >> x >> y;
        pAry[i] = new Circle( r, Point( x, y ) ) ;
        pAry[i]->Print();
    }
    cout << "The number of Circle is " << Circle::getCount() << endl;


    // 读入需要修改的一个circle的信息
    cin >> k >> r >> x >> y ;
    pAry[k-1]->SetCentre( x, y );
    pAry[k-1]->SetRadius( r );
    cout << pAry[k-1]->GetRadius() << " ("
         << pAry[k-1]->GetCentre().GetX() << ", "
         << pAry[k-1]->GetCentre().GetY() << ") " << endl;
    cout << "The number of Circle is " << Circle::getCount() << endl;

    // 读入最后一个circle，删除与该circle有任何重合部分的其他circle
    cin >> r >> x >> y;
    pTmp = new Circle(r, x, y);

    for ( i=0; i<5; i++ )
    {
        if ( pTmp->Relation(*pAry[i]) != 2 ){
            delete pAry[i];
            pAry[i] = NULL;
        }
    }
    cout <<"this is a test:" << pTmp->GetCentre().GetY() << endl;

    delete pTmp;
    pTmp = NULL;

    cout << "The number of Circle is " << Circle::getCount() << endl;
    return 0;
}
