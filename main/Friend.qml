import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Button {
    property int w: parent.width //宽度
    property int h: 60 //高度
    property alias headImg: headImg.source //头像路径
    property alias topText: top.text //昵称+备注
    property alias bottomText: bottom.text //个性签名
    background: Rectangle {
        implicitHeight: h
        implicitWidth: w
        RowLayout {
            anchors.fill: parent
            x: 12
            spacing: 12
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Rectangle {
                id: headRec
                width: 40
                height: 40
                color: "transparent"
                radius: 90
                Image {
                    id: headImg
                    asynchronous: true
                    fillMode: Image.PreserveAspectCrop
                    sourceSize: Qt.size(headRec.width, headRec.height)
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
