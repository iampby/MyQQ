import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1 as Lab
import "qrc:/"
import ImageHelper 1.0

//聊天界面
ApplicationWindow {
    property var obj: null //成员（number headImg tag name）  number 为号码+1 headImg路径 id+notgray 代表彩色图
    property string ip: ""
    property string port: ""
    property string ptext: "" //当前最新消息的纯文本,用于下次列表
    //用于鼠标拉伸
    property int enableSize: 6
    property bool isPressed: false
    property point customPoint: Qt.point(0, 0)
    //发送是否成功
    signal sendResult(bool success, string sendData)
    //参数1是否成功 参数二为发送的xml数据
    //消息发送结果
    signal getInfoListPlainText(string ptext)
    signal loadLog(string html, string time, string mq)
    //加载本地数据库记录 参数为富文本内容 发送时间 发送方的号码
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
            qqMainWin.mapChat.delete(number) //删除记录 号码+"1"
            destroy() //释放资源
        } catch (e) {
            console.log("warning:", e.message)
        }
    }
    //由于改写textedit原因，不能自动焦点处理 这里检测窗口丢失焦点处理其焦点
    onActiveChanged: {
        if (active) {
            edit.focus = true
            edit.delShow = true
            curTimer.restart()
        } else {
            edit.focus = false
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
            obj.headImg = obj.headImg + "NotGray" //加上非 灰色标记
        } catch (e) {
            console.log("warning:", e.message)
        }
    }
    //发送结果
    onSendResult: {
        if (success) {
            console.log("message sending success")
            //成功记录本地消息
            let number = obj.number
            number = number.substring(0, number.length - 1)
            funcc.saveSentFLog(number, sendData)
        } else {
            console.log("message sending failure")
        }
    }
    //获取发送消息纯文本
    onGetInfoListPlainText: {
        chatWin.ptext = ptext.replace(new RegExp("<br />", "gm"), "") //去掉所有换行
        console.log("senting mesage:", ptext)
    }
    //添加记录到模型
    onLoadLog: {
        console.log("loading native chat log")
        var pos
        var url
        if (mq == qqMainWin.myqq) {
            pos = 1
            url = qqMainWin.headSource
            url = url.toString() //转化为js值
        } else {
            pos = 0
            url = obj.headImg
        }
        model.insert(0, {
                         "r_pos": pos,
                         "r_img": url,
                         "r_text": html,
                         "r_time": time
                     }) //前插入
        chatLView.rollbackToBottom = true //滚动到底
    }

    Connections {
        target: funcc
        //截屏处理
        onGetScreenPximap: {
            console.log("onGetScreenPximap:")
            imageHelper.insertImage(pix, obj.number.substring(
                                        0, obj.number.length - 1),
                                    qqMainWin.myqq)
            edit.forceActiveFocus() //焦点重置
        }
        //好友地址记录
        onGetAddress: {
            chatWin.ip = ip
            chatWin.port = port
            console.log("ip=", ip, "port=", port, "status=", status,
                        obj.number.substring(0, obj.number.length - 1))
            if (obj.status != status) {
                console.log("status had changed")
                var m = qqMainWin.friendsModel
                var length = m.length
                for (var i = 0; i < length; ++i) {
                    let item = m[i]
                    let number = obj.number.substring(0, obj.number.length - 1)
                    let j = item.rowOf(number)
                    if (j != -1) {
                        images.flushPixmap2(obj.number, status)
                        item.setData(status, 6)
                        obj.status = status
                        qqMainWin.flushFImage(i) //刷新图片
                        break
                    }
                }
            }
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
    //消息到来 接受消息
    function addMessage(html, time) {
        console.log("addMessage(html)")
        model.append({
                         "r_pos": 0,
                         "r_img": obj.headImg,
                         "r_text": html,
                         "r_time": time
                     })
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
        Keys.enabled: true

        //键盘事件
        Keys.onPressed: {
            var key = event.key
            var modifiers = event.modifiers
            //关闭
            if (key === Qt.Key_C) {
                if (modifiers & Qt.AltModifier) {
                    cBtn.clicked()
                }
            } else if (key === Qt.Key_S) {
                //发送
                if (event.modifiers & Qt.AltModifier) {
                    sendBtn.clicked()
                }
                //截屏
            } else if (key === Qt.Key_A) {
                if (!(modifiers ^ (Qt.ControlModifier + Qt.AltModifier))) {
                    smenu.clicked(0)
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
                color: "white"
                text: title
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

        //聊天框 注意滚动栏必须在listview里才正常，避免listview的flick影响定位，前面的实现错误
        //借用crollview的hovered
        ScrollView {
            id: chatView
            x: bodyRec.border.width
            y: headerRec.height + bodyRec.border.width
            width: headerRec.width
            height: dragItem.y - headerRec.height - bodyRec.border.width
            contentItem: ListView {
                property bool rollbackToBottom: false
                id: chatLView
                width: parent.width
                height: parent.height
                clip: true
                model: model
                interactive: true

                //滚动栏
                ScrollBar.vertical: ScrollBar {
                    id: chatScroll
                    visible: chatLView.contentHeight
                             > chatLView.height ? (chatView.hovered || hovered
                                                   || pressed) ? true : false : false
                    padding: 0
                    parent: chatLView //必须设置父对象才能启动hovered and pressed以及鼠标事件
                    anchors.right: parent.right //绑定右边
                    width: 6
                    height: chatLView.height
                    interactive: true
                    orientation: Qt.Vertical
                    policy: ScrollBar.AlwaysOn
                    size: chatLView.height / chatLView.contentHeight
                    minimumSize: 30 / chatLView.height //最小30
                    hoverEnabled: true
                    z: 1
                    //发送消息就滚动底部
                    onSizeChanged: {
                        if (chatLView.rollbackToBottom) {
                            var tp = 1 - chatScroll.size
                            if (tp < 0)
                                tp = 0
                            chatScroll.position = tp
                            chatLView.rollbackToBottom = false
                        }
                    }
                    contentItem: Rectangle {

                        implicitWidth: 6
                        implicitHeight: chatLView.height * parent.size
                        radius: width / 2
                        color: parent.hovered ? Qt.darker("#d7d7d7",
                                                          1.3) : "#d7d7d7"
                    }
                }
                delegate: Rectangle {
                    property int pos: r_pos //0左边 1右边
                    id: chatItem
                    width: chatLView.width
                    height: chatHead.height > editItem.height ? chatHead.height + chatHead.y : editItem.height + chatHead.y

                    onPosChanged: {
                        canvasItem.requestPaint()
                    }
                    //更新列表高度

                    //时间
                    Label {
                        id: timeLabItem
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: r_time
                        font.pointSize: 9
                        color: "gray"
                    }

                    //头像
                    Rectangle {
                        id: chatHead
                        x: pos == 0 ? 10 : chatItem.width - 10 - width
                        y: timeLabItem.height + 3
                        width: 40
                        height: 40
                        clip: true
                        radius: 90
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                console.log("查看资料")
                                //右边代表是自己
                                if (r_pos == 1) {
                                    actions.openAlterUserInfoAct.trigger()
                                } else {
                                    let data = {}
                                    let number = chatWin.obj.number //赋值角色号码
                                    if (parseInt(number) < 100000) {
                                        console.log("warning:number is not normal")
                                        return
                                    }
                                    number = number.substring(0,
                                                              number.length - 1)
                                    data.number = number
                                    try {
                                        let fsm = qqMainWin.friendsModel
                                        for (let i in fsm) {
                                            let m = fsm[i]
                                            let row = m.rowOf(number)
                                            if (row != -1) {
                                                console.log("find the  friend:",
                                                            number)
                                                data.fgIndex = i
                                                data.last = i
                                                data.name = m.data(row, 1)
                                                data.signature = m.data(row, 2)
                                                data.headImg = m.data(row, 3)
                                                data.tag = m.data(row, 4)
                                                qqMainWin.openFriendDataWin(
                                                            data) //打开资料窗口
                                                break
                                            }
                                        }
                                    } catch (err) {
                                        console.log(err.message)
                                        return
                                    }
                                }
                            }
                        }

                        Image {
                            id: chatHeadImg
                            asynchronous: true
                            cache: false
                            visible: false
                            source: r_img
                            fillMode: Image.PreserveAspectCrop
                            sourceSize: Qt.size(40, 40)
                        }
                        //遮罩
                        OpacityMask {
                            width: chatHead.width
                            height: chatHead.height
                            source: chatHeadImg
                            maskSource: chatHead
                        }
                    }
                    TextArea {
                        property bool control: false //控制文本区域大小，只有第一次变化时才调整大小 不连续调整
                        id: editItem
                        x: pos == 0 ? chatHead.x + 3 + chatHead.width : chatHead.x - 3 - width
                        y: timeLabItem.height + 3
                        width: chatLView.width
                        height: contentHeight + 10
                        focus: true
                        wrapMode: TextEdit.WrapAnywhere
                        textFormat: TextEdit.RichText
                        font.pointSize: 11
                        leftPadding: chatItem.pos == 0 ? 15 : 5
                        rightPadding: chatItem.pos == 0 ? 5 : 15
                        clip: true
                        text: r_text
                        color: "black"
                        readOnly: true
                        onContentWidthChanged: {
                            if (!control) {
                                control = true
                                width = (contentWidth + 20)
                                        > (chatLView.width
                                           - 60) ? (chatLView.width - 60) : (contentWidth + 20)
                            } else {
                                control = false
                            }
                        }
                        onContentHeightChanged: {
                            height = contentHeight + 10
                        }
                        function clearAll() {
                            clear()
                            imageHelperItem.cleanup() //清除 c++资源
                        }

                        ImageHelper {
                            id: imageHelperItem
                            document: editItem.textDocument
                            cursorPosition: editItem.cursorPosition
                            selectionStart: editItem.selectionStart
                            selectionEnd: editItem.selectionEnd

                            onNeedUpdate: {
                                //editItemor.update() 这句不起作用，编辑器未改变，就不会更新，用下面的方法
                                let alpha = editItem.color.a
                                editItem.color.a = alpha - 0.01
                                editItem.color.a = alpha
                            }
                        }
                        background: Canvas {
                            id: canvasItem
                            implicitWidth: parent.width
                            implicitHeight: parent.height
                            contextType: "2d"
                            onHeightChanged: {
                                requestPaint()
                            }

                            onPaint: {
                                var ratio = 7 / 100
                                var ctx = getContext("2d")
                                ctx.reset()
                                ctx.beginPath()
                                var rl = height * ratio
                                if (rl < 7)
                                    rl = 7
                                //尖角开始
                                if (chatItem.pos == 0) {
                                    //顺时针
                                    ctx.moveTo(10, rl + 12)
                                    ctx.lineTo(4, rl + 4)
                                    ctx.lineTo(10, rl + 7)
                                    ctx.lineTo(10, rl)
                                    ctx.roundedRect(10, 0, width - 10,
                                                    height, rl, rl)
                                } else {
                                    //逆时针
                                    ctx.moveTo(width - 10, rl + 12)
                                    ctx.lineTo(width - 4, rl + 4)
                                    ctx.lineTo(width - 10, rl + 7)
                                    ctx.lineTo(width - 10, rl)
                                    ctx.roundedRect(0, 0, width - 10,
                                                    height, rl, rl)
                                }
                                //填充
                                ctx.closePath()
                                ctx.clip()
                                ctx.lineWidth = 1
                                ctx.strokeStyle = "#00000000" //透明
                                ctx.fillStyle = "#e5e5e5"
                                ctx.fill()
                            }
                        }

                        Component.onDestruction: {
                            clearAll() //毁灭释放资源
                        }
                    }
                }
            }
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
            clip: true
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
                    //截屏
                    ToolButton {
                        id: sBtn
                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: {
                                if (mouse.button == Qt.RightButton) {
                                    smenu.popup(-smenu.width / 2, -smenu.height)
                                } else {
                                    smenu.clicked(smenu.pos)
                                }
                            }
                        }
                        Connections {
                            target: smenu
                            onClicked: {
                                var b = smenu.isHide
                                //截屏
                                //屏幕处理时候隐藏窗口
                                if (b) {
                                    chatWin.showMinimized()
                                }
                                if (pos == 0) {
                                    //截屏处理
                                    funcc.screenShot()
                                }
                            }
                        }

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
                    //传文件
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
                    //发送图片
                    ToolButton {
                        onClicked: {
                            localPixMFDlog.open()
                        }
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
                y: toolBar.height
                width: headerRec.width
                height: parent.height - toolBar.height
                contentWidth: edit.paintedWidth
                contentHeight: edit.paintedHeight
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                function ensureVisible(r) {
                    //检测优先级下>上 右>左
                    if (contentX + width <= r.x + r.width)
                        contentX = r.x + r.width - width
                    else if (contentX >= r.x)
                        contentX = r.x
                    if (contentY + height <= r.y + r.height) {
                        contentY = r.y + r.height - height
                    } else if (contentY >= r.y)
                        contentY = r.y
                }
                ScrollBar.vertical: ScrollBar {
                    id: editScroll
                    visible: editFlick.contentHeight
                             > editFlick.height ? (edit.hovered || hovered
                                                   || pressed) ? true : false : false
                    padding: 0
                    width: 6
                    height: editFlick.height
                    anchors.right: parent.right //绑定右边
                    minimumSize: 25 / editFlick.height
                    orientation: Qt.Vertical
                    z: 1
                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: editFlick.height
                        radius: width / 2
                        color: parent.hovered ? Qt.darker("#d7d7d7",
                                                          1.3) : "#d7d7d7"
                    }
                }
                TextEdit {
                    property bool hasClear: false
                    property bool delShow: true
                    property bool hovered: false
                    id: edit
                    width: editFlick.width
                    clip: true
                    wrapMode: TextEdit.Wrap
                    textFormat: TextEdit.RichText
                    font.pointSize: 11
                    color: "black"
                    focus: true
                    Keys.enabled: true
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        Keys.enabled: false
                        onPressed: mouse.accepted = false
                        onHoveredChanged: {
                            parent.hovered = containsMouse
                        }
                    }
                    onCursorRectangleChanged: {
                        editFlick.ensureVisible(cursorRectangle)
                    }
                    //处理enter换行影响
                    onCursorPositionChanged: {
                        if (cursorPosition == 1) {
                            if (hasClear) {
                                hasClear = !hasClear
                                clearAll()
                            }
                        }
                    }
                    onFocusChanged: {
                        console.log("focus", focus)
                        if (focus) {
                            curTimer.restart()
                        } else {
                            curTimer.stop()
                            delShow = false //关闭可视
                        }
                    }

                    cursorDelegate: Rectangle {
                        id: editCur
                        height: edit.font.pixelSize
                        width: 1
                        color: "black"
                        visible: edit.delShow
                        onHeightChanged: {
                            var h = height
                            height = edit.font.pixelSize
                            var off = (h - edit.font.pixelSize)
                            y = off > 0 ? y + off : y
                        }
                    }
                    Keys.onPressed: {

                        var key = event.key
                        var modifiers = event.modifiers
                        if (key === Qt.Key_Return || key === Qt.Key_Enter) {
                            //enter
                            if (modifiers & Qt.ControlModifier) {
                                if (vmenu.pos == 0) {
                                    insert(cursorPosition, "<br />") //富文本换行
                                    event.accpeted = true //不传播到父对象控件
                                    return
                                } else {
                                    sendBtn.clicked()
                                    hasClear = true
                                    event.accpeted = true
                                    return
                                }
                            } else if (!(modifiers ^ Qt.NoModifier)) {
                                if (vmenu.pos == 0) {
                                    sendBtn.clicked()
                                    hasClear = true
                                    event.accpeted = true
                                    return
                                } else {
                                    event.accpeted = true //不传播到父对象控件
                                    return
                                }
                            }
                            //穿透各个快捷事件
                        } else if (key === Qt.Key_C
                                   && modifiers & Qt.AltModifier) {
                            bodyRec.Keys.pressed(event)
                        } else if (key === Qt.Key_S
                                   && modifiers & Qt.AltModifier) {
                            bodyRec.Keys.pressed(event)
                        } else if (key === Qt.Key_A
                                   && (!(modifiers ^ (Qt.ControlModifier + Qt.AltModifier)))) {
                            bodyRec.Keys.pressed(event)
                        }
                    }
                    //清除函数
                    function clearAll() {
                        clear()
                        imageHelper.cleanup() //清除 c++资源
                    }

                    ImageHelper {
                        id: imageHelper
                        document: edit.textDocument
                        cursorPosition: edit.cursorPosition
                        selectionStart: edit.selectionStart
                        selectionEnd: edit.selectionEnd

                        onNeedUpdate: {
                            //别人代码 不知道什么回事
                            //editor.update() 这句不起作用，编辑器未改变，就不会更新，用下面的方法
                            let alpha = edit.color.a
                            edit.color.a = alpha - 0.01
                            edit.color.a = alpha
                        }
                    }
                    Component.onDestruction: {
                        clearAll() //毁灭释放资源
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
            z: 1
            //移动
            MouseCustomForWindow {
                onSendPos: {
                    try {
                        chatWin.x += movedCoordinate.x
                        chatWin.y += movedCoordinate.y
                    } catch (e) {
                        console.log(e, movedCoordinate.x, movedCoordinate.y)
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
                    //发送按钮
                    ToolButton {
                        id: sendBtn
                        onClicked: {
                            var content = edit.text
                            var pt = edit.getText(0, edit.length)

                            edit.clearAll() //先清零
                            pt = pt.split(" ").filter(function (x) {
                                return x != ""
                            })
                            //空内容处理
                            if (pt.length == 0) {
                                console.log("length=0")
                                return
                            }
                            console.log("发送:\n")
                            content = imageHelper.htmlAdjsut(content)
                            chatLView.rollbackToBottom = true
                            var url = qqMainWin.headSource
                            try {
                                url = url.toString() //转换对象为字符串
                            } catch (e) {
                                console.log("warning:", e.message)
                                return
                            }
                            var datetime = new Date()
                            //聊天时间
                            var time = datetime.format("yyyy/MM/dd hh:mm:ss")
                            model.append({
                                             "r_pos": 1,
                                             "r_img": url,
                                             "r_text": content,
                                             "r_time": time
                                         })
                            //聊天列表时间
                            time = datetime.format("yyyy/MM/dd hh:mm")
                            edit.forceActiveFocus() //重获焦点
                            let number = chatWin.obj.number

                            number = number.substring(0,
                                                      number.length - 1) //获取号码
                            //自己不远程
                            if (number != qqMainWin.myqq)
                                funcc.sendFMessage(chatWin.ip, chatWin.port,
                                                   number, content,
                                                   chatWin) //发送消息
                            //自己则更新本地记录
                            else {
                                chatWin.ptext = funcc.saveSentFLog(number,
                                                                   content)
                            }

                            //更新消息列表
                            //先关闭小标签
                            if (loaderForFG.status != Loader.Null) {
                                loaderForFG.item.close()
                            }
                            if (loaderForFriend.status != Loader.Null) {
                                loaderForFriend.item.close()
                            }
                            number = chatWin.obj.number //变回+1状态
                            var length = infoModel.count
                            var existis = false
                            for (var i = 0; i < length; ++i) {
                                if (infoModel.get(i).r_number == number) {
                                    existis = true
                                    infoModel.set(i, {
                                                      "r_imgPath": url,
                                                      "r_title": title,
                                                      "r_tip": chatWin.ptext,
                                                      "r_time": time,
                                                      "r_number": number
                                                  })
                                    infoModel.move(i, 0, 1)
                                }
                            }
                            if (!existis) {
                                infoModel.insert(0, {
                                                     "r_imgPath": url,
                                                     "r_title": title,
                                                     "r_tip": chatWin.ptext,
                                                     "r_time": time,
                                                     "r_number": number
                                                 })
                            }
                            infoTip.visible = false
                        }

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
            //不要item item不会刷新 大量图片会有背景残留
            background: Rectangle {
                implicitHeight: 40
                implicitWidth: headerRec.width
            }
        }
    }
    //文件对话框 本地图片
    Lab.FileDialog {
        id: localPixMFDlog
        title: "打开"
        fileMode: Lab.FileDialog.OpenFiles
        defaultSuffix: "../"
        nameFilters: ["图像文件(*.jpeg;*.jpg;*.png)"]
        onAccepted: {
            var files = localPixMFDlog.files
            var length = files.length
            for (let i in files) {
                imageHelper.insertImage(files[i]) //插入图片
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
    //截屏菜单
    Menu {
        property int pos: 0 //默认截屏点击是第一项触发
        property bool isHide: false //默认第一项显示
        signal clicked(int pos)
        id: smenu
        width: 196
        height: contentHeight + 12
        visible: false
        topPadding: 6
        bottomPadding: 6
        leftPadding: 1
        rightPadding: 1
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        parent: sBtn //视觉对象 开启相对对象坐标定位
        modal: true
        Overlay.modal: Rectangle {
            color: "#00000000"
        }
        //屏幕截图 Ctrl+Alt+A
        MenuItem {
            id: s1
            height: 30
            width: 194
            leftPadding: 12
            indicator.visible: false //是否第一项可视
            text: qsTr("屏幕截图 Ctrl+Alt+A")
            checkable: true
            onTriggered: {
                smenu.pos = 0
                smenu.clicked(smenu.pos)
            }
        }
        //截图时隐藏当前窗口
        MenuItem {
            id: s2
            height: 30
            width: 194
            checkable: true
            leftPadding: 12
            text: qsTr("截图时隐藏当前窗口")
            indicator.visible: smenu.isHide
            onTriggered: {
                smenu.pos = 1
                smenu.isHide = !smenu.isHide
            }
        }
        delegate: MenuItem {
            property bool ic: false
            id: smenuItem
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
                text: smenuItem.text
                font: smenuItem.font
                opacity: enabled ? 1.0 : 0.3
                elide: Text.ElideRight
            }
            background: Rectangle {
                implicitWidth: 194
                implicitHeight: 30
                opacity: enabled ? 1 : 0.3
                color: smenuItem.highlighted ? Qt.lighter("lightgray",
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
    //聊天
    ListModel {
        id: model
    }
    //光标定时器
    Timer {
        id: curTimer
        repeat: true
        interval: 500
        onTriggered: {
            edit.delShow = !edit.delShow
        }
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
