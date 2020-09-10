import QtQuick 2.11
import QtQuick.Window 2.11
//帮助弹出小菜单
Window {
    id: win
    property alias updgradeBtn: updgradeBtn
    property alias aboutBtn: aboutBtn
    width: 152
    height: 82
    visible: false
    flags: Qt.FramelessWindowHint
    Connections {
        target: qqMainWin
        onSendCoordinate: {
            console.log("send")
            console.log(pos.x, pos.y)
            win.x = pos.x
            win.y = pos.y
            console.log("atrribute->", win.width, win.height)
        }
    }
    Rectangle {
        width: parent.width
        height: parent.height
        border.width: 1
        border.color: "lightgray"
        Column {
            x: 1
            y: 11
            ButtonForMenu {
                id: updgradeBtn
                hoverEnabled: true
                label.text: qsTr("升级")
                recBackground.color: hovered ? "#f2f2f2" : "white"
                action: actions.updgradeAct
            }
            ButtonForMenu {
                id: aboutBtn
                hoverEnabled: true
                label.text: qsTr("关于MyQQ")
                recBackground.color: hovered ? "#f2f2f2" : "white"
                action: actions.aboutAct
            }
        }
    }
}
