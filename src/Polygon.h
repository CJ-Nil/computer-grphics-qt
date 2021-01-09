#ifndef POLYGON_H
#define POLYGON_H
#include<QVector>
#include<QPoint>
class Polygon{
private:
    int xc, yc;
    int r;
    QVector< QPoint > points;
public:
    BresenhamCircle(int xc,int yc,int r);
    void drawPixels(int x,int y);
    QVector< QPair<int,int> > drawCircle();
};
#endif // POLYGON_H
