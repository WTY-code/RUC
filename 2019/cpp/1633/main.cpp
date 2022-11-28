#include <iostream>
#include "rectangle.h"
#include "point.h"
#include "line.h"
#include "quadrilateral.h"
#include "parallelogram.h"
#include <vector>
#include <iomanip>

using namespace std;

int main()
{
    vector<Point> vertices;
    for (int i=0;i<4;i++){
        double x,y;
        cin>>x>>y;
        Point p(x,y);
        vertices.push_back(p);
    }

    Quadrilateral q(vertices);
    Parallelogram pa(vertices);
    Rectangle r(vertices);

/*    cout<<"the four vertices of quadrilateral"<<endl;
    cout<<q.getpoint1().getX()<<" "<<q.getpoint1().getY()<<endl;
    cout<<q.getpoint2().getX()<<" "<<q.getpoint2().getY()<<endl;
    cout<<q.getpoint3().getX()<<" "<<q.getpoint3().getY()<<endl;
    cout<<q.getpoint4().getX()<<" "<<q.getpoint4().getY()<<endl;

    cout<<"the four lines' vertices"<<endl;
    cout<<q.getline12().getpointa().getX()<<" "<<q.getline12().getpointa().getY()<<endl;
    cout<<q.getline23().getpointa().getX()<<" "<<q.getline23().getpointa().getY()<<endl;
    cout<<q.getline34().getpointa().getX()<<" "<<q.getline34().getpointa().getY()<<endl;
    cout<<q.getline41().getpointa().getX()<<" "<<q.getline41().getpointa().getY()<<endl;
*/

    cout<<fixed<<setprecision(2);

    cout<<(q.verify()?1:0)<<endl;
    cout<<q.getArea()<<endl;
    cout<<q.getPerimeter()<<endl;

    cout<<(pa.verify()?1:0)<<endl;
    cout<<pa.getArea()<<endl;
    cout<<pa.getPerimeter()<<endl;

    cout<<(r.verify()?1:0)<<endl;
    cout<<r.getArea()<<endl;
    cout<<r.getPerimeter()<<endl;

    return 0;
}
