#ifndef BRESENHAM_LINE_H
#define BRESENHAM_LINE_H
#include<QVector>
#include<QPair>
class Bresenham_Line{
private:
    int x0, x1, y0, y1;
public:
    Bresenham_Line(int x0,int y0,int x1,int y1);
    QVector< QPair<int,int> > drawLine();
    void swap(int* a,int* b);
};

#endif // BRESENHAM_LINE_H
