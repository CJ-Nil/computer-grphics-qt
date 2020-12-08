#include "DDA_Line.h"
#include<QPair>
#include<QVector>
#include<stdio.h>
#include<cmath>
using namespace std;

DDALine::DDALine(int x0,int y0,int x1,int y1){
    this->x0 = x0;
    this->y0 = y0;
    this->x1 = x1;
    this->y1 = y1;
}
int DDALine::abs(int n){
    return ( (n>0) ? n : ( n * (-1)));
}
QVector< QPair<int,int> >DDALine::drawLine(){
    // calculate dx & dy
    float dx = abs(x1 - x0);
    float dy = abs(y1 - y0);

    // calculate steps required for generating pixels
    float steps = dx >= dy ? dx : dy;
    dx = dx/steps;
    dy = dy/steps;
    // Put pixel for each step
    float x = x0;
    float y = y0;
    QVector< QPair<int,int> > points;
    int i=0;
    while(i<=steps){
        points.push_back(qMakePair((int)x,(int)y));
        x = x+dx;
        y = y+dy;
        i++;
    }
    return points;
}
