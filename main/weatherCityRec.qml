import QtQuick 2.11
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0

//天气城市列表控件
Button {
    property alias img: img //右边小图片
    property alias tex: backlab.text
    property alias color: backRec.color
    property bool isSelected: false
    property alias icoRec: icoRec
    property bool icoIsVisible: false
    property bool icoVisible: false
    signal hoveredSmallIco(bool hovered)
    signal clickedIco
    width: 220
    height: 32
    hoverEnabled: true

    onTexChanged: {
        if (tex === "") {
            hoverEnabled = false
            isSelected = false
            icoRec.color = backRec.color
        } else {
            hoverEnabled = true
        }
    }
    onClicked: {
        isSelected = true
    }
    //选择与否颜色变化
    onIsSelectedChanged: {
        if (!isSelected)
            backRec.color = "#4b7c97"
        else
            backRec.color = "#5c88a1"
    }

    onHoveredChanged: {

        if (!icoIsVisible) {
            if (hovered) {
                icoVisible = true
                if (isSelected)
                    backRec.color = "#5c88a1"
                else
                    backRec.color = "#54829c"
            } else {
                icoVisible = false
                if (isSelected)
                    backRec.color = "#5c88a1"
                else
                    backRec.color = "#4b7c97"
            }
        }
    }
    background: Label {
        id: backlab
        width: 220
        height: 32
        color: "#ffffff" //"#4b7c97" //#5c88a1 54829c
        leftPadding: 5
        font.pointSize: 10
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        background: Rectangle {
            id: backRec
            implicitHeight: 32
            implicitWidth: 220
            color: "#4b7c97"
        }
        Rectangle {
            id: icoRec
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 10
            visible: icoVisible
            width: 13
            height: 13
            color: backRec.color
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                onHoveredChanged: {
                    hoveredSmallIco(containsMouse)
                }
                onClicked: {
                    mouse.accepted = true
                    clickedIco()
                }
            }
            Image {
                id: img
                visible: true
                sourceSize: Qt.size(13, 13)
                source: "qrc:/images/mainInterface/weather/cross.png" //496c80
            }
        }
    }
}
