import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Rectangle {
    property string school: ""
    property string detail: ""
    property bool isChecked: false
    property int index: -1
    signal clicked(int index)
    id: item
    implicitWidth: 384
    implicitHeight: 65
    color: "transparent"
    onIsCheckedChanged: {
        if (isChecked) {
            color = Qt.darker("#eeeeee", 1.05)
            clicked(index) //发送信号提醒其它控件改变状态
        } else {
            color = "transparent"
        }
    }

    Label {
        id: labShool
        x: 60
        y: 18
        width: 250
        height: 20
        text: school
        font.pointSize: 11
        font.family: "新宋体"
        elide: Text.ElideRight
    } //ebebeb
    Label {
        id: label
        x: 60
        y: 38
        text: detail
        width: 250
        height: 20
        color: "gray"
        font.pointSize: 11
        font.family: "新宋体"
        elide: Text.ElideRight
    }
    //鼠标区在按钮之下好处理逻辑
    MouseArea {
        width: parent.width
        height: parent.height
        hoverEnabled: true
        onClicked: {
            console.log("clicked")
            item.isChecked = true
        }
        onHoveredChanged: {
            console.log("hover changed")
            if (!item.isChecked) {
                if (containsMouse) {
                    item.color = "#eeeeee"
                } else {
                    item.color = "transparent"
                }
            }
        }
    }
    RowLayout {
        x: 320
        y: 25
        spacing: 15
        Button {
            onClicked: {
                console.log("tran can button clicked")
            }
            background: Image {
                source: "qrc:/images/mainInterface/trashCan.png"
            }
        }
        Button {
            onClicked: {
                console.log("pen button clicked")
            }

            background: Image {
                source: "qrc:/images/mainInterface/pen.png"
            }
        }
    }
}
