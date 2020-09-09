import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.12
import "qrc:/"

Window {
    property bool getFocus: false
    property var cwin: null
    property alias model: listModel
    signal getFVerify(var obj)
    id: win
    width: 602
    height: 608
    flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
    color: "lightgray" //边界颜色
    visible: true
    title: "更改封面"

    //关闭处理
    onClosing: {
        console.log("onClosing:interface for cover")
        hide() //躲避退出
        if (cwin != null)
            cwin.close()
        qqMainWin.loaderForVerify.source = "" //释放资源
    }
    //处理验证信息之好友
    onGetFVerify: {
        console.log(" onGetFVerify")
        for (var o in obj) {
            console.log(o)
            var cobj = obj[o]
            console.log(cobj["name"], cobj["sex"], cobj["age"], cobj["city"],
                        cobj["verify"], cobj["time"])
            var time = cobj["time"].split(" ").filter(function (x) {
                return x !== ""
            })
            var temp = time[1]
            time = time[0].split("-").filter(function (x) {
                return x !== ""
            })
            time = time[0] + "年" + time[1] + "月" + time[2] + "日" + " " + temp
            var path = "file:../user/" + mainWin.myqq + "/friends/verifyFriends/" + o
            listModel.append({
                                 "r_number": o,
                                 "r_name": cobj["name"],
                                 "r_sex": cobj["sex"],
                                 "r_age": cobj["age"],
                                 "r_city": cobj["city"],
                                 "r_attach": cobj["verify"],
                                 "r_time": time,
                                 "r_imgPath": path
                             })
        }
        scrView.visible = true
    }
    //移动鼠标
    MouseCustomForWindow {
        onSendPos: {
            win.x += movedCoordinate.x
            win.y += movedCoordinate.y
        }
        onPressed: {
            startPoint.x = mouseX
            startPoint.y = mouseY
            getFocus = true
            console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
        }
        onReleased: {
            getFocus = false
        }
    }

    //躯干
    Rectangle {
        id: bodyRec
        x: 1
        y: 1
        color: "#12b7f5"
        width: 600
        height: 606

        //header
        Rectangle {
            id: header
            height: 30
            width: 600
            color: "transparent"
            Image {
                x: 6
                y: (30 - height) / 2
                sourceSize: Qt.size(16, 16)
                source: "qrc:/images/bellinfo.png"
            }
            Label {
                x: 25
                y: (30 - height) / 2
                text: "验证消息"
                font.pointSize: 9
                color: "white"
            }
            RowLayout {
                x: header.width - 58
                spacing: 0
                Button {
                    id: minBtn
                    width: 30
                    height: 30
                    onClicked: {
                        win.showMinimized()
                    }

                    background: Rectangle {
                        implicitHeight: 30
                        implicitWidth: 30
                        color: "white"
                        Image {
                            id: minImg
                            sourceSize: Qt.size(30, 30)
                            source: "qrc:/images/mainInterface/alterHeadMin.png"
                        }
                        ColorOverlay {
                            anchors.fill: minImg
                            source: minImg
                            color: minBtn.hovered ? (minBtn.pressed ? Qt.lighter(
                                                                          "#12b7f5",
                                                                          1.2) : Qt.lighter(
                                                                          "#12b7f5",
                                                                          1.4)) : "#12b7f5"
                        }
                    }
                }
                Button {
                    id: closeBtn
                    width: 28
                    height: 30
                    onClicked: {
                        close()
                    }

                    background: Rectangle {
                        implicitHeight: 30
                        implicitWidth: 28
                        color: "white"
                        Image {
                            id: closeImg
                            sourceSize: Qt.size(28, 30)
                            source: "qrc:/images/mainInterface/alterHeadClose.png"
                        }
                        ColorOverlay {
                            anchors.fill: closeImg
                            source: closeImg
                            color: closeBtn.hovered ? (closeBtn.pressed ? Qt.darker("#d44027", 1.2) : "#d44027") : "#12b7f5"
                        }
                    }
                }
            }
        }
        Label {
            y: 30
            width: 115
            height: 30
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "好友验证"
            font.pointSize: 11
            color: "white"
            background: Rectangle {
                implicitWidth: 115
                implicitHeight: 30
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: "#12b7f5"
                    }
                    GradientStop {
                        position: 1.0
                        color: "#ccf0fd"
                    }
                }
            }
        }
        //加载提示
        Rectangle {
            id: loadTip
            y: 60
            color: "#ecf5f8"
            visible: !scrView.visible
            width: bodyRec.width
            height: bodyRec.height - 95
            Label {
                anchors.centerIn: parent
                text: "正在加载中..."
                font.pointSize: 14
            }
        }
        ScrollView {
            id: scrView
            y: 60
            width: bodyRec.width
            height: bodyRec.height - 95
            visible: false
            contentWidth: header.width
            contentHeight: lview.contentHeight
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            //滚动栏
            ScrollBar.vertical: ScrollBar {
                parent: scrView //必须设置父对象才能启动hovered and pressed以及鼠标事件
                anchors.right: parent.right //绑定右边
                orientation: Qt.Vertical
                policy: (scrView.contentHeight
                         > scrView.height) ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                size: scrView.height / scrView.contentHeight
                hoverEnabled: true
                active: true
                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: parent.size * scrView.height
                    radius: width / 2
                    color: parent.hovered ? Qt.darker("#d7d7d7",
                                                      1.3) : "#d7d7d7"
                }
            }
            contentItem: ListView {
                id: lview
                model: listModel

                onCountChanged: {
                    height = count * 110
                }

                delegate: Item {
                    id: vitem
                    property string number: r_number
                    property bool okVisible: true
                    property alias igt: igText.text
                    width: scrView.width
                    height: 115
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onPressed: {
                            getFocus = true
                        }
                    }

                    Label {
                        x: 25
                        font.pointSize: 11
                        color: "gray"
                        text: r_time
                    }
                    Rectangle {
                        x: 25
                        y: 20
                        width: scrView.width - 50
                        height: 90
                        color: "#ecf5f8"
                        border.color: "lightgray"
                        MouseArea {
                            id: itemMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            onPressed: {
                                getFocus = true
                            }
                            onReleased: {
                                getFocus = false
                            }

                            onContainsMouseChanged: {
                                if (containsMouse) {
                                    crossBtn.visible = true
                                } else {
                                    crossBtn.visible = false
                                }
                            }
                        }

                        //头像
                        Button {
                            id: btnHead
                            x: 10
                            y: 19
                            width: 52
                            height: 52
                            clip: true
                           onClicked: {

                           }

                            onHoveredChanged: {
                                if (hovered) {
                                    crossBtn.visible = true
                                }
                            }
                            background: Rectangle {
                                id: recHead
                                implicitWidth: 52
                                implicitHeight: 52
                                color: "#ecf5f8"
                                radius: 90
                                clip: true
                                Image {
                                    id: imgHead
                                    asynchronous: true
                                    cache: false
                                    visible: false
                                    source: r_imgPath
                                    sourceSize: Qt.size(parent.size,
                                                        parent.size)
                                }
                                //遮罩
                                OpacityMask {
                                    width: parent.width
                                    height: parent.height
                                    source: imgHead
                                    maskSource: recHead
                                }
                            }
                        }
                        ColumnLayout {
                            x: 85
                            y: btnHead.y
                            spacing: 3
                            //昵称
                            RowLayout {
                                spacing: 3
                                Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
                                Button {
                                    id: btnName
                                    width: labName.width < 440 ? labName.width : 440
                                    height: labName.height
                                    font.underline: btnName.hovered
                                    font.pointSize: 13
                                    clip: true
                                    onHoveredChanged: {
                                        if (hovered) {
                                            crossBtn.visible = true
                                        }
                                    }
                                    background: Label {
                                        id: labName
                                        text: r_name
                                        font: btnName.font
                                    }
                                }
                                Label {
                                    text: "来自MyQQ号查找"
                                    font.pointSize: 10
                                }
                            }
                            //性别年龄城市
                            RowLayout {
                                spacing: 5
                                Label {
                                    id: labSex
                                    text: r_sex
                                    font.pointSize: 10
                                }
                                Label {
                                    id: labAge
                                    text: r_age
                                    font.pointSize: 10
                                }
                                Label {
                                    id: labCisty
                                    text: r_city
                                    font.pointSize: 10
                                }
                            }
                            //附加消息
                            RowLayout {
                                spacing: 5
                                Label {
                                    id: labAttach
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onHoveredChanged: {
                                            if (containsMouse)
                                                crossBtn.visible = true
                                        }
                                    }
                                    textFormat: Qt.RichText
                                    text: "<font style='color:gray;'>附加消息:</font>" + r_attach
                                }
                            }
                        }

                        //按钮
                        RowLayout {
                            x: 380
                            y: 30
                            spacing: 5

                            //同意
                            Rectangle {
                                visible: vitem.okVisible
                                width: 88
                                height: 24
                                radius: 3
                                border.color: "lightgray"
                                Button {
                                    id: okBtn
                                    x: 1
                                    y: 1
                                    width: 67
                                    height: 22
                                    onClicked: {
                                        if (newWin.status == Component.Ready) {
                                            var com = newWin.createObject(win)
                                            if (null === com) {
                                                console.log("component creation is of failure")
                                                return
                                            }
                                            win.cwin = com
                                            com.number = r_number //标记号码
                                            com.show()
                                            com.raise()
                                            com.requestActivate()
                                        }
                                    }
                                    onHoveredChanged: {
                                        if (hovered)
                                            crossBtn.visible = false
                                    }

                                    Connections {
                                        target: win
                                        onGetFocusChanged: {
                                            if (getFocus) {
                                                okBtn.focus = false
                                            }
                                        }
                                    }
                                    background: Rectangle {
                                        implicitHeight: 22
                                        implicitWidth: 67
                                        radius: 3
                                        border.width: 2
                                        color: okBtn.hovered ? (okBtn.pressed ? "white" : "#d2eafa") : "white"
                                        border.color: okBtn.activeFocus ? "#3cc2f4" : color
                                        Text {
                                            x: (64 - width) / 2 + 3
                                            y: okBtn.pressed ? okBtn.hovered ? 4 : 6 : 4
                                            text: qsTr("同意")
                                            font.pointSize: 9
                                        }
                                    }
                                }
                                //附属小按钮
                                Button {
                                    id: attachBtn
                                    x: 62
                                    y: 1
                                    width: 25
                                    height: 22
                                    onClicked: {
                                        menu.visible = true
                                    }
                                    onHoveredChanged: {
                                        if (hovered) {
                                            crossBtn.visible = true
                                        }
                                    }
                                    Connections {
                                        target: win
                                        onGetFocusChanged: {
                                            if (getFocus) {
                                                attachBtn.focus = false
                                            }
                                        }
                                    }
                                    Menu {
                                        id: menu
                                        x: 15
                                        y: 35
                                        width: 138
                                        height: 74
                                        topPadding: 9
                                        MenuItem {
                                            x: 1
                                            width: 136
                                            height: 28
                                            text: qsTr("同意")
                                            onTriggered: okBtn.clicked()
                                            background: Rectangle {
                                                implicitHeight: 28
                                                implicitWidth: 136
                                                MouseArea {
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    onPressed: mouse.accepted = false
                                                    onHoveredChanged: {
                                                        if (containsMouse) {
                                                            parent.color = Qt.lighter(
                                                                        "lightgray",
                                                                        1.15)
                                                        } else {
                                                            parent.color = "transparent"
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        MenuItem {
                                            x: 1
                                            width: 136
                                            height: 28
                                            text: qsTr("拒绝")

                                            onTriggered: {
                                                igText.text = "已拒绝"
                                                igBtn.clicked()
                                            }
                                            background: Rectangle {
                                                implicitHeight: 28
                                                implicitWidth: 136
                                                MouseArea {
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    onPressed: mouse.accepted = false
                                                    onHoveredChanged: {
                                                        if (containsMouse) {
                                                            parent.color = Qt.lighter(
                                                                        "lightgray",
                                                                        1.15)
                                                        } else {
                                                            parent.color = "transparent"
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        background: Rectangle {
                                            implicitHeight: 74
                                            implicitWidth: 138
                                            radius: 3
                                            border.color: "lightgray"
                                        }
                                    }

                                    background: Rectangle {
                                        radius: 3
                                        implicitHeight: 22
                                        implicitWidth: 22
                                        border.width: 2
                                        color: attachBtn.hovered ? (attachBtn.pressed ? "white" : "#d2eafa") : "white"
                                        border.color: attachBtn.activeFocus ? "#3cc2f4" : color
                                        Text {
                                            padding: 0
                                            x: (22 - width) / 2 + 3
                                            y: attachBtn.hovered ? attachBtn.pressed ? 7 : 6 : 7
                                            text: qsTr("▼")
                                            font.pointSize: 7
                                        }
                                    }
                                }
                                //按钮小遮罩 掩饰边角
                                Rectangle {
                                    id: leftMRec
                                    x: 62
                                    y: 1
                                    width: 3
                                    height: 22
                                    visible: (okBtn.pressed || okBtn.hovered)
                                    color: "white"
                                    Rectangle {
                                        x: 0
                                        y: 3
                                        width: 1
                                        height: 16
                                        z: 1
                                        color: "lightgray"
                                    }
                                }
                                Rectangle {
                                    id: rightMRec
                                    x: 62
                                    y: 1
                                    visible: !leftMRec.visible
                                    width: 3
                                    height: 22
                                    color: "white"
                                    Rectangle {
                                        x: 2
                                        y: 3
                                        width: 1
                                        height: 16
                                        z: 1
                                        color: "lightgray"
                                    }
                                }
                            }
                            //忽略按钮
                            Button {
                                id: igBtn
                                width: 70
                                height: 24
                                //决绝处理
                                onClicked: {
                                    var obj = {}
                                    obj.myqq = qqMainWin.myqq
                                    obj.number = r_number //对方号码
                                    if (igText.text == "已拒绝") {
                                        console.log("igBtn attemp to refuse")
                                        obj.handle = "refuse"
                                    } else {
                                        console.log("igBtn attemp to ignore")
                                        igText.text = "已忽略"
                                        obj.handle = "ignore"
                                    }
                                    funcc.handleFVerify(obj)
                                }

                                onHoveredChanged: {
                                    if (hovered) {
                                        crossBtn.visible = true
                                    }
                                }
                                Connections {
                                    target: win
                                    onGetFocusChanged: {
                                        if (getFocus) {
                                            igBtn.focus = false
                                        }
                                    }
                                }
                                background: Rectangle {
                                    implicitWidth: 70
                                    implicitHeight: 24
                                    radius: 3
                                    color: igBtn.hovered ? igBtn.pressed ? "white" : "#d2eafa" : "white"
                                    border.color: igBtn.activeFocus ? "#3cc2f4" : color
                                    Text {
                                        id: igText
                                        anchors.centerIn: parent
                                        text: qsTr("忽略")
                                        font.pointSize: 9
                                        onTextChanged: {
                                            if (text != qsTr("忽略")) {
                                                parent.color = "transparent"
                                                parent.border.color = "transparent"
                                                color = "gray"
                                                vitem.okVisible = false
                                                igBtn.enabled = false
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //叉叉按钮 a5abad
                        Button {
                            id: crossBtn
                            x: pressed ? parent.width - width - 1 : parent.width - width - 2
                            y: pressed ? 2 : 1
                            width: 9
                            height: 9
                            visible: false
                            onClicked: {
                                console.log("cross")
                                if (igText.text == "忽略") {
                                    var obj = {}
                                    obj.myqq = qqMainWin.myqq
                                    obj.number = r_number //对方号码
                                    console.log()
                                    console.log("igBtn attemp to close")
                                    obj.handle = "close"
                                    funcc.handleFVerify(obj)
                                }
                                var length = listModel.count
                                for (var i = 0; i < length; ++i) {
                                    if (listModel.get(
                                                i).r_number == vitem.number) {
                                        listModel.remove(i)
                                    }
                                }
                            }

                            Connections {
                                target: win
                                onGetFocusChanged: {
                                    if (getFocus) {
                                        crossBtn.focus = false
                                    }
                                }
                            }

                            background: Image {
                                id: crossImg
                                sourceSize: Qt.size(9, 9)
                                source: "qrc:/images/mainInterface/verifyCross.png"
                            }
                            ColorOverlay {
                                anchors.fill: crossImg
                                source: crossImg
                                color: crossImg.hovered ? Qt.darker(
                                                              "#a5abad",
                                                              1.15) : "#a5abad"
                            }
                        }
                    }
                }
            }
            background: Rectangle {
                implicitWidth: bodyRec.width
                implicitHeight: bodyRec.height - 95
                color: "#ecf5f8"
                MouseCustomForWindow {
                    onSendPos: {
                        win.x += movedCoordinate.x
                        win.y += movedCoordinate.y
                    }
                    onPressed: {
                        startPoint.x = mouseX
                        startPoint.y = mouseY
                        getFocus = true
                        console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
                    }
                    onReleased: {
                        getFocus = false
                    }
                }
            }
        }
        //底部框
        Frame {
            y: bodyRec.height - height
            width: header.width
            height: 35
            //移动鼠标
            MouseCustomForWindow {
                onSendPos: {
                    win.x += movedCoordinate.x
                    win.y += movedCoordinate.y
                }
                onPressed: {
                    startPoint.x = mouseX
                    startPoint.y = mouseY
                    getFocus = true
                    console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
                }
                onReleased: {
                    getFocus = false
                }
            }
            background: Rectangle {
                implicitHeight: parent.height
                implicitWidth: parent.height
                color: "#d1e5f2"
            }
        }
    }
    //部件 添加分组与同意添加功能
    Component {
        id: newWin
        Window {
            property string number: "-1"
            signal addFGroup(string name)
            id: cwin
            width: 264
            height: 166
            flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
            onClosing: {
                console.log("closed newWin")
                hide()
                funcc.emitCloseAddFGroup()
                cwin.destroy()
            }
            onAddFGroup: {
                groupModel.append({
                                      "modelData": name
                                  })
                groupCBox.currentIndex = groupModel.count - 1
            }

            MouseCustomForWindow {
                onSendPos: {
                    cwin.x += movedCoordinate.x
                    cwin.y += movedCoordinate.y
                }
            }
            Rectangle {
                x: 1
                y: 1
                border.color: "lightgray"
                width: 262
                height: 164
                MouseCustomForWindow {
                    onSendPos: {
                        cwin.x += movedCoordinate.x
                        cwin.y += movedCoordinate.y
                    }
                }
                //header
                Rectangle {
                    id: header
                    width: 262
                    height: 30
                    Image {
                        id: headerImg
                        sourceSize: Qt.size(384, 30)
                        source: "qrc:/images/mainInterface/verifyNewHeader.png"
                    }

                    Button {
                        id: closeBtn
                        x: parent.width - width
                        width: 28
                        height: 30
                        onClicked: {
                            close()
                        }

                        background: Image {
                            id: closeImg
                            sourceSize: Qt.size(28, 30)
                            source: "qrc:/images/mainInterface/alterHeadClose.png"
                        }
                        ColorOverlay {
                            anchors.fill: closeImg
                            source: closeImg
                            color: closeBtn.hovered ? (closeBtn.pressed ? Qt.darker("#d44027", 1.2) : "#d44027") : "#12b7f5"
                        }
                    }
                }
                GridLayout {
                    x: 20
                    y: 50
                    columns: 2
                    columnSpacing: 5
                    rowSpacing: 5
                    Label {
                        width: 65
                        height: 20
                        clip: true
                        font.pointSize: 10
                        text: "备注姓名："
                    }
                    //备注
                    TextField {
                        id: fieldTag
                        font.family: "新宋体"
                        background: Rectangle {
                            implicitWidth: 150
                            implicitHeight: 22
                            radius: 3
                            color: "white"
                            border.color: fieldTag.hovered ? "#1583dd" : "lightgray"
                            border.width: fieldTag.hovered ? 2 : 1
                        }
                    }
                    Label {
                        width: 65
                        height: 20
                        clip: true
                        font.pointSize: 10
                        text: "分   组："
                    }
                    //分组选择
                    Row {
                        spacing: 3
                        ComboBox {
                            id: groupCBox
                            font.family: "新宋体"
                            font.pixelSize: 10
                            currentIndex: -1
                            editable: false
                            onCurrentTextChanged: {
                                okBtn.enabled = true
                            }
                            contentItem: Label {
                                leftPadding: 10
                                topPadding: 3
                                text: groupCBox.displayText
                                font: groupCBox.font
                                color: "black"
                            }

                            model: groupModel

                            delegate: ItemDelegate {
                                id: proDet
                                width: 118
                                height: 22

                                background: Rectangle {
                                    color: proDet.highlighted ? "#378fcf" : "#ffffff"
                                    border.color: "white"
                                    Text {
                                        y: (parent.height - height) / 2
                                        x: 8
                                        text: modelData
                                        color: proDet.highlighted ? "white" : "black"
                                        font: groupCBox.font
                                        elide: Text.ElideRight
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                highlighted: groupCBox.highlightedIndex === index
                            }

                            indicator: Canvas {
                                id: proCanvas
                                x: groupCBox.width - width - groupCBox.rightPadding
                                y: groupCBox.topPadding + (groupCBox.availableHeight - height) / 2
                                width: 10
                                height: 10
                                contextType: "2d"

                                Connections {
                                    target: professPop
                                    onVisibleChanged: {
                                        proCanvas.requestPaint()
                                    }
                                }

                                onPaint: {
                                    context.reset()
                                    if (!professPop.visible) {
                                        context.moveTo(0, height / 4)
                                        context.lineTo(width, height / 4)
                                        context.lineTo(width / 2,
                                                       height / 4 * 3)
                                        context.closePath()
                                    } else {
                                        context.moveTo(width / 2, height / 4)
                                        context.lineTo(0, height / 4 * 3)
                                        context.lineTo(width, height / 4 * 3)
                                    }
                                    context.closePath()
                                    context.fillStyle = "#434343"
                                    context.fill()
                                }
                            }

                            background: Rectangle {
                                implicitWidth: 120
                                implicitHeight: 22
                                border.color: groupCBox.hovered ? "#1583dd" : "lightgray"
                                border.width: groupCBox.hovered ? 2 : 1
                                radius: 3
                            }

                            popup: Popup {
                                id: professPop
                                y: groupCBox.height + 2

                                width: 120
                                height: 68
                                padding: 2
                                clip: true

                                Flickable {
                                    id: flick

                                    width: 120
                                    height: 68
                                    interactive: false
                                    contentWidth: popLView.width
                                    contentHeight: popLView.height
                                    ScrollBar.vertical: ScrollBar {
                                        parent: flick.parent //必须设置父对象才能启动hovered and pressed以及鼠标事件
                                        anchors.right: parent.right //绑定右边
                                        orientation: Qt.Vertical
                                        policy: (flick.contentHeight > flick.height) ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                                        size: flick.height / flick.contentHeight
                                        hoverEnabled: true
                                        active: true
                                        contentItem: Rectangle {
                                            implicitWidth: 6
                                            implicitHeight: parent.size * flick.height
                                            radius: width / 2
                                            color: parent.hovered ? Qt.darker(
                                                                        "#d7d7d7",
                                                                        1.3) : "#d7d7d7"
                                        }
                                    }
                                    ListView {
                                        id: popLView
                                        clip: true
                                        height: groupCBox.delegateModel.count * 22
                                        width: 120
                                        model: groupCBox.delegateModel
                                        currentIndex: groupCBox.highlightedIndex
                                        delegate: groupCBox.delegate
                                    }
                                }
                                background: Rectangle {
                                    implicitWidth: 120
                                    implicitHeight: 68
                                    color: "white"
                                    border.color: "#1583dd"
                                    border.width: 2
                                    radius: 2
                                }
                            }
                            Component.onCompleted: {

                                var length = groupModel.count
                                for (var i = 0; i < length; ++i) {
                                    if (groupModel.get(i).modelData == "我的好友")
                                        groupCBox.currentIndex = i
                                }
                            }
                        }
                        Button {
                            id: newBtn
                            width: 28
                            height: 22
                            //打开添加好友组对话框
                            onClicked: {
                                if (qqMainWin.friendGroupModel.count() > 15) {
                                    console.log("the number of friend group is not more than fifteen")
                                    return
                                }
                                funcc.openAddFGroupWidget(cwin, qqMainWin)
                            }
                            background: Rectangle {
                                implicitWidth: 28
                                implicitHeight: 22
                                color: "white"
                                Text {
                                    anchors.centerIn: parent
                                    text: "新建"
                                    font: newBtn.font
                                    color: newBtn.down ? "black" : "#1583dd"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    elide: Text.ElideRight
                                }
                            }
                        }
                    }
                }
                //底部
                Frame {
                    id: bottomFrame
                    padding: 6
                    x: 1
                    y: cwin.height - 37
                    MouseCustomForWindow {
                        onSendPos: {
                            cwin.x += movedCoordinate.x
                            cwin.y += movedCoordinate.y
                        }
                    }
                    background: Rectangle {
                        implicitHeight: 35
                        implicitWidth: cwin.width - 2
                        color: "#e5eff7"
                    }

                    Button {
                        id: okBtn
                        width: 72
                        height: 22
                        x: parent.width - width - 15
                        y: (parent.height - height) / 2
                        text: "确定"
                        onClicked: {
                            var index = groupCBox.currentIndex
                            if (index == -1) {
                                console.log("divided friend groups index is negative")
                                return
                            }
                            index += 1

                            var obj = {}
                            obj.groupName = groupCBox.currentText
                            obj.myqq = qqMainWin.myqq
                            obj.handle = "add"
                            obj.tag = fieldTag.text

                            obj.number = cwin.number //对方号码
                            var items = lview.contentItem.children
                            var length = listModel.count
                            funcc.handleFVerify(obj) //推送远程
                            for (var i = 0; i < length; ++i) {
                                var item = items[i]
                                if (item.number == cwin.number) {
                                    item.igt = "已同意"
                                    break
                                }
                            }

                            close()
                        }

                        background: Rectangle {
                            implicitWidth: 72
                            implicitHeight: 22
                            color: okBtn.hovered ? (okBtn.pressed ? Qt.darker(
                                                                        "#12b7f5",
                                                                        1.25) : Qt.darker(
                                                                        "#12b7f5",
                                                                        1.10)) : "#12b7f5"
                        }
                    }
                }
            }
        }
    }

    //模型
    //验证消息列表
    ListModel {
        id: listModel
    }
    ListModel {
        id: groupModel
        Component.onCompleted: {
            console.log(" groupModel")
            initGroup()
        }
    }
    //函数
    //更新好友组模型
    function initGroup() {
        groupModel.clear()
        var qm = qqMainWin.friendGroupModel
        var length = qm.count()
        console.log(length)
        for (var i = 1; i < length; ++i) {
            console.log(qm.data(i))
            groupModel.append({
                                  "modelData": qm.data(i)
                              })
        }
    }
    Component.onCompleted: {
        funcc.setWin(win)
        funcc.setSourceIco(":/images/bellinfo.png")
    }
}
