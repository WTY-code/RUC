#include "point.h"

Point::Point(){} 
Point::Point(double pa, double pb){
    this->_pa = pa;
    this->_pb = pb;
}
Point::Point(const Point &pt){
    _pa = pt.getX();
    _pb = pt.getY();
}

double Point::getX() const{
    return this->_pa;
}
double Point::getY() const{
    return this->_pb;
}
