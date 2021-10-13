TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Demo_Mifare_Classic.cpp

unix|win32: LIBS += -L$$PWD/../LIB/ -lODALID_Education

INCLUDEPATH += $$PWD/../LIB
DEPENDPATH += $$PWD/../LIB
