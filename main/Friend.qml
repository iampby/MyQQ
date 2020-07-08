import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Button {
    property int w: parent.width //宽度
    property int h: 60 //高度
    property alias headImg: hImg.source //头像路径
    property alias topText: top.text //昵称+备注
    property alias bottomText: bottom.text //个性签名
    background: Rectangle {
        implicitHeight: h
        implicitWidth: w
        RowLayout {
            x: 12
            height: h
            spacing: 12
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Rectangle {
                id: headRec
                width: 40
                height: 40
                color: "transparent"
                radius: 90
                Image {
                    id: hImg
                    anchors.fill: parent
                    asynchronous: true
                    cache: false
                    fillMode: Image.PreserveAspectCrop
                    sourceSize: Qt.size(40, 40)
                }
            }
            RowLayout {
                spacing: 0
                Layout.alignment: Qt.AlignLeft
                Label {
                    id: top
                    height: 15
                }
                Label {
                    id: bottom
                    height: 15
                }
            }
        }
    }
}
