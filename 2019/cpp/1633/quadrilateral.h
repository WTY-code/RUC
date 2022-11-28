#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H
#include "point.h"
#include "line.h"
#include <vector>
using namespace std;

class Quadrilateral
{
public:
    Quadrilateral();
    Quadrilateral(vector<Point>&vertices);
    bool verify();
    double getArea();
    double getPerimeter();
    Line getline12();//以下用作输出line
    Line getline23();
    Line getline34();
    Line getline41();
    Point getpoint1();
    Point getpoint2();
    Point getpoint3();
    Point getpoint4();
//    void giveline();//给四条边赋值的函数
protected:
    vector<Point>vertices;
    Line lin12,lin23,lin34,lin41;//四边形四条边
    Line lin13,lin24;
};

#endif // QUADRILATERAL_H
