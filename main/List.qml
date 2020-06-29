import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Button {
    property alias arrow: arrow
    property alias name: name
    property alias ratio: ratio
    property int sumForFriends: 0
    property int activeLine: 0
    property string backColor: "transparent"
    property int rotat: 0
    property int h: 35
    property int w: 100
    property string set:"none"
    property int pos:0
    id:lb
    width: w
    height: h
    visible: true
    hoverEnabled: true

    onHoveredChanged: {
        if (hovered)
            backColor = "#f2f2f2"
        else
            backColor = "transparent"
    }
    background: Rectangle {
        implicitWidth:w
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
                text: qsTr("新朋友")
                font.pointSize: 10
                font.family: song.name
                // color: "transparent"
            }
            Label {
                id: ratio
                text: qsTr(sumForFriends.toString(
                               ) + "/" + activeLine.toString())
                font.pointSize: 10
                font.family: song.name
            }
        }
    }
}
