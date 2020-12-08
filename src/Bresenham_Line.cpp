#include "Bresenham_Line.h"
#include<QPair>
#include<QVector>
#include<iostream>
using namespace std;

Bresenham_Line::Bresenham_Line(int x0,int y0,int x1,int y1){
    this->x0 = x0;
    this->y0 = y0;
    this->x1 = x1;
    this->y1 = y1;
}
QVector< QPair<int,int> >Bresenham_Line::drawLine(){
   QVector< QPair<int,int> > points;
   int dx,dy,pk,x,y;
   x=x0,y=y0;
   dx = abs(x1 - x0);
   dy = abs(y1 - y0);
   pk = 2*dy - dx;
   for(int i=0;i<=dx;i++){
       points.push_back(qMakePair(x,y));
       if(pk>=0){
           pk = pk + 2*dy - 2*dx;
           x++;
           y++;
       }
       else{
           pk = pk + 2*dy;
           x++;
       }
   }
    return points;
}
