import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
//消息列表控件或者好友控件
Button {
    property alias arrow: arrow
    property alias name: name
    property alias ratio: ratio
    property alias edit: edit
    property bool muter: false //防止丢失焦点
    property int sumForFriends: 0
    property int activeLine: 0
    property string backColor: "transparent"
    property int rotat: 0
    property int h: 35
    property int w: 100
    property string set: "none"
    property int pos: 0
    property bool isChecked: false
    signal editFinished(string t)
    id: lb
    width: w
    height: h
    visible: true
    hoverEnabled: true

    onIsCheckedChanged: {
        console.log("ischecke=", isChecked)
        if (isChecked)
            backColor = "#f2f2f2"
        else
            backColor = "transparent"
    }
    background: Rectangle {
        implicitWidth: w
        implicitHeight: h
        color: backColor
        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            x: 10
            Image {
                id: arrow
                sourceSize: Qt.size(11, 11)
                source: "qrc:images/mainInterface/arrowLeft.png"
                transform: Rotation {
                    origin.x: arrow.width / 2
                    origin.y: arrow.height / 2
                    axis {
                        x: 0
                        y: 0
                        z: 1
                    }
                    angle: rotat
                }
            }
            Label {
                id: name
                visible: !edit.visible
                text: qsTr("新朋友")
                font.pointSize: 10
                font.family: song.name
                // color: "transparent"
            }
            Label {
                id: ratio
                visible: !edit.visible
                text: qsTr(sumForFriends.toString(
                               ) + "/" + activeLine.toString())
                font.pointSize: 10
                font.family: song.name
            }
        }
    }
    //不能为背景，因为需要鼠标优先权
    TextField {
        property int lastLength: 0
        id: edit
        anchors.verticalCenter: parent.verticalCenter
        height: 24
        visible: false
        focus: true
        padding: 0
        width: w - arrow.width - 13
        x: 24
        z: 3
        font.family: "宋体"
        font.pointSize: 9
        hoverEnabled: true
        Keys.onLeftPressed: {
            console.log(cursorRectangle, text)
            cursorPosition = 0
        }

        onTextChanged: {
            var l = func.getCharByteLength(text)
            if (l > 24) {
                text = text.substring(0, lastLength)
            }
            lastLength = length
        }
        onEditingFinished: {
            if (muter) {
                selectAll()
                focus = true
                muter = false
                return
            }
            if (visible)
                //防止丢失焦点和enter重复发出
                editFinished(text)
            visible = false
        }

        background: Rectangle {
            implicitWidth: edit.width
            implicitHeight: edit.height
            radius: 1
            color: "white"
            border.color: edit.hovered ? "#1583dd" : "lightgray"
            border.width: edit.hovered ? 2 : 1
        }
    }
}
