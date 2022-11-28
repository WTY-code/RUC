#ifndef PARALLELOGRAM_H
#define PARALLELOGRAM_H
#include "point.h"
#include "line.h"
#include <vector>
#include "quadrilateral.h"
using namespace std;

class Parallelogram:public Quadrilateral
{
public:
    Parallelogram();
    Parallelogram(vector<Point>vertices);
    bool verify();
};

#endif // PARALLELOGRAM_H
