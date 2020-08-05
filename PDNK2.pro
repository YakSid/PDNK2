#-------------------------------------------------
#
# Project created by QtCreator 2019-10-06T21:26:33
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PDNK2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        cdatabasemanager.cpp \
        cmapmanager.cpp \
        cnode.cpp \
        corder.cpp \
        corderspage.cpp \
        coutcome.cpp \
        coutcomewidget.cpp \
        cstage.cpp \
        cstartpage.cpp \
        cstatisticspage.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        cdatabasemanager.h \
        cmapmanager.h \
        cnode.h \
        corder.h \
        corderspage.h \
        coutcome.h \
        coutcomewidget.h \
        cstage.h \
        cstartpage.h \
        cstatisticspage.h \
        mainwindow.h

FORMS += \
        corderspage.ui \
        coutcomewidget.ui \
        cstartpage.ui \
        cstatisticspage.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
