import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtGraphicalEffects 1.12
import "../"

//用户个人资料
Window {
    id: win
    width: 722
    height: 522
    title: "个人资料"
    flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
    color: "lightgray" //边界颜色
    onClosing: {
        console.log("invidualData onClosing")
        win.hide() //躲藏避免关闭主事件圈
        loaderForAlterInfo.source = ""
    }
    //移动鼠标
    MouseCustomForWindow {
        onSendPos: {
            win.x += movedCoordinate.x
            win.y += movedCoordinate.y
        }
    }
    //躯干
    Rectangle {
        id: body
        x: 1
        y: 1
        width: win.width - 2
        height: win.height - 2
        //左边封面
        Rectangle {
            id: leftRec
            width: 358
            height: 358
            Image {
                id: leftImg
                anchors.fill: parent
                sourceSize: Qt.size(358, 358)
                source: "qrc:/images/mainInterface/userInfoDefault.png"
            }
            //更换封面
            Button {
                property int xTip: 0
                property int yTip: 0
                id: renewCover
                x: 12
                y: 12
                width: 70
                height: 18
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onPositionChanged: {
                        if (!renewCover.visible) {
                            renewCover.xTip = mouseX
                            renewCover.yTip = mouseY
                        }
                    }
                    onHoveredChanged: {
                        if (containsMouse)
                            renewCoverTip.visible = true
                        else
                            renewCoverTip.visible = false
                    }
                    onContainsMouseChanged: {
                        if (containsMouse) {
                            cursorShape = Qt.PointingHandCursor
                        } else
                            cursorShape = Qt.ArrowCursor
                    }
                }

                background: Rectangle {
                    implicitHeight: 18
                    implicitWidth: 70
                    color: "transparent"
                    Image {
                        id: garment
                        y: 2
                        sourceSize: Qt.size(15, 13)
                        source: "qrc:/images/mainInterface/userInfoClothing.png"
                    }
                    ColorOverlay {
                        anchors.fill: garment
                        source: garment
                        color: renewCover.hovered ? "lightblack" : "white"
                    }
                    Text {
                        id: coverText
                        x: 18
                        y: 2
                        color: renewCover.hovered ? "lightblack" : "white"
                        font.pointSize: 10
                        text: qsTr("更换封面")

                        layer.enabled: true
                        layer.effect: Glow {
                            color: Qt.rgba(200, 200, 200)
                            samples: 12
                            spread: 0.2
                        }
                    }
                }
            }
            ToolTip {
                id: renewCoverTip
                x: renewCover.xTip
                y: renewCover.yTip + 25
                width: 60
                height: 22
                delay: 1000
                Text {
                    anchors.centerIn: parent
                    text: "更换封面"
                    color: "gray"
                    horizontalAlignment: Text.AlignVCenter
                    verticalAlignment: Text.AlignHCenter
                }
            }
        }
        Rectangle {
            id: lfetBRec
            y: 358
            width: leftRec.width
            height: win.height - 2 - leftRec.height
            color: "#5a898a"
            //头像区
            Rectangle {
                x: 59
                y: 45
                width: 240
                height: 60
                color: "transparent"
                //头像
                Rectangle {
                    property int posx: 0
                    property int posy: 0
                    id: recHead
                    y: 4
                    width: 56
                    height: 56
                    radius: 90
                    clip: true
                    color: "transparent"
                    border.color: "#23cdfe"
                    border.width: 0
                    MouseArea {
                        id: mouseHeadImg
                        width: parent.width
                        height: parent.height
                        hoverEnabled: true
                        onPressed: {
                            if (signatureTF.focus) {
                                //如果可以 个性签名编辑就能完成
                                signatureTF.focus = !signatureTF.focus
                            }
                        }
                        onHoveredChanged: {
                            if (containsMouse) {
                                recHead.border.width = 2
                                headTip.show("点击更换头像")
                            } else {
                                recHead.border.width = 0
                                headTip.close()
                            }
                        }
                        onPositionChanged: {
                            if (!headTip.visible) {
                                recHead.posx = mouseX
                                recHead.posy = mouseY
                            }
                        }
                    }
                    Rectangle {
                        x: 2
                        y: 2
                        width: 52
                        height: 52
                        radius: 90
                        clip: true
                        Image {
                            id: imgHead
                            antialiasing: true
                            asynchronous: true
                            cache: false
                            fillMode: Image.PreserveAspectCrop
                            source: inCenterLoader.item.headSource
                            visible: false
                            sourceSize: Qt.size(parent.size, parent.size)
                        }
                    }
                    //遮罩
                    OpacityMask {
                        x: 2
                        y: 2
                        width: 52
                        height: 52
                        source: imgHead
                        maskSource: recHead
                    }
                }
                //头像提示
                ToolTip {
                    id: headTip
                    x: recHead.x + recHead.posx
                    y: recHead.y + recHead.posy + 20
                    width: 85
                    height: 25
                    delay: 1000
                    visible: false
                    font.pointSize: 10
                }
                //昵称
                Label {
                    id: nameLab
                    height: 24
                    width: 170
                    x: recHead.x + recHead.width + 10
                    y: recHead.y
                    font.pointSize: 13
                    font.family: song.name
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    maximumLineCount: 1
                    text: inCenterLoader.item.name
                    color: "#ffffff"
                }
                //个性签名
                TextField {
                    property int posx: 0
                    property int posy: 0
                    id: signatureTF
                    x: nameLab.x
                    y: nameLab.y + nameLab.height
                    hoverEnabled: true
                    placeholderText: "编辑个性签名"
                    placeholderTextColor: "white"
                    text: sigMetrics.elidedText
                    color: "white"
                    font.pointSize: 10
                    font.family: song.name
                    verticalAlignment: Text.AlignVCenter

                    onActiveFocusChanged: {
                        if (activeFocus) {
                            text = sigMetrics.text
                            selectAll()
                            color = "black"
                        } else {
                            inCenterLoader.item.signature = text //失去焦点 把文本赋值给个性签名
                            text = sigMetrics.elidedText
                            color = "white"
                        }
                    }
                    onTextChanged: {
                        if (text.length > 80) {
                            text = text.substring(0, 80) //只能为80字
                        }
                    }
                    onEditingFinished: {
                        signature = text
                        focus = false
                        sigBgd.focus = true
                    }
                    background: Rectangle {
                        id: sigBgd
                        implicitWidth: headRec.width - signatureTF.x - weatherRec.width - 3
                        implicitHeight: 16
                        color: signatureTF.activeFocus ? "white" : "transparent"
                        border.width: signatureTF.hovered ? (signatureTF.activeFocus ? 0 : 1) : 0
                        border.color: "dimgray"
                        Keys.onReturnPressed: {
                            if (!signatureTF.activeFocus)
                                signatureTF.focus = true
                        }
                    }

                    MouseArea {
                        width: parent.width
                        height: parent.height
                        hoverEnabled: true
                        onPressed: {
                            mouse.accepted = false
                            if (signatureTF.focus) {
                                signatureTF.focus = !signatureTF.focus
                            }
                        }

                        onHoveredChanged: {
                            console.log("signature tip hovered change")
                            if (containsMouse) {
                                sigTip.visible = true
                            } else {
                                tipTimer1sForSig.restart()
                            }
                        }
                        onPositionChanged: {
                            if (!sigTip.visible) {
                                signatureTF.posx = mouseX
                                signatureTF.posy = mouseY
                            }
                            mouse.accepted = true
                        }
                    }
                    TextMetrics {
                        id: sigMetrics
                        font: signatureTF.font
                        text: inCenterLoader.item.signature
                        elideWidth: signatureTF.width
                        elide: Text.ElideRight
                    }
                }
                //个性签名提示
                ToolTip {
                    id: sigTip
                    x: signatureTF.x + signatureTF.posx
                    y: signatureTF.y + signatureTF.posy + 20
                    delay: 1000
                    visible: false
                    background: Label {
                        property string str: "个性签名更新:" + signature
                        property int bw: 0
                        property int bh: 0
                        id: sigTipBack
                        // elide: Text.ElideRight
                        font.pointSize: 10
                        color: "gray"
                        wrapMode: Text.Wrap
                        maximumLineCount: 2
                        lineHeight: 1.1
                        verticalAlignment: Text.AlignVCenter
                        onStrChanged: {
                            var one = "个性签名更新:", two = "" //第一行字符 第二行字符

                            var bytes = func.getCharByteLength(one) //初始化累计字节数
                            var length = str.length
                            for (var i = 7; i < length; i++) {
                                var temp = str.charAt(i)
                                console.log("temp", temp, bytes)
                                var l = func.getCharByteLength(temp) //单个字符字节数
                                if (bytes + l < 29) {
                                    //如果小于29个字节则为第一行字符
                                    one += temp
                                    bytes += l
                                } else {
                                    two = str.substring(i, length) //设置第二行字符
                                    break
                                }
                            }
                            sigTipBack.text = one + "\n"
                            sigTipMetc.text = one
                            sigTipBack.bw = sigTipMetc.width + 3
                            if (two !== "") {
                                sigTipBack.bh = (sigTipMetc.height + 4) * 2
                                sigTipMetc.text = two //用于获取省略的字符
                                sigTipBack.text += sigTipMetc.elidedText
                            } else {
                                sigTipBack.bh = sigTipMetc.height + 4
                            }
                        }
                        background: Rectangle {
                            height: sigTipBack.bh
                            width: sigTipBack.bw
                            color: "#fefee1"
                        }
                        TextMetrics {
                            id: sigTipMetc
                            font: sigTipBack.font
                            elideWidth: sigTipBack.bw - 3 //省略字符宽度为提示背景宽度
                            elide: Text.ElideRight
                        }
                    }
                }
            }
        }
    }
    //时钟
    //延迟关闭提示
    //个性签名
    Timer {
        id: tipTimer1sForSig
        interval: 1000
        onTriggered: {
            sigTip.close()
        }
    }
}
