import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import "qrc:/"

//头像悬浮时弹出的界面
Window {
    property bool headImgHovered: false //代表鼠标是否悬浮在头像 初始化为false 当设置为true时启动定时timer1sForStart
    property bool inside: false //代表鼠标是否在窗口里
    property string name: ""
    property int grade: -1
    property string signature: ""
    property string tag: ""
    id: win
    width: 292
    height: 109
    flags: Qt.FramelessWindowHint
    visible: true
    onHeadImgHoveredChanged: {
        if (!headImgHovered) {
            if (!win.inside)
                timer1sForStart.restart()
        }
    }

    onVisibleChanged: {
        if (!visible) {
            inside = false
            headImgHovered = false //关闭时回到初始化状态
        }
    }
    //等级图标更新
    onGradeChanged: {
        console.log("grade=", grade)
        func.resetGradeModel(gradeModel, grade)
    }

    //鼠标区
    MouseArea {
        id: winMouse
        width: parent.width
        height: parent.height
        hoverEnabled: true
        onContainsMouseChanged: {
            if (containsMouse) {
                win.inside = true //标记在里头
            } else {
                win.inside = false
                timer1sForStart.restart()
            }
        }
    }
    //背景
    BorderImage {
        id: i
        asynchronous: true
        width: parent.width
        height: parent.height

        source: "qrc:/images/mainInterface/headInfBgd.png"
    }

    //昵称栏
    Label {
        id: nameLab
        x: 15
        y: 20
        text: win.tag == "" ? win.name : win.tag
        font.pixelSize: 22
        color: "white"
        MouseForShape {
            shapeInside: Qt.PointingHandCursor
            shapeOutside: Qt.ArrowCursor
            onContainsMouseChanged: {
                if (containsMouse) {
                    win.inside = true
                    nameLab.font.underline = true
                } else {
                    nameLab.font.underline = false
                }
            }
        }
    }
    //个性签名栏
    Label {
        property int posx: 0
        property int posy: 0
        id: sigTI
        x: nameLab.x
        y: nameLab.y + nameLab.height + 5
        font.pointSize: 10
        text: signature
        color: "white"
        width: win.width - x //占用一行
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true
            propagateComposedEvents: true
            onContainsMouseChanged: {
                if (containsMouse) {
                    sigTip.visible = true
                    win.inside = true
                } else {
                    timer1sForSig.restart()
                }
            }
            onPositionChanged: {
                if (!sigTip.visible) {
                    sigTI.posx = mouseX
                    sigTI.posy = mouseY
                }
            }
        }
        //个性签名提示
        ToolTip {
            id: sigTip
            x: sigTI.posx
            y: sigTI.posy + 20
            delay: 1000
            visible: false

            background: Label {
                property string str: "个性签名更新:" + signature
                property int bw: 0
                property int bh: 0
                id: sigTipBack
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
    //等级图标
    GridLayout {
        property int posx: 0
        property int posy: 0
        id: gridLay
        x: nameLab.x
        y: sigTI.y + sigTI.height + 5
        columns: 10
        rowSpacing: 3
        Repeater {
            id: rep
            model: gradeModel
            ToolButton {
                background: Image {
                    sourceSize: Qt.size(19, 19)
                    source: img
                }
            }
        }

        ToolTip {
            id: gradeimgTip
            x: gridLay.posx
            y: gridLay.posy - 20 //tip不能超过界面这里反方向掩饰一下
            visible: false
            delay: 1000
            background: Label {
                font.pointSize: 11
                text: "等级:" + grade
                verticalAlignment: Text.AlignVCenter
                background: Rectangle {
                    height: parent.height
                    width: parent.width
                    color: "#fefee1"
                }
            }
        }
    }
    MouseArea {
        id: gridMouse
        x: gridLay.x
        y: gridLay.y
        width: win.width - x //占用一行
        height: gridLay.height
        hoverEnabled: true
        onContainsMouseChanged: {
            if (containsMouse) {
                cursorShape = Qt.PointingHandCursor
                win.inside = true
                gradeimgTip.visible = true
            } else {
                cursorShape = Qt.ArrowCursor
                timer1sForSig.restart()
                gradeimgTip.close()
            }
        }
        onPositionChanged: {
            if (!gradeimgTip.visible) {
                gridLay.posx = mouseX
                gridLay.posy = mouseY
            }
        }
    }
    //模型
    ListModel {
        id: gradeModel
    }

    //延迟1s关闭个性签名提示
    Timer {
        id: timer1sForSig
        interval: 1000
        onTriggered: {
            sigTip.close()
        }
    }
    //防止打开界面时离开头像范围马上关闭
    Timer {
        id: timer1sForStart
        interval: 500
        onTriggered: {
            if (!win.inside && !headImgHovered)
                win.hide()
        }
    }
}
