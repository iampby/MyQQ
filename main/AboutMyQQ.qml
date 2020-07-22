import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtWinExtras 1.0
import "qrc:/"
//关于界面
ApplicationWindow {
    id: aboutWin
    visible: true
    width: 378
    height: 284
    title: qsTr("关于MyQQ")
    modality: Qt.ApplicationModal
    flags: Qt.Window | Qt.FramelessWindowHint|Qt.WindowStaysOnTopHint
    x: qqMainWin.x + qqMainWin.width / 2 - width / 2
    y: qqMainWin.y + qqMainWin.height / 2 - height / 2
    onClosing: {
        hide() //不退出app
        loaderForAbout.source = "" //释放资源
    }
    background: Rectangle {
        border.width: 1
        border.color: "lightgray"
        color: "white"
    }

    MouseCustomForWindow {
        onSendPos: {
            aboutWin.x += movedCoordinate.x
            aboutWin.y += movedCoordinate.y
        }
    }

    header: ToolBar {
        height: 30
        MouseCustomForWindow {
            onSendPos: {
                aboutWin.x += movedCoordinate.x
                aboutWin.y += movedCoordinate.y
            }
        }
        RowLayout {
            x: 10
            height: parent.height
            Image {
                source: "qrc:/images/mainInterface/qqWhite.png"
            }
            Label {
                text: qsTr("关于MyQQ")
                color: "white"
            }
        }
        ToolButton {
            anchors.right: parent.right
            id: closeBtn
            hoverEnabled: true
            onClicked: {
                hide()
                loaderForAbout.source = ""
            }
            background: Rectangle {
                implicitWidth: 28
                implicitHeight: 30
                color: "white"
                Image {
                    id: imgClose
                    source: "qrc:/images/mainInterface/mesClose.png"
                    sourceSize: Qt.size(28, 30)
                }
                ColorOverlay {
                    anchors.fill: imgClose
                    source: imgClose
                    color: closeBtn.hovered ? "#d44027" : "#12b7fc"
                }
            }
        }
        background: Rectangle {
            implicitWidth: parent.width
            implicitHeight: parent.height
            color: "#12b7fc"
        }
    }
    Column {
        Image {
            source: "qrc:/images/mainInterface/aboutImg.png"
        }
        Rectangle {
            width: aboutWin.width
            height: 80
            Column {
                Label {
                    width: aboutWin.width
                    height: 20
                    font.pointSize: 10
                    text: qsTr("MyQQ0.0.0")
                }
                Label {
                    width: aboutWin.width
                    height: 20
                    font.pointSize: 10
                    text: qsTr("MyQQ公司 版权所有")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                Label {
                    width: aboutWin.width
                    height: 20
                    font.pointSize: 10
                    text: "Copyright © 1998-" + new Date().toLocaleString(
                              Qt.LocalDate, Locale.ShortFormat).slice(
                              0, 4) + " MyCompany All Rights Reserved"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                Label {
                    width: aboutWin.width
                    height: 20
                    font.pointSize: 10
                    text: "..."
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }
        }
        ScrollView {
            width: 378
            height: 72
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: contentHeight
                                       > height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            Text {
                anchors.centerIn: parent
                font.pointSize: 8
                text: qsTr("请遵守协议....")
            }
        }
    }
    footer: ToolBar {
        height: 36
        MouseCustomForWindow {
            onSendPos: {
                aboutWin.x += movedCoordinate.x
                aboutWin.y += movedCoordinate.y
            }
        }
        background: Rectangle {
            implicitWidth: parent.width
            implicitHeight: parent.height
            color: "#e5eff7"
        }
        ToolButton {
            id: okBtn
            x: 298
            y: 6
            text: qsTr("确认")
            hoverEnabled: true
            onClicked: {
                close()
            }

            background: Rectangle {
                implicitWidth: 70
                implicitHeight: 24
                radius: 15
                border.color: "#12b7fc"
                border.width: 1
                color: okBtn.hovered ? "#bee7fc" : "white"
            }
        }
    }
}
