QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    my_label.cpp \
    DDALine.cpp \
    Bresenham_Line.cpp \
    polar_circle.cpp \
    bresenham_circle.cpp \
    bresenham_ellipse.cpp \
    parametric_circle.cpp

HEADERS += \
    mainwindow.h \
    my_label.h \
    DDA_Line.h \
    Bresenham_Line.h \
    polar_circle.h \
    bresenham_circle.h \
    parametric_circle.h \
    bresenham_ellipse.h \
    Polygon.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
