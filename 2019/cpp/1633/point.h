#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point();
    Point(double pa, double pb);
    Point(const Point &pt);
    double getX() const;
    double getY() const;
private:
    double _pa,_pb;
};

#endif // POINT_H
