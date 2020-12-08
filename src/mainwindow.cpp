#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DDA_Line.h"
#include "Bresenham_Line.h"
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
//// to draw point at (x, y) assuming upper left box as (0, 0) point(x*drawSize-1, y*drawSize-1)

using namespace std;
using namespace std::chrono;

QImage img = QImage(1230, 730, QImage::Format_RGB888);
QRgb lineColor = qRgb(200,200,200);
QRgb frameColor = qRgb(30,30,30);
QRgb unsetPoint = qRgb(245, 224, 80);
QRgb setPoint = qRgb(255,0,0);
QTimer t;

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
    for(int i=startX+1;i<startX+gridSize;i++){
        for(int j=startY+1; j<startY+gridSize;j++)
        img.setPixel(i,j,color);
    }
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
    if ((x >= 0 && y >= 0) && img.pixel(x, y) != setPoint) {
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
    ui->draw->setEnabled(true);
}
void MainWindow::my_draw(QVector< QPair<int,int> > points){
    int i=0;
    while(i<points.size()){
        QPair<int,int> p = points[i];
        mySetPixel(p.first,p.second,setPoint);
        i++;
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->draw->setDisabled(true);
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
    else
        return;
}
