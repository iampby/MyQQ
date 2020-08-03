﻿import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.12

Button {
    property int w: parent.width //宽度
    property int h: 60 //高度
    property alias headImg: hImg.source //头像路径
    property alias topLab: topLab
    property alias bottomLab: bottomLab
    property string name: "" //昵称+备注
    property string label: "" //备注
    property alias bottomText: bottomLab.text //个性签名
    background: Rectangle {
        implicitHeight: h
        implicitWidth: w
        RowLayout {
            x: 12
            height: h
            spacing: 12
            Rectangle {
                id: headRec
                width: 40
                height: 40
                clip: true
                //color: "transparent" 遮罩源不能为透明
                radius: 90
                Image {
                    id: hImg
                    asynchronous: true
                    cache: false
                    visible: false
                    fillMode: Image.PreserveAspectCrop
                    sourceSize: Qt.size(40, 40)
                }
                //遮罩
                OpacityMask {
                    width: headRec.width
                    height: headRec.height
                    source: hImg
                    maskSource: headRec
                }
            }
            ColumnLayout {
                spacing: 3
                Label {
                    id: topLab
                    clip: true
                    width: w - 5
                    height: 18
                    textFormat: Qt.RichText
                    font.pointSize: 11
                    text: label == "" ? name : name + "<font style='color:gray;'>("
                                        + label + ")</font>"
                }
                Label {
                    id: bottomLab
                    font.pointSize: 9
                    clip: topLab.clip
                    width: topLab.width
                    height: 15
                    color: "gray"
                }
            }
        }
    }
}