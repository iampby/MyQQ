import QtQuick 2.12
import QtQuick.Window 2.11
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtGraphicalEffects 1.12
import "qrc:/"
import QtLocation 5.12

//聊天界面
Window {
    property var obj: null

    //用于鼠标拉伸
    property int enableSize: 6
    property bool isPressed: false
    property point customPoint: Qt.point(0, 0)
    id: chatWin
    width: 588
    height: 506
    minimumHeight: 506
    minimumWidth: 588
    color: "#00000000"
    flags: Qt.FramelessWindowHint | Qt.Window
    onClosing: {
        console.log("chatWin  onClosing")
        hide()
        try {
            var number = obj.number
            qqMainWin.mapChat.delete(number) //删除记录
            destroy() //释放资源
        } catch (e) {
            console.log("warning:", e.message)
        }
    }
    //数据更新
    onObjChanged: {
        if (!obj)
            return
        try {
            var name = obj.name
            var tag = obj.tag
            if (name == undefined)
                return
            if (tag == "" || tag == undefined) {
                title = name
            } else
                title = tag
            var img = obj.headImg
            img = img.substring(img.lastIndexOf("/") + 1, img.length)
            funcc.setSourceIco(chatWin, img)
        } catch (e) {
            console.log("warning:", e.message)
        }
    }
    //函数
    //拖拽调整大小 回调函数
    function sizeChanged(w, h, delta, directX, directY) {
        if (w < chatWin.minimumWidth) {
            if (directX < 0)
                chatWin.x += (chatWin.width - chatWin.minimumWidth)
            chatWin.width = chatWin.minimumWidth
        } else if (w > chatWin.maximumWidth) {
            if (directX < 0)
                chatWin.x += (chatWin.width - chatWin.maximumWidth)
            chatWin.width = chatWin.maximumWidth
        } else {
            chatWin.width = w
            if (directX < 0)
                chatWin.x += delta.x
        }

        if (h < chatWin.minimumHeight) {
            if (directY < 0)
                chatWin.y += (chatWin.height - chatWin.minimumHeight)
            chatWin.height = chatWin.minimumHeight
        } else if (h > chatWin.maximumHeight) {
            if (directY < 0)
                chatWin.y += (chatWin.height - chatWin.maximumHeight)
            chatWin.height = chatWin.maximumHeight
        } else {
            chatWin.height = h
            if (directY < 0)
                chatWin.y += delta.y
        }
    }

    //躯干
    Rectangle {
        id: bodyRec
        x: enableSize - border.width
        y: enableSize - border.width
        width: parent.width - 2 * enableSize + border.width * 2
        height: parent.height - 2 * enableSize + border.width * 2
        border.width: 2
        border.color: "lightgray"
        //键盘事件
        Keys.onPressed: {
            var key = event.key
            //关闭
            if (key === Qt.Key_C) {
                //enter
                if (event.modifiers & Qt.AltModifier) {
                    cBtn.clicked()
                }
            } else if (key === Qt.Key_S) {
                //enter
                if (event.modifiers & Qt.AltModifier) {
                    sendBtn.clicked()
                }
            }
        }
        //标题栏
        Rectangle {
            id: headerRec
            x: bodyRec.border.width
            y: bodyRec.border.width
            width: bodyRec.width - bodyRec.border.width * 2
            height: 39
            //移动
            MouseCustomForWindow {
                onSendPos: {

                    try {
                        chatWin.x += movedCoordinate.x
                        chatWin.y += movedCoordinate.y
                    } catch (e) {
                        console.log(e)
                    }
                }
            }
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0.0
                    color: "#4b8dff"
                }
                GradientStop {
                    position: 0.5
                    color: "#2eb9fd"
                }
                GradientStop {
                    position: (headerRec.width - 95) / headerRec.width
                    color: "#23cbfe"
                }
                GradientStop {
                    position: 1.0
                    color: "#23cbfe"
                }
            }

            Label {
                anchors.centerIn: parent
                text: title
                Component.onCompleted: {
                    console.log(text, "???")
                }
            }

            RowLayout {
                x: parent.width - 95
                spacing: 0
                Button {
                    onClicked: chatWin.showMinimized()
                    background: Rectangle {
                        implicitWidth: 32
                        implicitHeight: 33
                        color: "white"
                        Image {
                            id: imgMin
                            sourceSize: Qt.size(32, 33)
                            source: "qrc:/images/mainInterface/chatMin"
                        }
                    }
                    ColorOverlay {
                        anchors.fill: parent
                        source: imgMin
                        color: parent.hovered ? parent.pressed ? Qt.lighter(
                                                                     "#50d7fe",
                                                                     1.15) : "#50d7fe" : "#23cbfe"
                    }
                }
                Button {
                    property int status: 0
                    onClicked: {
                        if (status == 0) {
                            chatWin.showFullScreen()
                            status = 1
                            imgMax.source = "qrc:/images/mainInterface/chatMax1"
                        } else {
                            chatWin.showNormal()
                            status = 0
                            imgMax.source = "qrc:/images/mainInterface/chatMax"
                        }
                    }
                    background: Rectangle {
                        implicitWidth: 31
                        implicitHeight: 33
                        color: "white"
                        Image {
                            id: imgMax
                            sourceSize: Qt.size(31, 33)
                            source: "qrc:/images/mainInterface/chatMax"
                        }
                    }
                    ColorOverlay {
                        anchors.fill: parent
                        source: imgMax
                        color: parent.hovered ? parent.pressed ? Qt.lighter(
                                                                     "#50d7fe",
                                                                     1.15) : "#50d7fe" : "#23cbfe"
                    }
                }
                Button {
                    onClicked: chatWin.close()
                    background: Rectangle {
                        implicitWidth: 32
                        implicitHeight: 33
                        color: "white"
                        Image {
                            id: imgClose
                            sourceSize: Qt.size(32, 33)
                            source: "qrc:/images/mainInterface/chatClose"
                        }
                    }
                    ColorOverlay {
                        anchors.fill: parent
                        source: imgClose
                        color: parent.hovered ? parent.pressed ? Qt.darker(
                                                                     "#ff5439",
                                                                     1.15) : "#ff5439" : "#23cbfe"
                    }
                }
            }
        }
        ScrollView {
            id: chatView
            anchors.top: headerRec.bottom
            width: headerRec.width
            height: bodyRec.height - bodyRec.border.width - headRec.height - dragItem.y
            //滚动栏
            ScrollBar.vertical: ScrollBar {
                property bool isInint: true
                id: chatScroll
                padding: 0
                parent: chatView //必须设置父对象才能启动hovered and pressed以及鼠标事件
                anchors.right: parent.right //绑定右边
                // height: size*chatView.height
                orientation: Qt.Vertical
                policy: ScrollBar.AlwaysOn
                size: chatView.contentHeight
                      > chatView.height ? 1.0 - (chatView.contentHeight - chatView.height)
                                          / chatView.height : 1.0
                minimumSize: 0.2
                hoverEnabled: true
                active: true
                z: 1

                contentItem: Rectangle {
                    property int th: parent.size * chatView.height
                    implicitWidth: 6
                    implicitHeight: th > 30 ? th : 30
                    radius: width / 2
                    color: parent.hovered ? Qt.darker("#d7d7d7",
                                                      1.3) : "#d7d7d7"
                }
            }
            contentItem: ListView {}
        }
        Item {
            id: dragItem
            x: bodyRec.border.width
            y: bodyRec.height - bodyRec.border.width - 120
            height: 3
            width: headerRec.width
            Rectangle {
                height: 1
                width: parent.width
                color: "lightgray"
            }
            MouseArea {
                id: dmouse
                anchors.fill: parent
                drag.smoothed: false
                drag.target: dragItem
                drag.axis: Drag.YAxis
                drag.minimumY: bodyRec.height - 333
                drag.maximumY: bodyRec.height - 119
                cursorShape: Qt.SizeVerCursor
            }
        }
        Rectangle {
            x: bodyRec.border.width
            y: dragItem.y + dragItem.height
            width: headerRec.width
            height: bodyRec.height - bodyRec.border.width - y - footer.height
            ToolBar {
                id: toolBar
                topPadding: 6
                bottomPadding: 6
                background: Rectangle {
                    implicitHeight: 40
                    color: "white"
                }

                RowLayout {
                    spacing: 0
                    ToolButton {
                        background: Rectangle {
                            implicitHeight: 28
                            implicitWidth: 28
                            color: parent.hovered ? parent.pressed ? Qt.darker(
                                                                         "#f1f1f1",
                                                                         1.15) : "#f1f1f1" : "transparent"
                            Image {
                                asynchronous: true
                                sourceSize: Qt.size(28, 28)
                                source: "qrc:/images/mainInterface/scissor.png"
                            }
                        }
                    }
                    ToolButton {

                        background: Rectangle {
                            implicitHeight: 28
                            implicitWidth: 28
                            color: parent.hovered ? parent.pressed ? Qt.darker(
                                                                         "#f1f1f1",
                                                                         1.15) : "#f1f1f1" : "transparent"
                            Image {
                                asynchronous: true
                                sourceSize: Qt.size(28, 28)
                                source: "qrc:/images/mainInterface/folder.png"
                            }
                        }
                    }
                    ToolButton {
                        background: Rectangle {
                            implicitHeight: 28
                            implicitWidth: 28
                            color: parent.hovered ? parent.pressed ? Qt.darker(
                                                                         "#f1f1f1",
                                                                         1.15) : "#f1f1f1" : "transparent"
                            Image {
                                asynchronous: true
                                sourceSize: Qt.size(28, 28)
                                source: "qrc:/images/mainInterface/sendPix.png"
                            }
                        }
                    }
                }
            }
            //编辑框
            Flickable {
                id: editFlick
                anchors.top: toolBar.bottom
                clip: true
                interactive: true
                width: headerRec.width
                height: parent.height - toolBar.height
                contentWidth: edit.width
                contentHeight: edit.height
                function ensureVisible(r) {
                    if (contentX >= r.x)
                        contentX = r.x
                    else if (contentX + width <= r.x + r.width)
                        contentX = r.x + r.width - width
                    if (contentY >= r.y)
                        contentY = r.y
                    else if (contentY + height <= r.y + r.height)
                        contentY = r.y + r.height - height
                }
                ScrollBar.vertical: ScrollBar {
                    rightPadding: 0
                    parent: editFlick //必须设置父对象才能启动hovered and pressed以及鼠标事件
                    anchors.right: editFlick.right //绑定右边
                    orientation: Qt.Vertical
                    visible: true // editFlick.contentHeight > editFlick.height ? true : false
                    minimumSize: 0.2
                    active: true
                }
                TextEdit {
                    id: edit
                    width: headerRec.width
                    height: contentHeight
                    clip: true
                    wrapMode: TextEdit.WordWrap
                    textFormat: TextEdit.RichText
                    color: "red"
                    onHeightChanged: {
                        console.log("h=", height, contentHeight)
                    }
                    onCursorRectangleChanged: editFlick.ensureVisible(
                                                  cursorRectangle)
                    Keys.onPressed: {

                        var key = event.key
                        if (key === Qt.Key_Return || key === Qt.Key_Enter) {
                            //enter
                            if (event.modifiers & Qt.ControlModifier) {
                                if (vmenu.pos == 0) {

                                } else {
                                    sendBtn.clicked()
                                }
                                //ctrl enter
                            } else if (event.modifiers & Qt.NoModifier) {
                                if (vmenu.pos == 0) {
                                    sendBtn.clicked()
                                } else {

                                }
                            }
                            event.accpeted = true //不传播到父对象控件
                            return
                        }
                        event.accpeted = true
                        return
                    }
                }
            }
        }
        //底部按钮
        Frame {
            id: footer
            x: bodyRec.border.width
            y: bodyRec.height - bodyRec.border.width - height
            padding: 0
            height: 40
            width: headerRec.width

            //移动
            MouseCustomForWindow {
                onSendPos: {
                    try {
                        chatWin.x += movedCoordinate.x
                        chatWin.y += movedCoordinate.y
                    } catch (e) {
                        console.log(e)
                    }
                }
            }
            RowLayout {
                x: parent.width - 190
                y: 0
                spacing: 15 //12b7f5 47c8f8
                ToolButton {
                    id: cBtn
                    onClicked: close()

                    Text {
                        anchors.centerIn: parent
                        color: "black"
                        textFormat: Text.RichText
                        font: parent.font
                        text: qsTr("关闭(<font style='text-decoration:underline;'>C</font>)")
                    }
                    background: Rectangle {
                        implicitWidth: 70
                        implicitHeight: 25
                        color: parent.hovered ? parent.pressed ? Qt.darker(
                                                                     "#efeff0",
                                                                     1.15) : "#efeff0" : "transparent"
                        border.color: "lightgray"
                    }
                }
                Rectangle {
                    width: 88
                    height: 25
                    radius: 3
                    clip: true
                    ToolButton {
                        id: sendBtn
                        onHoveredChanged: {
                            if (hovered) {
                                if (vmenu.pos == 0)
                                    sendTip.show("按Enter键发送消息,按Ctrl+Enter键换行")
                                else {
                                    sendTip.show("按Ctrl+Enter键发送消息,按Enter键换行")
                                }
                            } else {
                                sendTip.hide()
                            }
                        }
                        Text {
                            anchors.centerIn: parent
                            textFormat: Text.RichText
                            font: parent.font
                            color: "white"
                            text: qsTr("发送(<font style='text-decoration:underline;'>S</font>)")
                        }
                        ToolTip {
                            id: sendTip
                            delay: 500
                            background: Rectangle {
                                border.color: "lightgray"
                                width: parent.width
                                height: parent.height
                            }
                        }
                        background: Rectangle {
                            implicitWidth: parent.hovered ? parent.pressed ? 63 : 64 : 63
                            implicitHeight: 25
                            color: parent.hovered ? parent.pressed ? Qt.darker(
                                                                         "#12b7f5",
                                                                         1.15) : "#47c8f8" : "#12b7f5"
                        }
                    }
                    ToolButton {
                        id: vBtn
                        x: 64
                        onClicked: {
                            vmenu.popup(vmenu.coordination)
                        }

                        Text {
                            anchors.centerIn: parent
                            font.pointSize: parent.font.pointSize - 2
                            textFormat: Text.RichText
                            text: "<font style='color:#ffffff;'>∨</font>"
                        }
                        background: Rectangle {
                            implicitWidth: 24
                            implicitHeight: 25
                            color: parent.hovered ? parent.pressed ? Qt.darker(
                                                                         "#12b7f5",
                                                                         1.15) : "#47c8f8" : "#12b7f5"
                        }
                    }
                }
            }

            background: Item {
                implicitHeight: 40
                implicitWidth: headerRec.width
            }
        }
    }

    //发送菜单
    Menu {
        property int pos: 0 //默认第一项显示
        property point coordination: Qt.point(12, vBtn.height / 2 - height)
        id: vmenu
        width: 196
        height: contentHeight + 12
        visible: false
        topPadding: 6
        bottomPadding: 6
        leftPadding: 1
        rightPadding: 1
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        parent: vBtn //视觉对象 开启相对对象坐标定位
        modal: true
        Overlay.modal: Rectangle {
            color: "#00000000"
        }
        //按Enter键发送消息
        MenuItem {
            id: v1
            height: 30
            width: 194
            leftPadding: 12
            indicator.visible: vmenu.pos == 0 //是否第一项可视
            text: qsTr("按Enter键发送消息")
            checkable: true
            onTriggered: {
                vmenu.pos = 0
            }
        }
        //按Ctrl+Enterr键发送消息
        MenuItem {
            id: v2
            height: 30
            width: 194
            checkable: true
            leftPadding: 12
            text: qsTr("按Ctrl+Enterr键发送消息")
            indicator.visible: !v1.indicator.visible
            onTriggered: {
                vmenu.pos = 1
            }
        }
        delegate: MenuItem {
            property bool ic: false
            id: menuItem
            implicitWidth: 194
            implicitHeight: 30
            arrow: null
            indicator: Text {
                visible: true
                anchors.centerIn: parent
                text: qsTr("✔")
            }
            contentItem: Text {
                leftPadding: 0
                text: menuItem.text
                font: menuItem.font
                opacity: enabled ? 1.0 : 0.3
                elide: Text.ElideRight
            }
            background: Rectangle {
                implicitWidth: 194
                implicitHeight: 30
                opacity: enabled ? 1 : 0.3
                color: menuItem.highlighted ? Qt.lighter("lightgray",
                                                         1.15) : "transparent"
            }
        }
        background: Rectangle {
            implicitHeight: parent.height
            implicitWidth: 196
            radius: 3
            border.color: "lightgray"
        }
    }
    //模型
    ListModel {
        id: model
    }
    //左上角
    Item {
        id: leftTop
        width: enableSize
        height: enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {

                func.press(mouse, chatWin)
            }
            onEntered: func.enter(1, chatWin)
            onReleased: func.release(chatWin)
            onPositionChanged: func.positionChange(mouse, -1, -1, chatWin,
                                                   sizeChanged)
        }
    }

    //Top
    Item {
        id: top
        height: enableSize
        width: chatWin.width - enableSize * 2
        x: enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {

                func.press(mouse, chatWin)
            }
            onEntered: func.enter(2, chatWin)
            onReleased: func.release(chatWin)

            onMouseYChanged: func.positionChange(Qt.point(customPoint.x,
                                                          mouseY), 1, -1,
                                                 chatWin, sizeChanged)
        }
    }

    //右上角
    Item {
        id: rightTop
        width: enableSize
        height: enableSize
        x: chatWin.width - enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                func.press(mouse, chatWin)
            }
            onEntered: func.enter(3, chatWin)
            onReleased: func.release(chatWin)
            onPositionChanged: func.positionChange(mouse, 1, -1, chatWin,
                                                   sizeChanged)
        }
    }

    //Left
    Item {
        id: left
        width: enableSize
        height: chatWin.height - 2 * enableSize
        y: enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {

                func.press(mouse, chatWin)
            }
            onEntered: func.enter(4, chatWin)
            onReleased: func.release(chatWin)

            onMouseXChanged: func.positionChange(Qt.point(mouseX,
                                                          customPoint.y), -1,
                                                 1, chatWin, sizeChanged)
        }
    }

    //Right
    Item {
        id: right
        width: enableSize
        height: chatWin.height - 2 * enableSize
        x: chatWin.width - enableSize
        y: enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {

                func.press(mouse, chatWin)
            }
            onEntered: func.enter(6, chatWin)
            onReleased: func.release(chatWin)

            onMouseXChanged: func.positionChange(Qt.point(mouseX,
                                                          customPoint.y), 1, 1,
                                                 chatWin, sizeChanged)
        }
    }

    //左下角
    Item {
        id: leftBottom
        width: enableSize
        height: enableSize
        y: chatWin.height - enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {

                func.press(mouse, chatWin)
            }
            onEntered: func.enter(7, chatWin)
            onReleased: func.release(chatWin)

            onPositionChanged: func.positionChange(mouse, -1, 1, chatWin,
                                                   sizeChanged)
        }
    }

    //bottom
    Item {
        id: bottom
        height: enableSize
        width: chatWin.width - 2 * enableSize
        x: enableSize
        y: chatWin.height - enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                func.press(mouse, chatWin)
            }
            onEntered: func.enter(8, chatWin)
            onReleased: func.release(chatWin)

            onMouseYChanged: func.positionChange(Qt.point(customPoint.x,
                                                          mouseY), 1, 1,
                                                 chatWin, sizeChanged)
        }
    }

    //右下角
    Item {
        id: rightBottom
        width: enableSize
        height: enableSize
        x: chatWin.width - enableSize
        y: chatWin.height - enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, chatWin)
            }
            onEntered: func.enter(9, chatWin)
            onReleased: func.release(chatWin)

            onPositionChanged: func.positionChange(mouse, 1, 1, chatWin,
                                                   sizeChanged)
        }
    }
}
