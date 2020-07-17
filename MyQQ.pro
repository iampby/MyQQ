RC_FILE+=MyQQ.rc
RC_ICONS=MyQQ.ico
OTHER_FILES+=\
MyQQ.rc
QT += quick winextras network xml
CONFIG += c++11
CONFIG += resources_big
CONIFG += qtquickcompiler
#资源过大使用它
QT += widgets network sql

# The following define makes your compiler emit warnings if you use
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bigfilesocket.cpp \
    friendgroupmodel.cpp \
    friendmodel.cpp \
    headimgview.cpp \
    headimgwidget.cpp \
    images.cpp \
    loginsocket.cpp \
        main.cpp \
    funcc.cpp \
    netmonitor.cpp \
    qmlimageprovider.cpp \
    UpdateTimer.cpp \
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
    bigfilesocket.h \
    friendgroupmodel.h \
    friendmodel.h \
    funcc.h \
    headimgview.h \
    headimgwidget.h \
    images.h \
    loginsocket.h \
    netmonitor.h \
    qmlimageprovider.h \
    UpdateTimer.h \
    weatherhandle.h \
    registersocket.h
