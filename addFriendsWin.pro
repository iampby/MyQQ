#-------------------------------------------------
#
# Project created by QtCreator 2019-09-25T10:40:42
#
#-------------------------------------------------
RC_FILE=appinfo_resource.rc
CONFIG += resources_big
QT       += core gui sql \
    quick network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = addFriendsWin
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
    addfriendswidget.cpp \
    combobox.cpp \
    findusermodel.cpp \
        main.cpp \
        mainwindow.cpp \
    movedwidget.cpp \
    placemodel.cpp \
    subcombobox.cpp \
    tcpsocket.cpp \
    titlebar.cpp \
    findbtn.cpp \
    label.cpp \
    userview.cpp \
    userwidget.cpp

HEADERS += \
    addfriendswidget.h \
    combobox.h \
    findusermodel.h \
        mainwindow.h \
    movedwidget.h \
    placemodel.h \
    subcombobox.h \
    tcpsocket.h \
    titlebar.h \
    findbtn.h \
    label.h \
    userview.h \
    userwidget.h


FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    build-addFriendsWin-Desktop_Qt_5_12_8_MSVC2017_64bit-Debug/debug/appInfo.res
