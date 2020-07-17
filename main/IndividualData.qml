﻿import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import "../"
import "../chinese-lunar.js" as CLunar

//用户个人资料
Window {
    property alias loaderForAlterCover: loaderForAlterCover
    id: indivadualWin
    width: 722
    height: 522
    visible: true //显示图标
    title: "我的资料"
    flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
    color: "lightgray" //边界颜色
    //更改封面完成处理
    Connections {
        target: loaderForAlterCover.item
        onUpdateCover: {
            console.log("updateCover() function has been called")
            var path = "../user/" + funcc.myQQ + "/cover"
            leftImg.source = "" //刷新
            leftImg.source = "file:" + path
            funcc.updateCover(path)
            console.log("updated Cover")
        }
    }
    //获取my个人数据
    Connections {
        target: funcc
        //(QVariantMap obj)
        onEmitPersonalJsonInfo: {


            /*writeObj.insert("birthday",query.value("birthday").toString());
            writeObj.insert("registerDateTime",query.value("registerDateTime").toString());
            writeObj.insert("bloodGroup",query.value("bloodGroup").toString());
            writeObj.insert("education",query.value("education").toString());
            writeObj.insert("profession",query.value("profession").toString());
            writeObj.insert("corporation",query.value("corporation").toString());
            writeObj.insert("location1",query.value("location1").toString());
            writeObj.insert("location2",query.value("location2").toString());
            writeObj.insert("location3",query.value("location3").toString());
            writeObj.insert("location4",query.value("location4").toString());
            writeObj.insert("home1",query.value("home1").toString());
            writeObj.insert("home2",query.value("home2").toString());
            writeObj.insert("home3",query.value("home3").toString());
            writeObj.insert("home4",query.value("home4").toString());
            writeObj.insert("personalStatement",query.value("personalStatement").toString());
            writeObj.insert("phone",query.value("phone").toString());*/
            console.log("onEmitPersonalJsonInfo", obj)
            var birarr = obj["birthday"].split("-")
            if (birarr.length < 3)
                console.log("birthday format is not normal")
            var age = func.getAge(birarr)
            labMyQQ.text = qqMainWin.myqq //MyQQ
            labSex.text = qqMainWin.sex
            labAge.text = age
            labBirthday.text = birarr[1] + "月" + birarr[2] + "日(公历)"
            labConstellation.text = func.getConstellation(birarr)
            var f = CLunar._chineseLunar
            var clyear = f.solarToLunar(new Date(birarr[0], birarr[1],
                                                 birarr[2])) //农历对象
            clyear = clyear["year"] //农历年
            var zodiac = f.animalName(clyear)
            labZodiac.text = zodiac //生肖
        }
        //QVector<QString> names
        onEmitPersonalCoverAndPhoto: {
            console.log(" onEmitPersonalCoverAndPhoto")
            var length = names.length
            for (var i = 0; i < length; ++i) {
                if (names[i] === "cover") {
                    leftImg.source = "file:../user/" + mainWin.myqq + "/cover"
                }
            }
        }
    }

    //关闭处理
    onClosing: {
        console.log("invidualData onClosing")
        indivadualWin.opacity = 0.0 //躲藏 不释放资源
        close.accepted = false //拒绝close
        indivadualWin.flags = Qt.FramelessWindowHint | Qt.Widget
    }
    //露出资料按钮
    //移动鼠标
    MouseCustomForWindow {
        onSendPos: {
            indivadualWin.x += movedCoordinate.x
            indivadualWin.y += movedCoordinate.y
        }
    }
    //躯干
    Rectangle {
        id: body
        x: 1
        y: 1
        width: indivadualWin.width - 2
        height: indivadualWin.height - 2
        //左边封面
        Rectangle {
            id: leftRec
            width: 358
            height: 358
            Image {
                id: leftImg
                anchors.fill: parent
                cache: false
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
                    onClicked: {
                        fDlog.open()
                    }
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
                x: renewCover.x + renewCover.xTip
                y: renewCover.x + renewCover.yTip + 25
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
            height: indivadualWin.height - 2 - leftRec.height
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
                            actions.openAlterHImgAct.trigger()
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
                    indivadualWin.showMinimized()
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
                    indivadualWin.close()
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
                                font.family: "宋体"
                                font.pointSize: 10
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
                            id: labAge
                            font.pointSize: 10
                            text: "22岁"
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
                        height: gridLay.height
                        Layout.alignment: Qt.AlignTop | Qt.AlignLeft //顶部对其，多行等级图标显示
                        //等级指示图标
                        Image {

                            source: "qrc:/images/mainInterface/userInfoGrade.png"
                        }
                        //等级图标
                        GridLayout {
                            property int posx: 0
                            property int posy: 0
                            id: gridLay
                            columns: 10
                            rowSpacing: 3
                            Repeater {
                                id: gradeRep
                                model: qqMainWin.gradeModel
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
                                y: gridLay.posy + 20
                                visible: false
                                // timeout: 1000
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
                            //等级区域鼠标
                            MouseArea {
                                id: gridMouse
                                x: gridLay.x
                                y: gridLay.y
                                width: gridLay.width //占用一行
                                height: gridLay.height
                                hoverEnabled: true
                                onContainsMouseChanged: {
                                    if (containsMouse) {
                                        //归零
                                        gradeimgTip.visible = false //刷新
                                        gradeimgTip.timeout = -1
                                        gradeimgTip.delay = 1000
                                        gradeimgTip.visible = true
                                    } else {
                                        //立即显示 超时1秒关闭
                                        gradeimgTip.timeout = 1000
                                        gradeimgTip.delay = 0
                                        gradeimgTip.visible = false //刷新状态
                                        gradeimgTip.show(
                                                    gradeimgTip.text) //显示超时一秒
                                    }
                                }

                                onPositionChanged: {
                                    if (!gradeimgTip.visible) {
                                        gridLay.posx = mouseX
                                        gridLay.posy = mouseY
                                    }
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

                    //MyQQ information set
                    Item {
                        width: 200
                        height: infoColumn.height
                        Image {
                            source: "qrc:/images/mainInterface/userInfoQAge.png"
                        }
                        ColumnLayout {
                            id: infoColumn
                            x: 30
                            spacing: 15
                            InfoSmallLabel {
                                id: whereInfo
                                preLabel.text: "所在地"
                                aftLabel.text: "北京 东城"
                            }
                            InfoSmallLabel {
                                id: phoneInfo
                                preLabel.text: "手机"
                                aftLabel.text: "15334184810"
                            }
                            InfoSmallLabel {
                                id: qAgeInfo
                                preLabel.text: "Q龄"
                                aftLabel.text: "0年"
                            }
                            InfoSmallLabel {
                                id: homeInfo
                                preLabel.text: "家乡"
                                aftLabel.text: "阿富汗 卡萨丁"
                            }
                            InfoSmallLabel {
                                id: professInfo
                                preLabel.text: "职业"
                                aftLabel.text: "生产/工艺/制造"
                            }
                            InfoSmallLabel {
                                id: corporationInfo
                                preLabel.text: "公司"
                                aftLabel.text: "天宝有限公司"
                            }
                            InfoSmallLabel {
                                id: educationInfo
                                preLabel.text: "教育经历"
                                aftLabel.text: "南开大学\n2019级 硕士"
                            }
                            InfoSmallLabel {
                                id: statementInfo
                                preLabel.text: "个人说明"
                                aftLabel.text: "天空如此栏 十大山东快书是的撒"
                            }
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
                                font.pointSize: 10
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
    //界面
    //更改封面
    Loader {
        id: loaderForAlterCover
        visible: loaderForAlterCover.status == Loader.Ready
    }

    //文件打开框，不要用Qtc++的静态调用文件对话框，Qtc++的静态调用文件对话框不会自动释放资源，应该是里面有指针实现部分，我用这个可是异常延迟一天爆发
    FileDialog {
        id: fDlog
        title: "打开"
        fileMode: FileDialog.OpenFile
        defaultSuffix: "../"
        nameFilters: ["图像文件(*.jpeg;*.jpg;*.png)"]
        onAccepted: {
            console.log("You chose: " + fDlog.file)
            actions.alterCoverAct.trigger(fDlog)
        }
        onRejected: {
            console.log("Canceled")
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
