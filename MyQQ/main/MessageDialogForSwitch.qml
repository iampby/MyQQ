import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11
import QtGraphicalEffects 1.0
import "qrc:/"

ApplicationWindow {
    id: mesWin
    visible: true //必须为true，loader加载时才能显示任务栏图标,并且关闭后重新用loader加载才能显示
    width: 340
    height: 155
    title: qsTr("提示")
    modality: Qt.ApplicationModal
    flags: Qt.Window | Qt.FramelessWindowHint
           | Qt.WindowStaysOnTopHint //窗口模式，可以显示任务栏图标 topmost,qqMainWin之上
    x: qqMainWin.x + qqMainWin.width / 2 - width / 2
    y: qqMainWin.y + qqMainWin.height / 2 - height / 2
    onClosing: {
        hide() //不退出app
        loaderForMesWin.source = ""
        console.log("mesWin on closeing->")
    }
    background: Rectangle {
        border.width: 1
        border.color: "lightgray"
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#ffffff"
            }
            GradientStop {
                position: 1
                color: "#c1bbf9"
            }
        }
    }

    header: ToolBar {
        height: 30
        MouseCustomForWindow {
            onSendPos: {
                mesWin.x += movedCoordinate.x
                mesWin.y += movedCoordinate.y
            }
        }
        RowLayout {
            x: 10
            height: parent.height
            Image {
                id: name
                source: "qrc:/images/mainInterface/qqWhite.ico"
            }
            Label {
                text: qsTr("提示")
                color: "white"
            }
        }
        ToolButton {
            anchors.right: parent.right
            id: closeBtn
            hoverEnabled: true
            onClicked: {
                mesWin.close()
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
            implicitWidth: 340
            implicitHeight: 30
            color: "#12b7fc"
        }
    }
    RowLayout {
        x: 25
        y: contentItem.height / 2 - 25
        spacing: 10

        Rectangle {
            width: 50
            height: 50
            radius: 90
            color: "red"
            Label {
                anchors.centerIn: parent
                text: "!"
                color: "white"
                font.pixelSize: 45
            }
        }
        Label {
            text: "你确定要退出，更换其他账号登录吗?"
            font.pointSize: 10
        }
    }
    footer: ToolBar {
        height: 39
        MouseCustomForWindow {
            onSendPos: {
                mesWin.x += movedCoordinate.x
                mesWin.y += movedCoordinate.y
            }
        }
        background: Rectangle {
            implicitWidth: 340
            implicitHeight: 39
            color: "#e5eff7"
        }
        RowLayout {
            x: 190
            y: 7.5
            ToolButton {
                id: okBtn
                text: qsTr("确认")
                hoverEnabled: true
                action: actions.mesWinForOkAct
                background: Rectangle {
                    implicitWidth: 70
                    implicitHeight: 24
                    radius: 15
                    border.color: "#12b7fc"
                    border.width: 1
                    color: okBtn.hovered ? "#bee7fc" : "white"
                }
            }
            ToolButton {
                id: cancelBtn
                text: qsTr("取消")
                hoverEnabled: true
                onClicked: {
                    mesWin.close()
                }
                background: Rectangle {
                    implicitWidth: 70
                    implicitHeight: 24
                    radius: 15
                    border.color: "#12b7fc"
                    border.width: 1
                    color: cancelBtn.hovered ? "#bee7fc" : "white"
                }
            }
        }
    }
}
