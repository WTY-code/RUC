#include "parallelogram.h"
#include "point.h"
#include "line.h"

Parallelogram::Parallelogram(){}
Parallelogram::Parallelogram(vector<Point> pts):Quadrilateral(pts){}
bool Parallelogram::verify(){
    if(Quadrilateral::verify()==0){return 0;}
    else if(this->lin12.getlenth()==this->lin34.getlenth()
           &&this->lin23.getlenth()==this->lin41.getlenth()){return 1;}
    else return 0;
}
