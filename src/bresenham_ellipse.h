#ifndef BRESENHAM_ELLIPSE_H
#define BRESENHAM_ELLIPSE_H
#include<QVector>
#include<QPair>
class BresenhamEllipse{
private:
    int xc, yc,rx,ry;
    QVector< QPair<int,int> > points;
public:
    BresenhamEllipse(int xc,int yc,int a,int b);
    void drawPixels(int x,int y);
    QVector< QPair<int,int> > drawCircle();
};
#endif // BRESENHAM_ELLIPSE_H
