#include<iostream>
#include "circle.h"

Circle::Circle(double r)
{
    _r = r;
}
double Circle::getArea(){
    return 3.1415926*_r*_r;
}
double Circle::getPerimeter(){
    return 2*3.1415926*_r;
}
