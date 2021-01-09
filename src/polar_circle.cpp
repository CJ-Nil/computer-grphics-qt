#include "polar_circle.h"
#include<QPair>
#include<QVector>
#include<iostream>
using namespace std;
PolarCircle::PolarCircle(int xc, int yc, int r){
    this->xc = xc;
    this->yc = yc;
    this->r = r;
}
void PolarCircle::eightWaySymmetricPlot(int x, int y){
    this->points.push_back(qMakePair(x+xc,y+yc));
    this->points.push_back(qMakePair(x+xc,-y+yc));
    this->points.push_back(qMakePair(-x+xc,-y+yc));
    this->points.push_back(qMakePair(-x+xc,y+yc));
    this->points.push_back(qMakePair(y+xc,x+yc));
    this->points.push_back(qMakePair(y+xc,-x+yc));
    this->points.push_back(qMakePair(-y+xc,-x+yc));
    this->points.push_back(qMakePair(-y+xc,x+yc));
}
QVector< QPair<int,int> > PolarCircle::drawCircle(){
    int x,y,d;
    x=0;
    y=r;
    d=3-2*r;
    eightWaySymmetricPlot(x,y);
    while(x<=y)
    {
         if(d<=0)
         {
              d=d+4*x+6;
          }
          else
          {
              d=d+4*x-4*y+10;
              y=y-1;
          }
          x=x+1;
          eightWaySymmetricPlot(x,y);
      }
    return this->points;
}
