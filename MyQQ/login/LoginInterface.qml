import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: login
    property int cursorShape: 0
    property alias loginCloseBtn: closeBtn
    property alias setTip: tip1
    property alias minimumTip: tip2
    property alias startAn: rotateAn.running
    property alias setAn: setAn
    property alias cwRotateAn: cwRotateAn.running
    property alias account: account
    property alias passwd: passwd
    property alias loginTip: loginTip //登录失败提示
    property alias loginText: loginBtnT.text
    width: imgLogin.width + 2
    height: imgLogin.height + 2
    color: "#00ffffff" //初始化空白 防止画布画图像不快而出现空白
    border.color: "lightgray"
    border.width: 1
    BorderImage {
        id: imgLogin
        anchors.centerIn: parent
        source: "qrc:images/qqLogin.png"
        width: 428
        height: 328
    }
    //设置
    Button {
        id: setBtn
        x: minBtn.x - width
        y: 1
        hoverEnabled: true
        action: actions.setAct
        //keys优先权比控件事件及KeyNavigation事件高
        Keys.onTabPressed: {
            canvasMin.bw = 1
            canvasMin.requestPaint()
            minBtn.forceActiveFocus() //强迫获得焦点
        }
        //onReturnPressed 代表 enter   onEnterPressed 代表 space
        Keys.onReturnPressed: {
            actions.setAct.trigger()
        }
        //是否显示边框线
        onActiveFocusChanged: {
            if (!activeFocus) {
                if (canvasSet.bw != 0) {
                    canvasSet.bw = 0
                    canvasSet.requestPaint()
                }
            }
        }
//画不同图片
        onHoveredChanged: {
            if (hovered) {
                canvasSet.f = 2
                canvasSet.requestPaint()
            } else {
                canvasSet.f = 1
                canvasSet.requestPaint()
            }
        }
        onPressedChanged: {
            if (pressed) {
                canvasSet.f = 1
                canvasSet.requestPaint()
            } else {
                if (hovered) {
                    canvasSet.f = 2
                    canvasSet.requestPaint()
                }
            }
        }
        background: Rectangle {
            implicitWidth: 31
            implicitHeight: 32
        }

        //帆布边框
        Canvas {
            property bool isready: false
            property int f: 0//图片选择
            property int bw: 0//是否画tab事件选中状态
            property var img1
            property var img2
            property string url: "qrc:images/qqSet.png"
            id: canvasSet
            width: setBtn.width
            height: setBtn.height
            contextType: "2d"
            onIsreadyChanged: {
                if (canvasMin.isready && canvasSet.isready
                        && canvasClose.isready) {
                    timer1sForBgd.start()
                }
            }

            onPaint: {
                if (f == 0)
                    return
                var ctx = getContext("2d")
                ctx.reset()
                if (f == 1) {
                    ctx.drawImage(img1, 0, 0, canvasSet.width, canvasSet.height)
                    isready = true //画完后再放标识已经画好背景，注意requestPaint()发出的重画信号是异步执行的,即标识不能放在请求重画之后
                } else {
                    ctx.drawImage(img2, 0, 0, canvasSet.width, canvasSet.height)
                }
                ctx.beginPath()
                ctx.lineWidth = bw
                if (bw != 0)
                    ctx.strokeStyle = "gray"
                else
                    ctx.strokeStyle = "#00ffffff"
                func.dottedLinTo(ctx, 0, 0, canvasSet.width,
                                 canvasSet.height, 50)
            }
            Component.onCompleted: {
                loadImage(url)//加载图片
            }
            onImageLoaded: {
                console.log("loaded set", f)
                if (f == 0) {
                    //先加载状态1图片
                    img1 = context.createImageData("qrc:images/qqSet.png")
                    f = 1
                    img1.height = canvasSet.height
                    img1.width = canvasSet.width
                    requestPaint()//先显示图片
                    unloadImage(url)//卸载后加载另一个图片，这样得到2个图片
                    loadImage("qrc:images/qqSetC.png")
                } else {
                    img2 = context.createImageData("qrc:images/qqSetC.png")
                    img2.height = canvasSet.height
                    img2.width = canvasSet.width
                    //清除资源
                    unloadImage("qrc:images/qqSetC.png")
                }
            }
        }
        //所有的tooltip显示机制全部有缺陷，没有对位置进行优化
        ToolTip {
            id: tip1
            x: 20
            y: 35
            text: "设置"
            width: 40
            height: 30
            visible: setBtn.hovered
        }
    }
    //最小化
    Button {
        id: minBtn
        x: closeBtn.x - width
        y: 1

        hoverEnabled: true
        onClicked: mainWin.hide()
        Keys.onTabPressed: {
            canvasClose.bw = 1
            canvasClose.requestPaint()
            closeBtn.forceActiveFocus()
        }
        onActiveFocusChanged: {
            if (!activeFocus) {
                if (canvasMin.bw != 0) {
                    canvasMin.bw = 0
                    canvasMin.requestPaint()
                }
            }
        }

        //onReturnPressed 代表 enter   onEnterPressed 代表 space
        Keys.onReturnPressed: {
            mainWin.hide()
        }
        background: Rectangle {
            implicitWidth: 31
            implicitHeight: 32
        }
        onHoveredChanged: {
            if (hovered) {
                canvasMin.f = 2
                canvasMin.requestPaint()
            } else {
                canvasMin.f = 1
                canvasMin.requestPaint()
            }
        }
        onPressedChanged: {
            if (pressed) {
                canvasMin.f = 1
                canvasMin.requestPaint()
            } else {
                if (hovered) {
                    canvasMin.f = 2
                    canvasMin.requestPaint()
                }
            }
        }
        //帆布边框
        Canvas {
            property bool isready: false
            property int f: 0
            property int bw: 0
            property var img1
            property var img2
            property string url: "qrc:images/qqMinimum.png"
            id: canvasMin
            width: minBtn.width
            height: minBtn.height
            contextType: "2d"
            onIsreadyChanged: {
                if (canvasMin.isready && canvasSet.isready
                        && canvasClose.isready) {
                    timer1sForBgd.start()
                }
            }
            onPaint: {
                if (f == 0)
                    return
                var ctx = getContext("2d")
                ctx.reset()
                if (f == 1) {
                    ctx.drawImage(img1, 0, 0, canvasMin.width, canvasMin.height)
                    isready = true //画完后再放标识已经画好背景，注意requestPaint()发出的重画信号是异步执行的
                } else {
                    ctx.drawImage(img2, 0, 0, canvasMin.width, canvasMin.height)
                }
                ctx.beginPath()
                ctx.lineWidth = bw
                if (bw != 0)
                    ctx.strokeStyle = "gray"
                else
                    ctx.strokeStyle = "#00000000"
                //画边框断线
                func.dottedLinTo(ctx, 0, 0, canvasMin.width,
                                 canvasMin.height, 50)
            }
            Component.onCompleted: {
                loadImage(url)
            }
            onImageLoaded: {
                console.log("loaded")
                if (f == 0) {
                    img1 = context.createImageData("qrc:images/qqMinimum.png")
                    f = 1
                    img1.height = canvasMin.height
                    img1.width = canvasMin.width
                    requestPaint()
                    unloadImage(url)
                    loadImage("qrc:images/qqMinimumC.png")
                } else {
                    img2 = context.createImageData("qrc:images/qqMinimumC.png")
                    img2.height = canvasMin.height
                    img2.width = canvasMin.width
                    unloadImage("qrc:images/qqMinimumC.png")
                }
            }
        }
        ToolTip {
            id: tip2
            x: 20
            y: 35
            text: "最小化"
            width: 50
            height: 30
            visible: minBtn.hovered
        }
    }
    //关闭
    Button {
        id: closeBtn
        x: login.width - width - 1
        y: 1

        hoverEnabled: true
        KeyNavigation.tab: account.edit //没有附加操作直接导航

        onActiveFocusChanged: {
            if (!activeFocus) {
                if (canvasClose.bw != 0) {
                    canvasClose.bw = 0
                    canvasClose.requestPaint()
                }
            }
        }

        //onReturnPressed 代表 enter   onEnterPressed 代表 space
        Keys.onReturnPressed: {
            Qt.quit()
            console.log("close button clicked")
        }
        onHoveredChanged: {
            if (hovered) {
                canvasClose.f = 2
                canvasClose.requestPaint()
            } else {
                canvasClose.f = 1
                canvasClose.requestPaint()
            }
        }
        onPressedChanged: {
            if (pressed) {
                canvasClose.f = 1
                canvasClose.requestPaint()
            } else {
                if (hovered) {
                    canvasClose.f = 2
                    canvasClose.requestPaint()
                }
            }
        }
        //帆布边框
        Canvas {
            property bool isready: false
            property int f: 0
            property int bw: 0
            property var img1
            property var img2
            property string url: "qrc:images/qqClose.png"
            id: canvasClose
            width: closeBtn.width
            height: closeBtn.height
            contextType: "2d"
            onIsreadyChanged: {
                if (canvasMin.isready && canvasSet.isready
                        && canvasClose.isready) {
                    timer1sForBgd.start()
                }
            }
            onPaint: {
                if (f == 0)
                    return
                var ctx = getContext("2d")
                ctx.reset()
                if (f == 1) {
                    ctx.drawImage(img1, 0, 0, canvasClose.width,
                                  canvasClose.height)
                    isready = true //画完后再放标识已经画好背景，注意requestPaint()发出的重画信号是异步执行的
                } else {
                    ctx.drawImage(img2, 0, 0, canvasClose.width,
                                  canvasClose.height)
                }
                ctx.beginPath()
                ctx.lineWidth = bw
                if (bw != 0)
                    ctx.strokeStyle = "gray"
                else
                    ctx.strokeStyle = "#00000000"
                func.dottedLinTo(ctx, 0, 0, canvasClose.width,
                                 canvasClose.height, 50)
            }
            Component.onCompleted: {
                loadImage(url)
            }
            onImageLoaded: {
                if (f == 0) {
                    img1 = context.createImageData(url)
                    f = 1
                    img1.height = canvasClose.height
                    img1.width = canvasClose.width
                    requestPaint()
                    unloadImage(url)
                    loadImage("qrc:images/qqCloseC.png")
                } else {
                    img2 = context.createImageData("qrc:images/qqCloseC.png")
                    img2.height = canvasClose.height
                    img2.width = canvasClose.width
                    unloadImage("qrc:images/qqCloseC.png")
                }
            }
        }
        background: Rectangle {
            implicitWidth: 31
            implicitHeight: 32
        }
        onClicked: {
            Qt.quit()
            console.log("close button clicked")
        }
        ToolTip {
            id: closeTip
            text: "关闭"
            x: 15
            y: 35
            width: 40
            height: 30
            visible: closeBtn.hovered
        }
    }

    //QQ登入界面的下部分
    ColumnLayout {
        x: 1
        y: 89
        spacing: 10
        width: 428
        Rectangle {
            //头像
            id: imgHeadMask
            Layout.alignment: Qt.AlignHCenter
            width: 65
            height: 65
            radius: 90
            clip: true
            Image {
                id: imgHead
                anchors.fill: parent
                smooth: true
                source: "qrc:images/head.png"
                fillMode: Image.PreserveAspectCrop
                visible: false
                sourceSize: Qt.size(parent.size, parent.size)
            }
            OpacityMask {
                source: imgHead
                maskSource: imgHeadMask
                anchors.fill: parent
            }
        }
        LoginEdit {
            property bool isTopArrow: false
            signal loseFocus
            id: account
            height: 30
            width: 250
            Layout.alignment: Qt.AlignCenter
            edit.selectByMouse: true
            edit.font.pixelSize: 14
            edit.text: "MyQQ号码"
            edit.color: "lightgray"
            KeyNavigation.tab: passwd.edit
            Keys.onReturnPressed: {
                actions.loginAct.trigger()
            }
            edit.onActiveFocusChanged: {
                console.log("edit ", activeFocus)
                if (edit.activeFocus) {
                    isTopArrow = false
                    iconL = "qrc:images/qq小图标"
                    underL.hoverEnabled = false
                    btnR.hoverEnabled = false
                    underL.color = "#ff12b7f5"
                    if (edit.text == "MyQQ号码" & edit.font.pixelSize == 14) {
                        edit.font.pixelSize = 16
                        edit.color = "black"
                        edit.clear()
                    } else
                        edit.selectAll()
                } else {
                    iconL = "qrc:images/qq灰色小图标"
                    btnR.hoverEnabled = true
                    underL.hoverEnabled = true
                    underL.color = "lightgray"
                    if (edit.text == '') {
                        edit.font.pixelSize = 14
                        edit.color = "lightgray"
                        edit.text = "MyQQ号码"
                    }
                }
            }
            btnR.onPressed: actions.accountRAct.trigger()
            btnR.onActiveFocusChanged: {
                if (!btnR.activeFocus) {
                    isTopArrow = false
                }
            }
            onLoseFocus: {
                iconL = "qrc:images/qq灰色小图标"
                underL.color = "lightgray"
                edit.focus = false
                btnR.focus = false
                btnR.hoverEnabled = true
                underL.hoverEnabled = true
            }
            onIsTopArrowChanged: {
                if (isTopArrow) {
                    iconR = "qrc:images/top.png"
                    btnRColor = "#12b7f5"
                } else {
                    iconR = "qrc:images/bottom.png"
                    btnRColor = "#d3d3d3"
                }
            }
            onCursorFromChanged: cursorShape = cursorFrom
        }
        LoginEdit {
            signal loseFocus
            id: passwd
            height: 30
            width: 250
            Layout.alignment: Qt.AlignCenter
            edit.text: "密码"
            iconL: "qrc:images/lockerGray.png"
            iconR: btnR.pressed ? "qrc:images/keybordB.png" : "qrc:images/keybord.png"
            edit.inputMethodHints: Qt.ImhHiddenText //禁用输入法
            edit.color: edit.text == "密码" ? "lightgray" : "black"
            edit.echoMode: edit.text == "密码" ? TextInput.Normal : TextInput.Password
            btnR.action: actions.passwdRAct
            btnRMouse.shapeInside: Qt.PointingHandCursor
            Keys.onTabPressed: {

                canvasRem.bw = 1
                canvasRem.requestPaint()
                rememberC.forceActiveFocus()
            }
            Keys.onReturnPressed: {
                actions.loginAct.trigger()
            }
            edit.validator: RegExpValidator {
                regExp: /[0-9A-Za-z`~!@#$%^&*()_-+={}\[\]:;\"\'<>,.?\\\/]{40}/
            } //密码为40个字符
            edit.onActiveFocusChanged: {
                if (edit.activeFocus) {
                    if (edit.text == "密码")
                        edit.text = ""
                    iconL = "qrc:images/lockerBlue.png"
                    underL.hoverEnabled = false
                    btnR.hoverEnabled = false
                    underL.color = "#ff12b7f5"
                } else {
                    if (edit.text == "")
                        edit.text = "密码"
                    iconL = "qrc:images/lockerGray.png"
                    underL.hoverEnabled = true
                    btnR.hoverEnabled = true
                    underL.color = "lightgray"
                }
            }
            onLoseFocus: {
                edit.focus = false
            }

            onCursorFromChanged: cursorShape = cursorFrom
            btnRMouse.onCursorShapeChanged: login.cursorShape = btnRMouse.cursorShape
        }

        RowLayout {
            Layout.alignment: Qt.AlignCenter
            spacing: 20
            //帆布边框
            Canvas {
                property int bw: 0
                id: canvasAuto
                width: 72
                height: 17
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    if (bw == 0) {
                        ctx.clearRect(0, 0, canvasAuto.width, canvasAuto.height)
                        return
                    }
                    ctx.lineWidth = bw
                    ctx.strokeStyle = "gray"
                    func.dottedLinTo(ctx, 0, 0, canvasAuto.width,
                                     canvasAuto.height, 80)
                }
                CheckBox {
                    id: autoC
                    x: 1
                    y: 1
                    width: parent.width - 2
                    height: parent.height - 2

                    text: qsTr("自动登录")
                    font.pixelSize: 13
                    spacing: 3
                    checked: rememberC.checked ? autoC.checked : false
                    Keys.onTabPressed: {
                        canvasFind.bw = 1
                        canvasFind.requestPaint()
                        findBtn.forceActiveFocus()
                    }
                    onActiveFocusChanged: {
                        if (!activeFocus) {
                            if (canvasAuto.bw != 0) {
                                console.log("auto?")
                                canvasAuto.bw = 0
                                canvasAuto.requestPaint()
                            }
                        }
                    }

                    Keys.onReturnPressed: {
                        actions.loginAct.trigger() //登录点击
                    }

                    indicator: Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: autoC.checked ? "qrc:images/checked.png" : "qrc:images/unchecked.png"
                    }
                    contentItem: Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: autoC.text
                        font: autoC.font
                        color: "gray"
                    }
                    leftPadding: autoC.indicator.width + autoC.spacing
                }
            }
            //帆布边框 记住密码
            Canvas {
                property int bw: 0
                id: canvasRem
                width: 72
                height: 17
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    if (bw == 0) {
                        ctx.clearRect(0, 0, canvasRem.width, canvasRem.height)
                        return
                    }
                    ctx.lineWidth = bw
                    ctx.strokeStyle = "gray"
                    func.dottedLinTo(ctx, 0, 0, canvasRem.width,
                                     canvasRem.height, 80)
                }
                CheckBox {
                    id: rememberC
                    x: 1
                    y: 1
                    width: parent.width - 2
                    height: parent.height - 2
                    text: qsTr("记住密码")
                    font.pixelSize: 13
                    spacing: 3
                    checked: autoC.checked ? true : autoC.checked
                    Keys.onTabPressed: {
                        canvasAuto.bw = 1
                        canvasAuto.requestPaint()
                        autoC.forceActiveFocus()
                    }
                    onActiveFocusChanged: {
                        if (!activeFocus) {
                            if (canvasRem.bw != 0) {
                                canvasRem.bw = 0
                                canvasRem.requestPaint()
                            }
                        }
                    }
                    Keys.onReturnPressed: {
                        actions.loginAct.trigger() //登录点击
                    }
                    indicator: Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: rememberC.checked ? "qrc:images/checked.png" : "qrc:images/unchecked.png"
                    }
                    contentItem: Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: rememberC.text
                        font: rememberC.font
                        color: "gray"
                    }
                    leftPadding: rememberC.indicator.width + rememberC.spacing
                }
            }
            //找到密码
            Canvas {
                property int bw: 0
                id: canvasFind
                width: 60
                height: 17
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    if (bw == 0) {
                        ctx.clearRect(0, 0, canvasFind.width, canvasFind.height)
                        return
                    }
                    ctx.lineWidth = bw
                    ctx.strokeStyle = "gray"
                    func.dottedLinTo(ctx, 0, 0, canvasFind.width,
                                     canvasFind.height, 60)
                }
                Button {
                    id: findBtn
                    x: 1
                    y: 1
                    hoverEnabled: true
                    Text {
                        width: findBtn.width
                        height: findBtn.height
                        color: (findBtn.pressed ? "#ff12b7f5" : (findBtn.hovered ? Qt.darker("gray") : "gray"))
                        text: qsTr("找回密码")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 13
                    }
                    flat: true
                    Keys.onTabPressed: {
                        canvasReg.bw = 1
                        canvasReg.requestPaint()
                        registerBtn.forceActiveFocus()
                    }
                    onActiveFocusChanged: {
                        if (!activeFocus) {
                            if (canvasFind.bw != 0) {
                                canvasFind.bw = 0
                                canvasFind.requestPaint()
                            }
                        }
                    }
                    background: Rectangle {
                        implicitHeight: canvasFind.height - 2
                        implicitWidth: canvasFind.width - 2
                        color: "white"
                    }
                    MouseArea {
                        width: parent.width
                        height: parent.height
                        hoverEnabled: true
                        onPressed: mouse.accepted = false
                        onContainsMouseChanged: {
                            if (containsMouse)
                                login.cursorShape = Qt.IBeamCursor
                            else
                                login.cursorShape = 0
                        }
                    }
                }
            }
        }
        //登录
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 250
            height: 30
            Button {
                id: loginBtn
                anchors.fill: parent
                onPressed: {
                    loginBtnT.font.pixelSize = 18
                }
                onClicked: {
                    actions.loginAct.trigger()
                }
                onReleased: {
                    loginBtnT.font.pixelSize = 16
                }
                Text {
                    id: loginBtnT
                    anchors.centerIn: parent
                    color: "white"
                    text: qsTr("登录")
                    font.pixelSize: 16
                    horizontalAlignment: Text.horizontalCenter
                    //verticalAlignment: Text.verticalCenter
                }
                flat: true
                background: Rectangle {
                    anchors.fill: parent
                    color: "#ff12b7f5"
                }
                ToolTip {
                    id: loginTip
                    x: 180
                    y: -40
                    visible: false
                    text: qsTr("登入失败！\n")
                    font.pointSize: 14
                    delay: 1000
                    timeout: 3000
                    contentItem: Text {
                        color: "red"
                        text: loginTip.text
                        font: loginTip.font
                    }
                    background: Rectangle {
                        implicitWidth: 70
                        implicitHeight: 60
                        border.width: 1
                        border.color: "red"
                    }
                }
            }
        }
    }
    //登入状态在头像之后声明才能作为顶层部件
    Rectangle {
        id: imgHeadStatusMask
        x: 232
        y: 138
        width: 16
        height: 16
        radius: 90
        clip: true
        Image {
            id: imgHeadStatus
            anchors.fill: parent
            source: "qrc:images/qqGreenL.png"
            fillMode: Image.PreserveAspectCrop
            visible: false
            smooth: true
            sourceSize: Qt.size(parent.size, parent.size)
        }
        OpacityMask {
            source: imgHeadStatus
            maskSource: imgHeadStatusMask
            anchors.fill: parent
        }
    }
    //注册按钮
    Canvas {
        property int bw: 0
        id: canvasReg
        x: 10
        y: login.height - 28
        width: 72
        height: 17
        contextType: "2d"
        onPaint: {
            var ctx = getContext("2d")
            if (bw == 0) {
                ctx.clearRect(0, 0, canvasReg.width, canvasReg.height)
                return
            }
            ctx.lineWidth = bw
            ctx.strokeStyle = "gray"
            func.dottedLinTo(ctx, 0, 0, canvasReg.width, canvasReg.height, 60)
        }
        Button {
            id: registerBtn
            x: 1
            y: 1

            flat: true
            Keys.onReturnPressed: {
                actions.registerAct.trigger()
            }

            Keys.onTabPressed: {
                canvasSet.bw = 1
                canvasSet.requestPaint()
                setBtn.forceActiveFocus()
                console.log("tab press")
            }
            onActiveFocusChanged: {
                if (!activeFocus) {
                    if (canvasReg.bw != 0) {
                        canvasReg.bw = 0
                        canvasReg.requestPaint()
                    }
                }
            }

            action: actions.registerAct
            Text {
                width: registerBtn.width
                height: registerBtn.height
                color: registerBtn.pressed ? "#ff12b7f5" : "gray"
                text: qsTr("注册账号")
                font.pixelSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                width: parent.width
                height: parent.height
                hoverEnabled: true
                //propagateComposedEvents: true
                onPressed: mouse.accepted = false
                onContainsMouseChanged: {
                    if (containsMouse) {
                        login.cursorShape = Qt.PointingHandCursor
                    } else
                        login.cursorShape = Qt.ArrowCursor
                }
            }
            background: Rectangle {
                implicitWidth: canvasReg.width - 2
                implicitHeight: canvasReg.height - 2
                color: "white"
            }
        }
    }
    Button {
        //二维码登录待实现
        id: codeBtn
        x: 1
        y: 1
        width: 25
        height: 25
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        enabled: true
        hoverEnabled: true
        anchors.margins: 5
        background: Image {
            anchors.fill: parent
            source: codeBtn.pressed ? "qrc:images/codeB.png" : "qrc:images/code.png"
        }
        ToolTip {
            id: codeTip
            text: "二维码登录"
            x: 20
            y: 30
            visible: codeBtn.hovered
            delay: 500
        }
    }
    //// 因为帆布背景线程为异步画的原因，延迟一秒显示 ，降低出现空白的概率
    Timer {
        id: timer1sForBgd
        interval: 1000
        onTriggered: {
            login.color = "#ffffff"
        }
    }

    transform: Rotation {
        origin: {
            origin.x = 428 / 2
            origin.y = 328 / 2
        }
        axis {
            x: 0
            y: 1
            z: 0
        }
        NumberAnimation on angle {
            //设置按钮的前半部分旋转动画
            running: false
            id: rotateAn
            from: 0
            to: 90
            duration: 500
            loops: 1
        }
        NumberAnimation on angle {
            //设置界面的OKCNCEL按钮的后半部分旋转动画
            running: false
            id: cwRotateAn
            from: 90
            to: 0
            duration: 500
            loops: 1
        }
    }
    SequentialAnimation on visible {
        //设置按钮的旋转动画
        id: setAn
        running: false
        loops: 1
        PropertyAnimation {
            target: login
            property: "border.width"
            to: 1
            duration: 0
        }
        PropertyAnimation {
            target: login
            property: "startAn"
            to: true
            duration: 0
        }

        PauseAnimation {
            duration: 500
        }
        PropertyAnimation {
            target: login
            property: "visible"
            to: false
            duration: 0
        }
        PropertyAnimation {
            target: loginSetInf
            properties: "startAcwAn,visible"
            to: true
            duration: 0
        }
        PropertyAnimation {
            target: login
            property: "border.width"
            to: 1
            duration: 0
        }
    }
}
