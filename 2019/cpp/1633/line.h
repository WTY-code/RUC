#ifndef LINE_H
#define LINE_H
#include "point.h"

class Line
{
public:
    Line();
    Line(Point &a, Point &b);
    Line(Line &line);
    double getlenth();//if lenth==0 chonghe
/*    bool existk(Point pt_a, Point pt_b);
    bool existk(Line &line);*/
    Point getpointa();
    Point getpointb();
private:
    Point pt_a,pt_b;
    double lenth;
};

#endif // LINE_H
