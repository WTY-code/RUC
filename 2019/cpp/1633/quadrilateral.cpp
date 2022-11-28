#include "quadrilateral.h"
#include "point.h"
#include "line.h"
#include <vector>
#include <math.h>

using namespace std;

Quadrilateral::Quadrilateral(){}
Quadrilateral::Quadrilateral(vector<Point> &pts):lin12(pts[0],pts[1]),lin23(pts[1],pts[2]),//先后顺序？？？
lin34(pts[2],pts[3]),lin41(pts[3],pts[0]),lin13(pts[0],pts[2]),lin24(pts[1],pts[3]){
    for(int i=0; i<4; i++){
        vertices.push_back(pts[i]);
    }
}
/*void Quadrilateral::giveline(){
    lin12(vertices[0],vertices[1]);
    lin23(vertices[1],vertices[2]);
    lin34(vertices[2],vertices[3]);
    lin41(vertices[3],vertices[0]);
}*/
//Quadrilateral(Quadrilateral &qua){}
bool Quadrilateral::verify(){
    if(((vertices[1].getX()-vertices[0].getX())
        *(vertices[2].getY()-vertices[0].getY())
        -(vertices[1].getY()-vertices[0].getY())
        *(vertices[2].getX()-vertices[0].getX()))
        *((vertices[1].getX()-vertices[0].getX())
        *(vertices[3].getY()-vertices[0].getY())
        -(vertices[1].getY()-vertices[0].getY())
        *(vertices[3].getX()-vertices[0].getX()))<0
    ||((vertices[3].getX()-vertices[0].getX())
        *(vertices[1].getY()-vertices[0].getY())
        -(vertices[3].getY()-vertices[0].getY())
        *(vertices[1].getX()-vertices[0].getX()))
        *((vertices[3].getX()-vertices[0].getX())
        *(vertices[2].getY()-vertices[0].getY())
        -(vertices[3].getY()-vertices[0].getY())
        *(vertices[2].getX()-vertices[0].getX()))<0){return 0;}
    else return 1;
}
/*double Quadrilateral::getArea(){
    double area1,area2;
    area1 = 0.5*((vertices[2].getX()-vertices[0].getX())*(vertices[1].getY()-vertices[0].getY())
            -(vertices[2].getY()-vertices[0].getY())*(vertices[1].getX()-vertices[0].getX()));
    area2 = 0.5*((vertices[2].getX()-vertices[0].getX())*(vertices[3].getY()-vertices[0].getY())
            -(vertices[2].getY()-vertices[0].getY())*(vertices[3].getX()-vertices[0].getX()));
    return area1+area2;
}*/
double Quadrilateral::getArea(){
    double p1 = (lin12.getlenth()+lin23.getlenth()+lin13.getlenth())/2.0;
    double p2 = (lin34.getlenth()+lin41.getlenth()+lin13.getlenth())/2.0;
    double area1 = sqrt(p2*(p2-lin34.getlenth())*(p2-lin41.getlenth())*(p2-lin13.getlenth()));
    double area2 = sqrt(p1*(p1-lin12.getlenth())*(p1-lin23.getlenth())*(p1-lin13.getlenth()));
    return area1+area2;
}
double Quadrilateral::getPerimeter(){
    return lin12.getlenth()+lin23.getlenth()+lin34.getlenth()+lin41.getlenth();
}
Line Quadrilateral::getline12(){
    return lin12;
}
Line Quadrilateral::getline23(){
    return lin23;
}
Line Quadrilateral::getline34(){
    return lin34;
}
Line Quadrilateral::getline41(){
    return lin41;
}
Point Quadrilateral::getpoint1(){
    return vertices[0];
}
Point Quadrilateral::getpoint2(){
    return vertices[1];
}
Point Quadrilateral::getpoint3(){
    return vertices[2];
}
Point Quadrilateral::getpoint4(){
    return vertices[3];
}
