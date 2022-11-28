#include "rectangle.h"
#include "point.h"
#include "line.h"
#include "quadrilateral.h"
#include "parallelogram.h"
#include <vector>
using namespace std;

Rectangle::Rectangle(){}
Rectangle::Rectangle(vector<Point>pts):Parallelogram(pts),lin13(pts[0],pts[2]),lin24(pts[1],pts[3]){}
bool Rectangle::verify(){
    if(Parallelogram::verify()==0){return 0;}
    else if(this->lin13.getlenth()==this->lin24.getlenth()) return 1;
    else return 0;
}
