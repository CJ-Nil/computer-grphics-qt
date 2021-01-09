#ifndef PARAMETRIC_CIRCLE_H
#define PARAMETRIC_CIRCLE_H
#include<QVector>
#include<QPair>
class ParametricCircle{
private:
    int xc, yc;
    int r;
    QVector< QPair<int,int> > points;
public:
    ParametricCircle(int xc,int yc,int r);
    QVector< QPair<int,int> > drawCircle();
};

#endif // PARAMETRIC_CIRCLE_H
