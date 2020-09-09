import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import "../"
ApplicationWindow {
    id:addFriendsWin
    visible: true
    width:826
    height:594
title: qsTr("查找")
    flags: Qt.FramelessWindowHint | Qt.Window //取消窗口边缘框架
    background: Rectangle {
        border.width: 1
        border.color: "lightgray"
        color: "white"
    }
    MouseCustomForWindow {
        onSendPos: {
            addFriendsWin.x += movedCoordinate.x
            addFriendsWin.y += movedCoordinate.y
        }
    }
header: ToolBar{
    height: 30
    MouseCustomForWindow {
        onSendPos: {
            addFriendsWin.x += movedCoordinate.x
            addFriendsWin.y += movedCoordinate.y
        }
    }
    RowLayout {
        x: 10
        height: parent.height
        Image {
            source: "qrc:/images/mainInterface/qqWhite.ico"
        }
        Label {
            text: qsTr("关于MyQQ")
            color: "white"
        }
    }
}

}
