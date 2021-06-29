QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

#include ( /home/ivizot/Qt/qwt6/features/qwt.prf )

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cghvpsform.cpp \
    eth.cpp \
    graphs.cpp \
    ledindicator.cpp \
    main.cpp \
    mainwindow.cpp \
    matRoutine.cpp \
    qcustomplot.cpp \
    setupGUI.cpp

HEADERS += \
    headers.h \
    cghvpsform.h \
    eth.h \
    graphs.h \
    ledindicator.h \
    mainwindow.h \
    matRoutine.h \
    qcustomplot.h \
    setupGUI.h

FORMS += \
    cghvpsform.ui \
    mainwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES +=
