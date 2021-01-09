
#ifndef POLAR_CIRCLE_H
#define POLAR_CIRCLE_H
#include<QVector>
#include<QPair>
class PolarCircle{
private:
    int xc, yc;
    int r;
    QVector< QPair<int,int> > points;
public:
    PolarCircle(int xc,int yc,int r);
    void eightWaySymmetricPlot(int x,int y);
    QVector< QPair<int,int> > drawCircle();
};




#endif // POLAR_CIRCLE_H
