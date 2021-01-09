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
void Bresenham_Line::swap(int* a,int* b) {
    int t=*a;
    *a=*b;
    *b=t;
}

QVector< QPair<int,int> >Bresenham_Line::drawLine(){
   QVector< QPair<int,int> > points;
   int dx,dy,d,incry,incre,incrne,slope=0;
   points.push_back(qMakePair(x0,y0));
   dx=abs(x0-x1);
   dy=abs(y0-y1);
   if(dy>dx) {
       swap(&x0,&y0);
       swap(&x1,&y1);
       swap(&dx,&dy);
       slope=1;
   }
   if(x0>x1) {
       swap(&x0,&x1);
       swap(&y0,&y1);
   }
   if(y0>y1)
       incry=-1;
   else
       incry=1;
   d=2*dy-dx;
   incre=2*dy;
   incrne=2*(dy-dx);

   while(x0<x1) {
       if(d<=0)
           d+=incre;
       else {
           d+=incrne;
           y0+=incry;
       }
       x0++;
       if(slope) {
           points.push_back(qMakePair(y0,x0));
       } else {
           points.push_back(qMakePair(x0,y0));
       }
   }
    return points;
}
