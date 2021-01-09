#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QVector>
#include <QPair>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
typedef struct coordinate
{
    double x,y;
    char code[4];
    coordinate(){
        x=0;
        y=0;
    }
    coordinate(int a,int b){
        x=a;
        y=b;
    }
}PT;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void showCurrentMousePosition(QPoint&);
    void mousePressed();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_show_axes_stateChanged(int arg1);

    void on_grid_size_valueChanged(int arg1);

    void on_reset_clicked();

    void on_set_p1_clicked();

    void on_set_p2_clicked();

    void on_unset_p2_clicked();

    void on_unset_p1_clicked();
    void on_unset_np_clicked();
    void on_draw_clicked();
    void on_draw_2_clicked();
    void on_set_np_clicked();
    void on_draw_3_clicked();

    void on_radius_box_valueChanged(int arg1);
    void on_radius_box_2_valueChanged(int arg1);
    void set_draw_type(QString t);
    void set_color_box(QString c);
    void set_fill_type(QString t);


    void set_transform_type(QString t);
    void on_transform_b_clicked();
    void on_scale_x_valueChanged(double arg1);
    void on_scale_y_valueChanged(double arg1);

     void set_clip_type(QString t);
     void on_window_point_clicked();
     void on_unset_window_clicked();
     void on_draw_window_clicked();
     void on_clipping_clicked();

private:
    Ui::MainWindow *ui;
    int gridSize;
    int radius;
    int radius2;
    QPoint p1, p2;
    QString type,type2,transfrom,cliptype;
    QVector< QPair<double,double> > Npoints,WindowPoints;
    double sx,sy;
    //////////////////// BASIC UTILITY FUNCTION////////////////////
    void point(int x, int y, int r=200, int g=200, int b=200);
    void point(int x, int y, QRgb color);
    void mySetPixel(int x,int y, QRgb color);
    QRgb myGetPixel(int x,int y);
    void clear_frame();
    int scale(int X);
    int transformX(int X);
    int transformY(int Y);
    void reset();
    bool isValidPixel(int x,int y);


    ////////////////// FUNCTIONS FOR GRID AND AXES////////////////////
    void draw_axes();
    void remove_axes();
    void draw_grid();
    ////////////drawing structures////////////
    void my_draw(QVector<QPair<int, int> > points);
    void fill_left(int x,int y,QRgb color,QRgb oldColor);
    void fill_right(int x,int y,QRgb color,QRgb oldColor);
    void myFloodFill(int x,int y,QRgb color);
    void myBoundaryFill(int x,int y,QRgb fill,QRgb boundary);
    void myScanFill(QVector<QPair<double,double> > a,QRgb edge);
    void resetPolygon();
    void rotateOBj(int ang);
    void scaleObj();
    void translateObj();
    void draw_polygon(QVector<QPair<double, double> > point);
    QVector<QVector<double> > make_transformation_matrix(double s[3][3]);
    void transformObj(QVector< QVector<double> > TM);
    void reflectObj();
    void shearObj();

    void drawwindow();
    void draw_clip_line(PT pt1, PT pt2);
    PT setcode(PT p);
    int visibility(PT pt1, PT pt2);
    PT resetendpt(PT pt1, PT pt2);
    void line_clip();
    void polygon_clip();
};
#endif // MAINWINDOW_H
