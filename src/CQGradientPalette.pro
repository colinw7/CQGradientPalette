TEMPLATE = lib

TARGET = CQGradientPalette

DEPENDPATH += .

QT += widgets

CONFIG += staticlib
CONFIG += debug

QMAKE_CXXFLAGS += -std=c++11

MOC_DIR = .moc

# Input
SOURCES += \
CGradientPalette.cpp \
CQGradientPaletteControl.cpp \
CQGradientPalette.cpp \

HEADERS += \
../include/CGradientPalette.h \
../include/CQGradientPaletteControl.h \
../include/CQGradientPalette.h \
CUnixFile.h \

OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

DESTDIR = ../lib

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
