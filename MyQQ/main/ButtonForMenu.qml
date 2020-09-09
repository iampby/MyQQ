import QtQuick 2.0
import QtQuick.Controls 2.5

//qqmainwin菜单栏的升级和关于按钮
ToolButton {
    property alias recBackground: recBackg
    property alias iconL: img
    property alias label: label
    width: 150
    height: 30
    background: Rectangle {
        id: recBackg
        implicitWidth: 150
        implicitHeight: 30
        color: "white"
        Image {
            id: img
            x: 25
            y: (parent.height - height) / 2
            sourceSize: Qt.size(25, 24)
            source: ""
        }
        Label {
            id: label
            x: 45
            height: parent.height
            text: qsTr("text")
            verticalAlignment: Text.AlignVCenter
        }
    }
}
