import QtQuick 2.5
import QtQuick.Controls 2.5
import QtQuick.Controls 1.5 as C1
import QtQuick.Layouts 1.1

RowLayout {
    height: 40
    x: 260 * dpJust
    y: 20 * dpJust
    C1.ComboBox {
        id: langCBox
        Layout.alignment: Qt.AlignVCenter
        clip: true
        width: 60
        height: 30
        model: ["简体中文", "繁體中文", "English"]
        currentIndex: 0
        onCurrentIndexChanged: {
            if(currentIndex===0)
                return
            tip.visible = true
        }

        ToolTip {
            id: tip
            x: 55
            y: 25
            text: "该功能未实现！"
            delay: 200
            timeout: 2000
            contentItem: Text {
                text: tip.text
                font: tip.font
                color: "red"
            }
            background: Rectangle {
                radius: 8
                color: "white"
                border.color: "blue"
            }
        }
    }

    Button {
        id: minBtn
        Layout.alignment: Qt.AlignVCenter
        //flat: true
        onClicked: mainWin.hide()
        hoverEnabled: true
        onHoveredChanged: {
            if (minBtn.hovered) {
                tM.font.pixelSize = 18
                tM.font.bold = true
            } else {
                tM.font.pixelSize = 16
                tM.font.bold = false
            }
        }
        Text {
            id: tM
            anchors.fill: parent
            text: "最小化"
            font.pixelSize: 16
        }
        background: Rectangle {
            color: "transparent"
            implicitWidth: 50
            implicitHeight: langCBox.height
        }
    }
    Button {
        id: closeBtn
        Layout.alignment: Qt.AlignVCenter
        //flat: true
        hoverEnabled: true
        onClicked: Qt.quit()
        onHoveredChanged: {
            if (closeBtn.hovered) {
                tC.font.pixelSize = 18
                tC.font.bold = true
            } else {
                tC.font.pixelSize = 16
                tC.font.bold = false
            }
        }
        Text {
            id: tC
            text: "关 闭"
            anchors.fill: parent
            font.pixelSize: 16
        }
        background: Rectangle {
            color: "transparent"
            implicitWidth: 50
            implicitHeight: langCBox.height
        }
    }
}
