#ifndef DDA_LINE_H
#define DDA_LINE_H
#include<QVector>
#include<QPair>
class DDALine{
private:
    int x0, x1, y0, y1;
public:
    DDALine(int x0,int y0,int x1,int y1);
    int abs(int n);
    QVector< QPair<int,int> > drawLine();
};

#endif // DDA_LINE_H

