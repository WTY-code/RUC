#include "line.h"
#include <math.h>
#include "point.h"
using namespace std;
Line::Line(){}
Line::Line(Point &a, Point &b):pt_a(a),pt_b(b){}
Line::Line(Line &line):pt_a(line.pt_a),pt_b(line.pt_b){}
double Line::getlenth(){
    this->lenth = sqrt(pow(pt_a.getX()-pt_b.getX(),2.0)+pow(pt_a.getY()-pt_b.getY(),2.0));
/*    if(lenth==0){

    }*/
    return this->lenth;
}
Point Line::getpointa(){
    return pt_a;
}
Point Line::getpointb(){
    return pt_b;
}
    //if lenth==0 chonghe
