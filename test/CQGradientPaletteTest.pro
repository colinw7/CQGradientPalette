TEMPLATE = app

QT += widgets

TARGET = CQGradientPalette

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11

CONFIG += debug

# Input
SOURCES += \
CQGradientPaletteTest.cpp \
CMandelbrot.cpp \

HEADERS += \
CQGradientPaletteTest.h \
CMandelbrot.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
../../CQUtil/include \
../../CExpr/include \
../../CImageLib/include \
../../CFont/include \
../../CFile/include \
../../CConfig/include \
../../COS/include \
../../CStrUtil/include \
../../CUtil/include \
../../CMath/include \
../../CRegExp/include \
../../CRGBName/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CExpr/lib \
-L../../CImageLib/lib \
-L../../CConfig/lib \
-L../../CFont/lib \
-L../../CFile/lib \
-L../../CFileUtil/lib \
-L../../CStrUtil/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-L../../CRGBName/lib \
-L../../CRegExp/lib \
-lCQGradientPalette -lCQUtil -lCExpr -lCImageLib -lCConfig -lCFont -lCFile \
-lCFileUtil -lCStrUtil -lCRGBName -lCUtil -lCOS -lCRegExp \
-ljpeg -lpng -lcurses -ltre
