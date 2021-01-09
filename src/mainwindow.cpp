#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DDA_Line.h"
#include "Bresenham_Line.h"
#include "polar_circle.h"
#include "bresenham_circle.h"
#include "parametric_circle.h"
#include "bresenham_ellipse.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#include<QVector>
#include<QPair>
#include<ctime>
#include <chrono>
#include<QThread>
#include <complex>
//typedef complex<double> point;
//#define xp real()
//#define yp imag()

// Constant PI for providing angles in radians
#define PI 3.1415926535897932384626
//// to draw point at (x, y) assuming upper left box as (0, 0) point(x*drawSize-1, y*drawSize-1)
#define SIN(x) sin(x * 3.141592653589 / 180)
#define COS(x) cos(x * 3.141592653589 / 180)
using namespace std;
using namespace std::chrono;

QImage img = QImage(1230, 730, QImage::Format_RGB888);
QRgb lineColor = qRgb(200,200,200);
QRgb frameColor = qRgb(30,30,30);
QRgb unsetPoint = qRgb(245, 224, 80);
QRgb setPoint = qRgb(255,0,0);
QTimer t;
QRgb boundary;
QVector< QPair<int,int> > arr;
int MainWindow::scale(int X) {
    if (gridSize == 0) return X;
    if (X % gridSize) return X/gridSize;
    return (X/gridSize) - 1;
}

int MainWindow::transformX(int X) {
    X = scale(X);
    return (X-scale(img.width()/2));
}

int MainWindow::transformY(int Y) {
    Y = scale(Y);
    return -(Y-scale(img.height()/2));
}
bool MainWindow::isValidPixel(int x,int y){
    int startX;
    int startY;
    int sx = img.width()/2;
    int sy = img.height()/2;
    startX = sx + (x * gridSize);
    startY = sy - (y * gridSize);
    int endx = startX + gridSize;
    int endy = startY + gridSize;
    if(startX<0 || endx>= img.width() || startY<0 || endy >=img.height()){
        return false;
    }
    else
    return true;
}
void MainWindow::point(int x, int y, int r, int g, int b) {
    point(x, y, qRgb(r, g, b));
}

void MainWindow::point(int x, int y, QRgb color) {
    if (gridSize == 0) {
        img.setPixel(x, y, color);
    } else {
        int startX = (x/gridSize) * gridSize;
        int startY = (y/gridSize) * gridSize;

        for (int i = startX+1; i < startX + gridSize; i++) {
            if (i >= img.width() || i < 0) break;
            for (int j = startY+1; j < startY + gridSize; j++) {
                if (j >= img.height() || j < 0) break;
                img.setPixel(i,j, color);
            }
        }
//        ui->frame->setPixmap(QPixmap::fromImage(img));
    }
}
void MainWindow::mySetPixel(int x,int y,QRgb color){
    int startX;
    int startY;
    int sx = ((img.width()/2)/gridSize)* gridSize;
    int sy = ((img.height()/2)/gridSize)* gridSize;
    startX = sx + (x * gridSize);
    startY = sy - (y*gridSize);
    if((startX+gridSize)>=img.width() || (startY+gridSize) >=img.height() || startX<0 || startY<0 )
       return;
    for(int i=startX+1;i<startX+gridSize;i++){
        for(int j=startY+1; j<startY+gridSize;j++)
                img.setPixel(i,j,color);
    }
}
QRgb MainWindow::myGetPixel(int x,int y){
    int startX;
    int startY;
    int sx = img.width()/2;
    int sy = img.height()/2;
    startX = sx + (x * gridSize);
    startY = sy - (y * gridSize);
    return img.pixel(startX+1,startY+1);
}

void MainWindow::draw_axes() {

    if(gridSize == 0) {
        // X axis
        for(int i=0;i<img.width();i++)
            if (img.pixel(i, img.height()/2) == qRgb(30, 30, 30)) point(i,img.height()/2);

        // Y axis
        for(int i=0;i<img.height();i++)
            if (img.pixel(img.width()/2, i) == qRgb(30, 30, 30)) point(img.width()/2,i);
    } else {
        // X axis
        for(int i=0;i<img.width();i+=gridSize)
            if (img.pixel(i+1, img.height()/2) == qRgb(30, 30, 30)) point(i,img.height()/2);
        // Y axis
        for(int i=0;i<img.height();i+=gridSize)
            if (img.pixel(img.width()/2, i+1) == qRgb(30, 30, 30)) point(img.width()/2,i);
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::remove_axes() {
    if(gridSize == 0) {
        // X axis
        for(int i=0;i<img.width();i++) {
            if (img.pixel(i, img.height()/2) == qRgb(200, 200, 200)) point(i,img.height()/2, 30, 30, 30);
        }
        // Y axis
        for(int i=0;i<img.height();i++) {
            if (img.pixel(img.width()/2, i) == qRgb(200, 200, 200)) point(img.width()/2,i, 30, 30, 30);
        }
    } else {
        // X axis
        for(int i=0;i<img.width();i+=gridSize) {
            if (img.pixel(i + 1, img.height()/2) == qRgb(200, 200, 200)) point(i,img.height()/2, 30, 30, 30);
        }

        // Y axis
        for(int i=0;i<img.height();i+=gridSize) {
            if (img.pixel(img.width()/2, i+1) == qRgb(200, 200, 200)) point(img.width()/2,i, 30, 30, 30);
        }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::clear_frame() {
    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {
            img.setPixel(i,j, qRgb(30,30,30));
        }
    }
    ui->frame->setPixmap((QPixmap::fromImage(img)));
}

void MainWindow::reset() {
    clear_frame();
    ui->mouse_position->setText("");
    ui->locked_position->setText("");
    ui->show_axes->setCheckState(Qt::Unchecked);
    Npoints.clear();
}
void MainWindow::draw_grid() {
    // vertical lines
    for (int i = 0; i < img.width(); i += gridSize) {
        for (int j = 0; j < img.height(); j++) {
            img.setPixel(i,j, qRgb(200, 200,200));
        }
    }

    // horizontal lines
    for (int i = 0; i < img.height(); i+= gridSize) {
        for (int j = 0; j < img.width(); j++) {
            img.setPixel(j, i, qRgb(200,200,200));
        }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->frame,SIGNAL(mousePosLock()),this,SLOT(mousePressed()));
    connect(ui->frame,SIGNAL(sendMousePos(QPoint&)),this,SLOT(showCurrentMousePosition(QPoint&)));
    connect(ui->drawCombo, SIGNAL(currentIndexChanged(QString)),this, SLOT(set_draw_type(QString)));
    connect(ui->color_box, SIGNAL(currentIndexChanged(QString)),this,SLOT(set_color_box(QString)));
    connect(ui->drawCombo_2, SIGNAL(currentIndexChanged(QString)),this,SLOT(set_fill_type(QString)));
    connect(ui->transform_m, SIGNAL(currentIndexChanged(QString)),this,SLOT(set_transform_type(QString)));
    connect(ui->clip_type, SIGNAL(currentIndexChanged(QString)),this,SLOT(set_clip_type(QString)));
    p1.setX(-1), p1.setY(-1);
    p2.setX(-1), p2.setY(-1);
    gridSize = ui->grid_size->value();
    clear_frame();
    type = "";
//    ui->locked_position->setText(QString::number(gridSize));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCurrentMousePosition(QPoint& pos) {
//    ui->mouse_position->setText("X: " + QString::number(scale(pos.x())) + ", Y: " + QString::number(scale(pos.y())));
    ui->mouse_position->setText("X: " + QString::number(transformX(pos.x())) + ", Y: " + QString::number(transformY(pos.y())));
}

void MainWindow::mousePressed() {
    static int x = -1, y = -1;

    if (img.pixel(ui->frame->x, ui->frame->y) == setPoint) {
        return;
    }


    //////////// handles old unset point deleting if it's valid///////////////
    if ((x >= 0 && y >= 0) && img.pixel(x, y) == unsetPoint) {
        if (x % gridSize == 0) x++; // fixes a bug
        if (y % gridSize == 0) y++;
        if ((scale(x) == scale(img.width()/2)  || scale(y) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(x, y);
        else point(x, y, 30, 30, 30);
    }


        /////////////handles diselecting unset point//////////////
    if (scale(x) == scale(ui->frame->x) && scale(y) == scale(ui->frame->y)) {
        if (x % gridSize == 0) x++; // fixes a bug
        if (y % gridSize == 0) y++;
        if ((scale(x) == scale(img.width()/2)  || scale(y) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(x, y);
        else point(x, y, 30, 30, 30);
        ui->locked_position->setText("");
        x = -1, y = -1;
    }

        /////////////handles updating new point after clicking////////////
    else {
        x = ui->frame->x, y = ui->frame->y;
        ui->locked_position->setText("X: " + QString::number(scale(ui->frame->x)) + ", Y: " + QString::number(scale(ui->frame->y)));
        ui->locked_position->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
        point(ui->frame->x, ui->frame->y, 245, 224, 80);
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_show_axes_stateChanged(int arg1)
{
    if (arg1)
        draw_axes();
    else
        remove_axes();
}

void MainWindow::on_grid_size_valueChanged(int arg1)
{
    reset();
    gridSize = arg1;
    draw_grid();
}

void MainWindow::on_radius_box_valueChanged(int arg1){
    radius = arg1;
}

void MainWindow::on_radius_box_2_valueChanged(int arg1){
    radius2 = arg1;
}

void MainWindow::on_reset_clicked()
{
    reset();
    gridSize = 0;
    p1.setX(-1);
    p1.setY(-1);
    p2.setX(-1);
    p2.setY(-1);
    ui->mouse_position->setText(" ");
    ui->locked_position->setText("");
    ui->p1_label->setText("");
    ui->p2_label->setText("");
    ui->set_p1->setEnabled(true);
    ui->set_p2->setEnabled(true);
    /*gridSize = 0;
    draw_grid();
    ui->grid_size->setValue(0);
   ;*/
}

void MainWindow::on_set_p1_clicked()
{
    if (ui->frame->x == p2.x() && ui ->frame->y == p2.y()) return;
    p1.setX(transformX(ui->frame->x));
    p1.setY(transformY(ui->frame->y));
    ui->p1_label->setText("X: " + QString::number(p1.x()) + ", Y: " + QString::number(p1.y()));
    mySetPixel(p1.x(),p1.y(),setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1->setDisabled(true);
}

void MainWindow::on_set_p2_clicked()
{
    if (ui->frame->x == p1.x() && ui ->frame->y == p1.y()) return;
    p2.setX(transformX(ui->frame->x));
    p2.setY(transformY(ui->frame->y));
    ui->p2_label->setText("X: " + QString::number(p2.x()) + ", Y: " + QString::number(p2.y()));
    mySetPixel(p2.x(),p2.y(),setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p2->setDisabled(true);
}

void MainWindow::on_unset_p1_clicked()
{
    if ((p1.x() == 0  || p1.y() == 0) && ui->show_axes->checkState() == Qt::Checked)
        mySetPixel(p1.x(), p1.y(),lineColor);
    else
        mySetPixel(p1.x(), p1.y(), frameColor);
    p1.setX(-1);
    p1.setY(-1);
    ui->p1_label->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1->setDisabled(false);
}
void MainWindow::set_color_box(QString c){
    if(c=="red"){
        setPoint = qRgb(255,0,0);
    }
    else if(c == "blue"){
        setPoint = qRgb(0,0,255);
    }
    else{
        setPoint = qRgb(0,255,0);
    }
}

void MainWindow::on_unset_p2_clicked()
{
    if ((p2.x() == 0  || p2.y() == 0) && ui->show_axes->checkState() == Qt::Checked)
        mySetPixel(p2.x(), p2.y(),lineColor);
    else
        mySetPixel(p2.x(), p2.y(), frameColor);
    p2.setX(-1);
    p2.setY(-1);
    ui->p2_label->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p2->setDisabled(false);
}
void MainWindow::set_draw_type(QString t){
    type = t;
    ui->type_label->setText(type);
    //ui->draw->setEnabled(true);
}
void MainWindow::my_draw(QVector< QPair<int,int> > points){
    int i=0;
    while(i<points.size()){
        QPair<int,int> p = points[i];
        if(isValidPixel(p.first,p.second))
            mySetPixel(p.first,p.second,setPoint);
        i++;
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}
void MainWindow::set_fill_type(QString t){
    type2 = t;
    ui->type_label->setText(type2);
}
void MainWindow::on_set_np_clicked(){
    QPair<double,double> p = qMakePair(transformX(ui->frame->x)*1.0,transformY(ui->frame->y)*1.0);
    mySetPixel((int)p.first,(int)p.second,setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    this->Npoints.push_back(p);
}
void MainWindow::on_unset_np_clicked(){
    Npoints.clear();
}
void MainWindow::on_draw_2_clicked(){
    if(Npoints.size()==0) return;
    QPair<double,double> pstart;
    QPair<double,double> pend;
    boundary = setPoint;
    Npoints.push_back(Npoints.first());
    for(auto it=Npoints.begin();it!=Npoints.end();){
        pstart = *it;
        it++;
        if(it==Npoints.end())
            break;
        else
        {
            pend = *it;
        }
        Bresenham_Line bl(pstart.first,pstart.second,pend.first,pend.second);
        QVector< QPair<int,int> > points = bl.drawLine();
        my_draw(points);
    }
    return;
}
void MainWindow::on_draw_clicked(){
    //long time_start,time_end,duration;
    if(type=="" || type == "Select Drawing type")
        return;
    if(type == "DDA Line Drawing"){
        // Get starting timepoint
        auto time_start = high_resolution_clock::now();
        DDALine dl(p1.x(),p1.y(),p2.x(),p2.y());
        QVector< QPair<int,int> > points = dl.drawLine();
        my_draw(points);
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_1->setText("DDA Time(micro):"+QString::number(duration.count()));
    }
    else if(type == "Bresenham Line Drawing"){
        auto time_start = high_resolution_clock::now();
        Bresenham_Line bl(p1.x(),p1.y(),p2.x(),p2.y());
        QVector< QPair<int,int> > points = bl.drawLine();
        my_draw(points);
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Bresenham Time(micro):"+QString::number(duration.count()));
    }
    else if(type == "Polar circle"){
        if(radius==0)
            return;
        auto time_start = high_resolution_clock::now();
        PolarCircle plc(p1.x(),p1.y(),radius);
        QVector< QPair<int,int> > points = plc.drawCircle();
        my_draw(points);
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Polar Circle Time(micro):"+QString::number(duration.count()));
    }
    else if(type == "Bresenham circle"){
        if(radius==0)
            return;
        auto time_start = high_resolution_clock::now();
        BresenhamCircle bcl(p1.x(),p1.y(),radius);
        QVector< QPair<int,int> > points = bcl.drawCircle();
        my_draw(points);
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Bresenham Circle Time(micro):"+QString::number(duration.count()));
    }
    else if(type == "Bresenham Ellipse"){
        if(radius ==0 || radius2==0)
            return;
        auto time_start = high_resolution_clock::now();
        BresenhamEllipse be(p1.x(),p1.y(),radius,radius2);
        QVector< QPair<int,int> > points = be.drawCircle();
        my_draw(points);
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Bresenham Ellipse Time(micro):"+QString::number(duration.count()));
    }
    else if(type == "Parametric circle"){
        if(radius==0)
            return;
        auto time_start = high_resolution_clock::now();
        ParametricCircle pcl(p1.x(),p1.y(),radius);
        QVector< QPair<int,int> > points = pcl.drawCircle();
        my_draw(points);
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Parametric Circle Time(micro):"+QString::number(duration.count()));
    }
    else
        return;
}
void MainWindow::fill_right(int x,int y,QRgb newColor,QRgb OldColor) {
    if(isValidPixel(x,y) && (myGetPixel(x,y)==OldColor||myGetPixel(x,y)==unsetPoint)){
        mySetPixel(x,y,newColor);
        fill_right(++x,y,newColor,OldColor);
        x = x - 1 ;
        fill_right(x,y-1,newColor,OldColor);
        fill_right(x,y+1,newColor,OldColor);
    }
}
void MainWindow::fill_left(int x,int y,QRgb newColor,QRgb oldColor)
{
    if(isValidPixel(x,y) &&(myGetPixel(x,y)==oldColor||myGetPixel(x,y)==unsetPoint)){
        mySetPixel(x,y,newColor);
        fill_left(--x,y,newColor,oldColor);
        x = x + 1 ;
        fill_left(x,y-1,newColor,oldColor);
        fill_left(x,y+1,newColor,oldColor);
    }
}
void MainWindow::myFloodFill(int x,int y,QRgb newColor){
   fill_right(x,y,newColor,frameColor);
   fill_left(x-1,y,newColor,frameColor);
}
void MainWindow::myBoundaryFill(int x, int y,QRgb fill,QRgb boundary){
    QRgb current;
    if(!isValidPixel(x,y)) return;
    current = myGetPixel(x,y);
    if( (current != boundary) && (current != fill) ) {
        mySetPixel(x,y,fill);
        myBoundaryFill(x+1,y,fill,boundary);
        myBoundaryFill(x-1,y,fill,boundary);
        myBoundaryFill(x,y+1,fill,boundary);
        myBoundaryFill(x,y-1,fill,boundary);
    }
}
void MainWindow::myScanFill(QVector<QPair<double,double> >a,QRgb edge){
    for(int i=0;i<a.size();i++){
        int x = a[i].first;
        int y = a[i].second;
        int X = (img.width()/2)+(x*gridSize);
        int Y = (img.height()/2) -(y*gridSize);
        QPair<double,double> p = qMakePair(X*1.0,Y*1.0);
        a.replace(i,p);
    }
    int k = gridSize;
    int dx = 0;
    int dy = 0;

    vector<float> inv_slope;
    int n = a.size() - 1;
    int x_intersec[n];
    int ctr_intersec = 0;


    int y_min = INT_MAX;
    int y_max = 0;


    //////////// initialisation of needed informations ////////////
    for(int i = 0; i < n; i++)
    {
        dy = a[i+1].second - a[i].second;
        dx = a[i+1].first - a[i].first;

        if (a[i].second < y_min) y_min = a[i].second;
        if (a[i].second > y_max) y_max = a[i].second;

        if(dy == 0) inv_slope.push_back( 1.0);
        if(dx == 0) inv_slope.push_back(0.0);

        if((dy != 0) && (dx != 0))
        {
            inv_slope.push_back((float) dx/dy);
        }
    }




    /// for every y
    for(int y = y_min + k; y < y_max; y += k)
    {
        ctr_intersec = 0;

        // count intersections
        for(int i = 0; i < n; i++)
            if(((a[i].second <= y) && (a[i+1].second > y)) || ((a[i].second > y) && (a[i+1].second <= y)))
                x_intersec[ctr_intersec++] = (int)(a[i].first + inv_slope[i]*(y - a[i].first));



        // sorting intersections
        sort(x_intersec, x_intersec+ctr_intersec);


        // for every valid segment in that y
        for(int i = 0; i < ctr_intersec; i+=2)
        {
            int x_min = x_intersec[i];
            int x_max = x_intersec[i+1];


            while (img.pixel(x_min, y) == edge)
                x_min += 1;
            while (img.pixel(x_max, y) == edge)
                x_max -= 1;
            if (x_max < x_min) continue;

            // for drawing bresh line
            p1.setX(x_min), p1.setY(y), p2.setX(x_max + k), p2.setY(y);
            Bresenham_Line bl(transformX(p1.x()),transformY(p1.y()),transformX(p2.x()),transformY(p2.y()));
            QVector< QPair<int,int> > points = bl.drawLine();
            my_draw(points);
        }
    }
}

void MainWindow::on_draw_3_clicked(){
    QRgb fill = setPoint;
    if(type2=="Flood fill"){
        auto time_start = high_resolution_clock::now();
        myFloodFill(transformX(ui->frame->x),transformY(ui->frame->y),fill);
        ui->frame->setPixmap(QPixmap::fromImage(img));
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_1->setText("Flood fill Time(micro):"+QString::number(duration.count()));
    }
    else if(type2 == "Boundary fill"){
        auto time_start = high_resolution_clock::now();
        myBoundaryFill(transformX(ui->frame->x),transformY(ui->frame->y),fill,boundary);
        ui->frame->setPixmap(QPixmap::fromImage(img));
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Boundary fill Time(micro):"+QString::number(duration.count()));
    }
    else if(type2 == "Scan fill"){
        auto time_start = high_resolution_clock::now();
        myScanFill(Npoints,boundary);
        ui->frame->setPixmap(QPixmap::fromImage(img));
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Scan fill Time(micro):"+QString::number(duration.count()));
    }
    else
        return;
}
void MainWindow::resetPolygon(){
    for(int i=0;i<img.width();i++){
        for(int j=0;j<img.height();j++){
            if(img.pixel(i,j)!=frameColor && img.pixel(i,j)!=lineColor )
                img.setPixel(i,j,frameColor);
        }
    }
    draw_axes();
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::draw_polygon(QVector<QPair<double,double> > points){
    QPair<double,double> pstart;
    QPair<double,double> pend;
    for(int i=0;i<points.size();){
        double x1,x2,y1,y2;
        pstart = points[i];
        x1 = pstart.first;
        y1 = pstart.second;
        i++;
        if(i==points.size())
            break;
        else
        {
            pend = points[i];
        }
        x2 = pend.first;
        y2 = pend.second;
        Bresenham_Line bl(x1,y1,x2,y2);
        QVector< QPair<int,int> > rpoints = bl.drawLine();
        my_draw(rpoints);
    }
}

void MainWindow::set_transform_type(QString t){
    this->transfrom = t;
    ui->type_label->setText(t);
}
void MainWindow::on_scale_x_valueChanged(double s){
    sx = s;
}
void MainWindow::on_scale_y_valueChanged(double s){
    sy = s;
}
void MainWindow::on_transform_b_clicked(){
    if(this->transfrom == "Scalling"){
        auto time_start = high_resolution_clock::now();
        scaleObj();
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_1->setText("Scalling Time(micro):"+QString::number(duration.count()));
        return;
    }
    else if(this->transfrom == "Translation"){
        auto time_start = high_resolution_clock::now();
        translateObj();
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Translation Time(micro):"+QString::number(duration.count()));
        return;
    }
    else if(this->transfrom == "Rotation"){
        auto time_start = high_resolution_clock::now();
        rotateOBj(sx);
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_1->setText("Rotation Time(micro):"+QString::number(duration.count()));
    }
    else if(this->transfrom == "Reflection"){
        auto time_start = high_resolution_clock::now();
        reflectObj();
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Reflection Time(micro):"+QString::number(duration.count()));
    }
    else if(this->transfrom == "Sharing"){
        auto time_start = high_resolution_clock::now();
        shearObj();
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_1->setText("Shearing Time(micro):"+QString::number(duration.count()));
    }

}

QVector< QVector<double> > multiply(QVector< QVector<double> > A,QVector< QVector<double> > B){
    QVector< QVector<double> > C;
    for (int i = 0; i < 3; i++)
    {
        QVector<double> t;
        for (int j = 0; j < 3; j++)
        {
            double s = 0.0;
            for (int k = 0; k < 3; k++)
            {
                s += A[i][k]*B[k][j];
            }
            t.push_back(s);
        }
        C.push_back(t);
    }
    return C;
}

QVector< QVector<double> > MainWindow::make_transformation_matrix(double a[3][3])
{
    double tx,ty;
    if(p1.x()==-1 && p1.y()==-1){
        tx=0.0;
        ty=0.0;
    }
    else{
        tx = p1.x()*1.0;
        ty = p1.y()*1.0;
    }
    double t1[3][3] = {{1.0,0.0,-tx},
                       {0.0,1.0,-ty},
                       {0.0,0.0,1.0}};
    double t2[3][3] = {{1.0,0.0,tx},
                       {0.0,1.0,ty},
                       {0.0,0.0,1.0}};
    QVector< QVector<double> > T1,T2;
    QVector< QVector<double> > R,A;
    for(int i=0;i<3;i++){
        QVector<double> tt1,aa,tt2;
        for(int j=0;j<3;j++){
            tt1.push_back(t1[i][j]);
            aa.push_back(a[i][j]);
            tt2.push_back(t2[i][j]);
        }
        T1.push_back(tt1);
        T2.push_back(tt2);
        A.push_back(aa);
    }
    R = multiply(T2,A);
    R = multiply(R,T1);
    return R;
}

void MainWindow::transformObj(QVector<QVector<double> > TM){
    for(int i=0;i<Npoints.size();i++){
        QPair<double,double> p = Npoints[i];
        double x = p.first;
        double y = p.second;
        double X,Y;
        X = x*TM[0][0] + y*TM[0][1] + 1*TM[0][2];
        Y = x*TM[1][0] + y*TM[1][1] + 1*TM[1][2];
        p = qMakePair(X,Y);
        Npoints.replace(i,p);
    }
}

void MainWindow::scaleObj(){
    double S[3][3] = {{sx,0.0,0.0},
                       {0.0,sy,0.0},
                       {0.0,0.0,1.0}};
    QVector< QVector<double> > TM = make_transformation_matrix(S);

    QRgb temp = setPoint;
    setPoint = qRgb(120,120,255);
    draw_polygon(Npoints);
    setPoint = temp;
    transformObj(TM);

    draw_polygon(Npoints);
}

void MainWindow::translateObj(){
    QRgb temp = setPoint;
    setPoint = qRgb(120,120,255);
    draw_polygon(Npoints);
    setPoint = temp;
    QPair<double,double> pstart;
    int tx = sx;
    int ty = sy;
    for(int i=0;i<Npoints.size();i++){
        double x1,y1;
        pstart = Npoints[i];
        x1 = pstart.first+tx;
        y1 = pstart.second+ty;
        pstart = qMakePair(x1,y1);
        Npoints.replace(i,pstart);
    }
    draw_polygon(Npoints);
}

void MainWindow::rotateOBj(int angle)
{
    double cosx,sinx;
    cosx = COS(angle);
    sinx = SIN(angle);
    double R[3][3] = {{cosx,-sinx,0.0},
                      {sinx,cosx,0.0},
                      {0.0,0.0,1.0}};
    QVector< QVector<double> > TM = make_transformation_matrix(R);
    QRgb temp = setPoint;
    setPoint = qRgb(120,120,255);
    draw_polygon(Npoints);
    setPoint = temp;

    transformObj(TM);
    draw_polygon(Npoints);
}
complex<double> reflect(complex<double> P,complex<double> A, complex<double> B)
{
    // Performing translation and shifting origin at A
    complex<double> Pt = P-A;
    complex<double> Bt = B-A;

    // Performing rotation in clockwise direction
    // BtAt becomes the X-Axis in the new coordinate system
    complex<double> Pr = Pt/Bt;

    // Reflection of Pr about the new X-Axis
    // Followed by restoring from rotation
    // Followed by restoring from translation

    return conj(Pr)*Bt + A;
}
void MainWindow::reflectObj(){
    QRgb temp = setPoint;
    setPoint = qRgb(120,120,255);
    draw_polygon(Npoints);
    setPoint = temp;
    for(int i=0;i<Npoints.size();i++){
        double x,y;
        x=Npoints[i].first;
        y = Npoints[i].second;
        complex<double> A(p1.x()*1.0, p1.y()*1.0);
        complex<double> B(p2.x()*1.0, p2.y()*1.0);
        complex<double> p(x,y);
        complex<double> pr = reflect(p,A,B);
        x=pr.real();
        y=pr.imag();
        QPair<double,double> pa = qMakePair(x,y);
        Npoints.replace(i,pa);
    }
    draw_polygon(Npoints);
}

void MainWindow::shearObj(){
    QRgb temp = setPoint;
    setPoint = qRgb(120,120,255);
    draw_polygon(Npoints);
    setPoint = temp;
    double S[3][3] = {{1.0,sx,0.0},
                       {sy,1.0,0.0},
                       {0.0,0.0,1.0}};
    QVector< QVector<double> > TM;
    for(int i=0;i<3;i++){
        QVector<double> tt1;
        for(int j=0;j<3;j++){
            tt1.push_back(S[i][j]);
        }
        TM.push_back(tt1);
    }
    transformObj(TM);
    draw_polygon(Npoints);
}
void MainWindow::set_clip_type(QString t){
    cliptype = t;
}
void MainWindow::on_window_point_clicked(){
    QPair<double,double> p = qMakePair(transformX(ui->frame->x)*1.0,transformY(ui->frame->y)*1.0);
    mySetPixel((int)p.first,(int)p.second,setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    this->WindowPoints.push_back(p);
}
void MainWindow::on_draw_window_clicked(){
    WindowPoints.push_back(WindowPoints.front());
    drawwindow();
}

void MainWindow::drawwindow()
{
    QPair<double,double> pstart;
    QPair<double,double> pend;
    for(int i=0;i<WindowPoints.size();){
        double x1,x2,y1,y2;
        pstart = WindowPoints[i];
        x1 = pstart.first;
        y1 = pstart.second;
        i++;
        if(i==WindowPoints.size())
            break;
        else
        {
            pend = WindowPoints[i];
        }
        x2 = pend.first;
        y2 = pend.second;
        Bresenham_Line bl(x1,y1,x2,y2);
        QVector< QPair<int,int> > rpoints = bl.drawLine();
        my_draw(rpoints);
    }
}
void MainWindow::on_unset_window_clicked(){
    WindowPoints.clear();
    Npoints.clear();
    resetPolygon();
}

void MainWindow::on_clipping_clicked(){
    if(this->cliptype == "Line clipping"){
        auto time_start = high_resolution_clock::now();
        line_clip();
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_1->setText("Line Clipping Time(micro):"+QString::number(duration.count()));
        return;
    }
    else if(this->cliptype == "Polygon clipping"){
        auto time_start = high_resolution_clock::now();
        polygon_clip();
        auto time_end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(time_end - time_start);
        ui->timer_label_2->setText("Polygon Clipping Time(micro):"+QString::number(duration.count()));
        return;
    }
}

void MainWindow::draw_clip_line(PT pt1,PT pt2)
{
    Bresenham_Line bl(pt1.x,pt1.y,pt2.x,pt2.y);
    QVector< QPair<int,int> > points = bl.drawLine();
    my_draw(points);
}
double max(double a,double b){
    if(a>b)
        return a;
    else
        return b;
}
double min(double a,double b){
    if(a<b)
        return a;
    else
        return b;
}

PT MainWindow::setcode(PT p)	//for setting the 4 bit code
{
    PT ptemp;
    double maxX=INT_MIN,maxY=INT_MIN,minX=INT_MAX,minY=INT_MAX;
    for(int i=0;i<WindowPoints.size();i++){
        maxX = max(maxX,WindowPoints[i].first);
        maxY = max(maxY,WindowPoints[i].second);
        minX = min(minX,WindowPoints[i].first);
        minY = min(minY,WindowPoints[i].second);
    }
    if(p.y<maxY)
        ptemp.code[0]='0';	//Top
    else
        ptemp.code[0]='1';

    if(p.y<=minY)
        ptemp.code[1]='1';	//Bottom
    else
        ptemp.code[1]='0';

    if(p.x>=maxX)
        ptemp.code[2]='1';	//Right
    else
        ptemp.code[2]='0';

    if(p.x<=minX)
        ptemp.code[3]='1';	//Left
    else
        ptemp.code[3]='0';

    ptemp.x=p.x;
    ptemp.y=p.y;

    return(ptemp);
}

int MainWindow::visibility(PT pt1,PT pt2)
{
    int i,flag=0;

    for(i=0;i<4;i++)
    {
        if((pt1.code[i]!='0') || (pt2.code[i]!='0'))
            flag=1;
    }

    if(flag==0)
        return(0);

    for(i=0;i<4;i++)
    {
        if((pt1.code[i]==pt2.code[i]) && (pt1.code[i]=='1'))
            flag=0;
    }

    if(flag==0)
        return(1);

    return(2);
}

PT MainWindow::resetendpt(PT pt1,PT pt2)
{
    int maxX=INT_MIN,maxY=INT_MIN,minX=INT_MAX,minY=INT_MAX;
    for(int i=0;i<WindowPoints.size();i++){
        maxX = max(maxX,WindowPoints[i].first);
        maxY = max(maxY,WindowPoints[i].second);
        minX = min(minX,WindowPoints[i].first);
        minY = min(minY,WindowPoints[i].second);
    }
    PT temp;
    int x,y,i;
    float m,k;

    if(pt1.code[3]=='1')
        x=minX;

    if(pt1.code[2]=='1')
        x=maxX;

    if((pt1.code[3]=='1') || (pt1.code[2]=='1'))
    {
        m=(float)(pt2.y-pt1.y)/(pt2.x-pt1.x);
        k=(pt1.y+(m*(x-pt1.x)));
        temp.y=k;
        temp.x=x;

        for(i=0;i<4;i++)
            temp.code[i]=pt1.code[i];

        if(temp.y<=maxY && temp.y>=minY)
            return (temp);
    }

    if(pt1.code[0]=='1')
        y=maxY;

    if(pt1.code[1]=='1')
        y=minY;

    if((pt1.code[0]=='1') || (pt1.code[1]=='1'))
    {
        m=(float)(pt2.y-pt1.y)/(pt2.x-pt1.x);
        k=(float)pt1.x+(float)(y-pt1.y)/m;
        temp.x=k;
        temp.y=y;

        for(i=0;i<4;i++)
            temp.code[i]=pt1.code[i];

        return(temp);
    }
    else
        return(pt1);
}
void MainWindow::line_clip(){
    PT pt1(p1.x(),p1.y());
    PT pt2(p2.x(),p2.y());
    PT pt4,pt3;
    pt1=setcode(pt1);
    pt2=setcode(pt2);
    int v=visibility(pt1,pt2);
    resetPolygon();
    switch(v)
    {
        case 0:
                drawwindow();
                draw_clip_line(pt1,pt2);
                break;
        case 1:
                drawwindow();
                break;
        case 2:
                pt3=resetendpt(pt1,pt2);
                pt4=resetendpt(pt2,pt1);
                drawwindow();
                draw_clip_line(pt3,pt4);
                break;
    }
}
void MainWindow::polygon_clip(){
    resetPolygon();
    drawwindow();
    QVector<QPair<double,double> > newP;
    for(int i=0;i<Npoints.size();){
        QPair<double,double> pstart = Npoints[i];
        i++;
        if(i==Npoints.size())
            break;
        QPair<double,double> pend = Npoints[i];
        PT pt1(pstart.first,pstart.second);
        PT pt2(pend.first,pend.second);
        PT pt4,pt3;
        pt1=setcode(pt1);
        pt2=setcode(pt2);
        int v=visibility(pt1,pt2);
        QPair<double,double> pn;
        switch(v)
        {
            case 0:
                    pn = qMakePair(pt1.x*1.0,pt1.y*1.0);
                    newP.push_back(pn);
                    draw_clip_line(pt1,pt2);
                    break;
            case 1:
                    break;
            case 2:
                    pt3=resetendpt(pt1,pt2);
                    pt4=resetendpt(pt2,pt1);
                    draw_clip_line(pt3,pt4);
                    pn = qMakePair(pt3.x,pt3.y);
                    newP.push_back(pn);
                    pn =  qMakePair(pt4.x,pt4.y);
                    newP.push_back(pn);
                    break;
        }
    }
    if(newP.size()>0){
    newP.push_back(newP.front());
    Npoints = newP;
    QRgb temp = setPoint;
    setPoint = qRgb(120,120,255);
    draw_polygon(Npoints);
    draw_polygon(Npoints);

    setPoint = temp;
    }
}
