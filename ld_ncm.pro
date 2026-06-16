#-------------------------------------------------
#
# Project created by QtCreator 2026-06-15T15:10:07
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ld_ncm
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

CONFIG += c++11 openssl

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    network.cpp \
    httpclient.cpp \
    ncm_login.cpp \
    userdata.cpp \
    common.cpp \
    music_list_item.cpp

HEADERS += \
        mainwindow.h \
    network.h \
    httpclient.h \
    ncm_login.h \
    userdata.h \
    common.h \
    music_list_item.h

FORMS += \
        mainwindow.ui \
    ncm_login.ui \
    music_list_item.ui
win32:msvc {
    QMAKE_CXXFLAGS += /utf-8
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
