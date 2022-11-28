TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    point.cpp \
    line.cpp \
    parallelogram.cpp \
    quadrilateral.cpp \
    rectangle.cpp

HEADERS += \
    point.h \
    line.h \
    parallelogram.h \
    quadrilateral.h \
    rectangle.h
