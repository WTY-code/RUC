#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "rectangle.h"
#include "point.h"
#include "line.h"
#include "quadrilateral.h"
#include "parallelogram.h"
#include <vector>
using namespace std;

class Rectangle:public Parallelogram
{
public:
    Rectangle();
    Rectangle(vector<Point>vertices);
//    void diagonalline();
    bool verify();
private:
    Line lin13,lin24;
};

#endif // RECTANGLE_H
