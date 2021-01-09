#include "bresenham_circle.h"
#include<QPair>
#include<QVector>
#include<iostream>
using namespace std;
BresenhamCircle::BresenhamCircle(int xc, int yc, int r){
    this->xc = xc;
    this->yc = yc;
    this->r = r;
}
void BresenhamCircle::drawPixels(int x, int y)
{
    this->points.push_back(qMakePair(xc+x, yc+y));
    this->points.push_back(qMakePair(xc-x, yc+y));
    this->points.push_back(qMakePair(xc+x, yc-y));
    this->points.push_back(qMakePair(xc-x, yc-y));
    this->points.push_back(qMakePair(xc+y, yc+x));
    this->points.push_back(qMakePair(xc-y, yc+x));
    this->points.push_back(qMakePair(xc+y, yc-x));
    this->points.push_back(qMakePair(xc-y, yc-x));
}
QVector< QPair<int,int> > BresenhamCircle::drawCircle(){
    int x,y,d;
    x=0;
    y=r;
    d = 3 - 2 * r;
    drawPixels(x, y);
    while (y >= x)
    {

         x++;
         if (d > 0)
         {
             y--;
             d = d + 4 * (x - y) + 10;
         }
         else
             d = d + 4 * x + 6;
         drawPixels(x, y);
     }
    return this->points;
}
