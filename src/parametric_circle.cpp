#include "parametric_circle.h"
#include<QPair>
#include<QVector>
#include<iostream>
using namespace std;
ParametricCircle::ParametricCircle(int xc, int yc, int r){
    this->xc = xc;
    this->yc = yc;
    this->r = r;
}
QVector< QPair<int,int> > ParametricCircle::drawCircle(){
    int x,y;
    for(x = r-xc;x<r+xc;x++)
    {
        y=yc+sqrt((r*r-(x-xc)*(x-xc)));
        points.push_back(qMakePair(x,y));
        y=yc-sqrt((r*r-(x-xc)*(x-xc)));
        points.push_back(qMakePair(x,y));
    }
    return points;
}
