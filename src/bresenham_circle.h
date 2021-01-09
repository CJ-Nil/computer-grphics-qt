#ifndef BRESENHAM_CIRCLE_H
#define BRESENHAM_CIRCLE_H

#include<QVector>
#include<QPair>
class BresenhamCircle{
private:
    int xc, yc;
    int r;
    QVector< QPair<int,int> > points;
public:
    BresenhamCircle(int xc,int yc,int r);
    void drawPixels(int x,int y);
    QVector< QPair<int,int> > drawCircle();
};

#endif // BRESENHAM_CIRCLE_H
