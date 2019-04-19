TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
        main.c

INCLUDEPATH+=/usr/include/SDL2/
INCLUDEPATH += /usr/include/c++/7.3.0/
LIBS += /usr/lib/x86_64-linux-gnu/libSDL2.so
