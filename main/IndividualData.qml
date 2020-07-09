import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtGraphicalEffects 1.12
import "../"

//用户个人资料
Window {
    id: win
    width: 722
    height: 522
    visible: true
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
                                cursorShape = Qt.PointingHandCursor
                                recHead.border.width = 2
                                headTip.show("点击更换头像")
                            } else {
                                cursorShape = Qt.ArrowCursor
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
                    MouseForShape {
                        shapeInside: Qt.SizeVerCursor
                        shapeOutside: Qt.ArrowCursor
                    }
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
        //右边
        Item {
            id: right
            x: 358
            clip: true
            width: parent.width - 358
            height: parent.height

            Button {
                property int posx: 0
                property int posy: 0
                id: minBtn
                x: right.width - 64
                width: 32
                height: 33
                onClicked: {
                    win.showMinimized()
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPositionChanged: {
                        //记录tip坐标
                        if (!minTip.visible) {
                            minBtn.posx = mouseX
                            minBtn.posy = mouseY
                        }
                    }
                    onPressed: {
                        mouse.accepted = false
                    }
                    onHoveredChanged: {
                        if (containsMouse) {
                            minTip.show("最小化")
                        } else {
                            minTip.close()
                        }
                    }
                }
                background: Rectangle {
                    implicitWidth: 32
                    implicitHeight: 33
                    color: "#5e6368"
                    Image {
                        id: minImg
                        source: "qrc:/images/mainInterface/userInfoMin.png"
                        sourceSize: Qt.size(32, 33)
                    }
                    ColorOverlay {
                        anchors.fill: minImg
                        source: minImg
                        color: minBtn.hovered ? (minBtn.pressed ? "#dedede" : Qt.lighter(
                                                                      "#dedede",
                                                                      1.10)) : "white"
                    }
                }
                //最小化提示
                ToolTip {
                    id: minTip
                    x: minBtn.posx
                    y: minBtn.posy + 20
                    delay: 1000
                    visible: false
                    font.pointSize: 10
                    Text {
                        anchors.fill: parent
                        text: minTip.text
                        font: minTip.font
                        color: "lightgray"
                    }
                    background: Rectangle {
                        height: minTip.height
                        width: minTip.width
                        border.width: 1
                        border.color: "lightgray"
                    }
                }
            }
            Button {
                property int posx: 0
                property int posy: 0
                id: closeBtn
                x: right.width - 32
                width: 32
                height: 33
                onClicked: {
                    win.close()
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPositionChanged: {
                        //记录tip坐标
                        if (!closeTip.visible) {
                            closeBtn.posx = mouseX
                            closeBtn.posy = mouseY
                        }
                    }
                    onPressed: {
                        mouse.accepted = false
                    }

                    onHoveredChanged: {
                        if (containsMouse) {
                            closeTip.show("关闭")
                        } else {
                            closeTip.close()
                        }
                    }
                }

                background: Rectangle {
                    implicitWidth: 32
                    implicitHeight: 33
                    color: "#5e6368"
                    Image {
                        id: closeImg
                        source: "qrc:/images/mainInterface/userInfoClose.png"
                        sourceSize: Qt.size(32, 33)
                    }
                    ColorOverlay {
                        anchors.fill: closeImg
                        source: closeImg //ededed dedede ff5439 e04a32
                        color: closeBtn.hovered ? (closeBtn.pressed ? "#e04a32" : Qt.lighter(
                                                                          "#e04a32",
                                                                          1.2)) : "white"
                    }
                }
                //关闭提示
                ToolTip {
                    id: closeTip
                    x: closeBtn.posx
                    y: closeBtn.posy + 20
                    delay: 1000
                    visible: false
                    font.pointSize: 10
                    Text {
                        anchors.fill: parent
                        text: closeTip.text
                        font: closeTip.font
                        color: "lightgray"
                    }
                    background: Rectangle {
                        height: closeTip.height
                        width: closeTip.width
                        border.width: 1
                        border.color: "lightgray"
                    }
                }
            }

            //右边信息
            Flickable {
                id: rightFlick
                x: 20
                y: 40
                clip: true
                interactive: false
                width: right.width - x
                height: right.height - y
                contentWidth: column.width
                contentHeight: column.height
                ScrollBar.vertical: ScrollBar {
                    parent: rightFlick //必须设置父对象才能启动hovered and pressed以及鼠标事件
                    anchors.right: rightFlick.right //绑定右边
                    orientation: Qt.Vertical
                    policy: ScrollBar.AlwaysOn // (contentHeight > container.height
                    //- 40) ? (hovered ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff) : ScrollBar.AlwaysOff
                    size: rightFlick.height / rightFlick.contentHeight
                    hoverEnabled: true
                    active: true
                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: parent.size * rightFlick.height
                        radius: width / 2
                        color: parent.hovered ? Qt.darker("#d7d7d7",
                                                          1.3) : "#d7d7d7"
                    }
                }

                ColumnLayout {
                    id: column
                    clip: true
                    spacing: 15
                    //penguin bar
                    Item {
                        width: 320
                        height: 16
                        Image {
                            source: "qrc:/images/mainInterface/userInfoPenguin.png"
                        }
                        Label {
                            id: labMyQQ
                            x: 30
                            font.pointSize: 10
                            text: "2567469480"
                        }

                        Button {
                            id: editMaterialBtn
                            x: 320 - width
                            width: 62
                            height: 16
                            onClicked: {
                                console.log("editMaterialBtn clicked")
                            }

                            background: Label {
                                width: 62
                                height: 16
                                text: "编辑资料"
                                font.pointSize: 11
                                color: editMaterialBtn.hovered ? Qt.lighter(
                                                                     "#009bdb",
                                                                     1.15) : "#009bdb"
                            }
                        }
                    }
                    //person bar
                    RowLayout {
                        spacing: 10
                        Image {
                            source: "qrc:/images/mainInterface/userInfoPerson.png"
                        }
                        Label {
                            id: labSex
                            font.pointSize: 10
                            text: "男"
                        }
                        Label {
                            id: labBirthday
                            font.pointSize: 10
                            text: "6月1日"
                        }
                        Label {
                            id: labConstellation
                            font.pointSize: 10
                            text: "巨蟹座"
                        }
                        Label {
                            id: labZodiac
                            font.pointSize: 10
                            text: "属鼠"
                        }
                    }
                    //grade bar
                    RowLayout {
                        spacing: 10
                        Image {
                            source: "qrc:/images/mainInterface/userInfoGrade.png"
                        }
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
                    }
                    //separator
                    Rectangle {
                        width: 320
                        height: 1
                        color: "lightgray"
                    }

                    //MyQQ age bar
                    Item {
                        width: 200
                        height: 20
                        Image {
                            source: "qrc:/images/mainInterface/userInfoQAge.png"
                        }
                        Label {
                            x: 35
                            font.pointSize: 10
                            text: "Q龄"
                        }
                        Label {
                            id: labQAge
                            x: 105
                            font.pointSize: 10
                            text: "0年"
                        }
                    }
                    //separator
                    Rectangle {
                        width: 320
                        height: 1
                        color: "lightgray"
                    }

                    //photo wall
                    Item {
                        width: rightFlick.width
                        height: 18
                        Label {
                            text: "照片墙"
                            font.pointSize: 12
                        }
                        Button {
                            id: loadPhotoBtn1
                            x: 320 - width
                            width: 62
                            height: 16
                            onClicked: {
                                console.log("editMaterialBtn clicked")
                            }

                            background: Label {
                                width: 62
                                height: 16
                                text: "上传照片"
                                font.pointSize: 11
                                color: loadPhotoBtn1.hovered ? Qt.lighter(
                                                                   "#009bdb",
                                                                   1.15) : "#009bdb"
                            }
                        }
                    }
                    //照片墙内容
                    Container {
                        id: wallContiner
                        width: 320
                        height: 220
                        currentIndex: 0
                        contentItem: Item {}
                        //无照片时状态
                        Rectangle {
                            id: con1
                            width: 320
                            height: 220
                            visible: true
                            Image {
                                sourceSize: Qt.size(320, 220)
                                source: "qrc:/images/mainInterface/wallBack.png"
                            }
                            Button {
                                id: loadPhotoBtn2
                                x: 71
                                y: 86
                                width: 178
                                height: 46
                                background: Rectangle {
                                    implicitHeight: 46
                                    implicitWidth: 178
                                    color: loadPhotoBtn2.hovered ? Qt.lighter(
                                                                       "#009bdb",
                                                                       1.15) : "#009bdb"
                                    Image {
                                        sourceSize: Qt.size(178, 46)
                                        source: "qrc:/images/mainInterface/loadPhoto.png"
                                    }
                                }
                            }
                        }
                        //有照片时状态
                        GridView {
                            id: cont2
                            visible: false
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
    //等级图标模型
    ListModel {
        id: gradeModel
    }
}
