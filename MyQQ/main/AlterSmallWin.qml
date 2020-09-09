import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import "qrc:/"

//修改备注小窗口
ApplicationWindow {
    property string ttext: qsTr("修改好友备注")
    property alias value: edit.text
    id: alterWin
    visible: true //必须为true，loader加载时才能显示任务栏图标,并且关闭后重新用loader加载才能显示
    width: 324
    height: 152
    title: ttext

    modality: Qt.ApplicationModal
    //topmost,qqMainWin之上
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    onClosing: {
        hide() //不退出app
        loaderForAlterTag.source = ""
        console.log("alterwin on closeing")
    }
    background: Rectangle {
        border.width: 1
        border.color: "lightgray"
        color: "white"
    }

    header: ToolBar {

        height: 30
        MouseCustomForWindow {
            onSendPos: {
                alterWin.x += movedCoordinate.x
                alterWin.y += movedCoordinate.y
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
                id: tlabel
                text: ttext
                color: "white"
            }
        }
        Button {
            id: closeBtn
            x: parent.width - width
            width: 28
            height: 30
            onClicked: {
                close()
            }

            background: Rectangle {
                implicitHeight: 30
                implicitWidth: 28
                color: "white"
                Image {
                    id: closeImg
                    sourceSize: Qt.size(28, 30)
                    source: "qrc:/images/mainInterface/alterHeadClose.png"
                }
                ColorOverlay {
                    anchors.fill: closeImg
                    source: closeImg
                    color: closeBtn.hovered ? (closeBtn.pressed ? Qt.darker(
                                                                      "#d44027",
                                                                      1.2) : "#d44027") : "#12b7f5"
                }
            }
        }
        background: Rectangle {
            implicitWidth: 340
            implicitHeight: 30
            color: "#12b7fc"
        }
    }
    ColumnLayout {
        x: 30
        y: contentItem.height / 2 - 20
        spacing: 5
        Label {
            text: "请输入好友备注："
            color: "black"
            font.pixelSize: 12
        }
        TextField {
            id: edit
            font.family: "新宋体"
            focus: true

            Keys.onEnterPressed: okBtn.clicked()
            Keys.onReturnPressed: okBtn.clicked()
            background: Rectangle {
                implicitWidth: 280
                implicitHeight: 22
                radius: 3
                color: "white"
                border.color: edit.hovered ? "#1583dd" : "lightgray"
                border.width: edit.hovered ? 2 : 1
            }
        }
    }
    footer: ToolBar {
        height: 39
        MouseCustomForWindow {
            onSendPos: {
                alterWin.x += movedCoordinate.x
                alterWin.y += movedCoordinate.y
            }
        }
        background: Rectangle {
            implicitWidth: 324
            implicitHeight: 39
            color: "#e5eff7"
        }
        RowLayout {
            x: 170
            y: 7.5
            ToolButton {
                id: okBtn
                text: qsTr("确认")
                hoverEnabled: true
                Keys.onEnterPressed: clicked()
                Keys.onReturnPressed: clicked()
                onClicked: {
                    console.log("clicked()")
                    actions.alterFTagAct.trigger(alterWin)
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
            ToolButton {
                id: cancelBtn
                text: qsTr("取消")
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
                    color: cancelBtn.hovered ? "#bee7fc" : "white"
                }
            }
        }
    }
}
