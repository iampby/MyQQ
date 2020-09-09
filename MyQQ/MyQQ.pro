RC_FILE+=MyQQ.rc
RC_ICONS=MyQQ.ico
OTHER_FILES+=\
MyQQ.rc
QT += quick winextras network xml  widgets network sql
CONFIG += c++11
#资源过大使用它
CONFIG += resources_big
CONIFG += qtquickcompiler
# The following define makes your compiler emit warnings if you use
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriendgroupwidget.cpp \
    bigfilesocket.cpp \
    friendgroupmodel.cpp \
    friendmodel.cpp \
    headimgview.cpp \
    headimgwidget.cpp \
    imagehelper.cpp \
    images.cpp \
    loginsocket.cpp \
        main.cpp \
    funcc.cpp \
    nativeserver.cpp \
    nativesocket.cpp \
    netmonitor.cpp \
    qmlimageprovider.cpp \
    screenwidget.cpp \
    sendsocket.cpp \
    updatetimer.cpp \
    weatherhandle.cpp \
    registersocket.cpp

RESOURCES += qml.qrc \
    images_widget.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.

HEADERS += \
    addfriendgroupwidget.h \
    bigfilesocket.h \
    friendgroupmodel.h \
    friendmodel.h \
    funcc.h \
    headimgview.h \
    headimgwidget.h \
    imagehelper.h \
    images.h \
    loginsocket.h \
    nativeserver.h \
    nativesocket.h \
    netmonitor.h \
    qmlimageprovider.h \
    screenwidget.h \
    sendsocket.h \
    updatetimer.h \
    weatherhandle.h \
    registersocket.h

DISTFILES += \
    images/QQTray.png \
    images/bellinfo.png \
    images/belltras.png \
    images/border.png \
    images/bottom.png \
    images/checked.png \
    images/code.png \
    images/codeB.png \
    images/head.png \
    images/keybord.png \
    images/keybordB.png \
    images/lockerBlue.png \
    images/lockerGray.png \
    images/maskLogin.png \
    images/qqClose.png \
    images/qqCloseC.png \
    images/qqGreenL.png \
    images/qqLogin.png \
    images/qqMinimum.png \
    images/qqMinimumC.png \
    images/qqSet.png \
    images/qqSetC.png \
    images/qqSetClose.png \
    images/qqSetCloseC.png \
    images/qqSetMinimum.png \
    images/qq小图标.png \
    images/qq灰色小图标.png \
    images/top.png \
    images/unchecked.png \
    images/云.png
