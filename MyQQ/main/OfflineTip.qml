import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import "qrc:/"

//强制离线提示框
ApplicationWindow {
    property string ip: ""
    property string host: ""
    property string loginTime: ""
    id: offWin
    visible: true //必须为true，loader加载时才能显示任务栏图标,并且关闭后重新用loader加载才能显示
    width: 340
    height: 155
    title: qsTr("警告")
    modality: Qt.ApplicationModal
    //topmost,qqMainWin之上
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    onClosing: {
        hide() //不退出app
        loaderForOffline.source = ""
        console.log("offwin on closeing->")
        qqMainWin.close()
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
                offWin.x += movedCoordinate.x
                offWin.y += movedCoordinate.y
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
                text: qsTr("警告")
                color: "white"
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
        TextArea {
            readOnly: true
            textFormat: Text.RichText
            wrapMode: TextEdit.WrapAnywhere
            text: "<font style='color:red; text-indent:2em;'>" + "账号在别处登录，你已被挤下线！" + "</font>"
                  + "<br/>IP:" + "<font style='color:red;'>" + ip + "</font>"
                  + "<br/>计算机名称:" + "<font style='color:red;'>" + host + "</font>"
                  + "<br/>登陆时间:" + "<font style='color:red;'>" + loginTime + "</font>"
            font.pointSize: 10
        }
    }
    footer: ToolBar {
        height: 39
        MouseCustomForWindow {
            onSendPos: {
                offWin.x += movedCoordinate.x
                offWin.y += movedCoordinate.y
            }
        }
        background: Rectangle {
            implicitWidth: 340
            implicitHeight: 39
            color: "#e5eff7"
        }
        ToolButton {
            id: okBtn
            x: 240
            y: 7.5
            text: qsTr("确认")
            hoverEnabled: true
            onClicked: {
                offWin.close()
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
