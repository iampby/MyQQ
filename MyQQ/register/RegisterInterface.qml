import QtQuick 2.11
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import "qrc:/"

Rectangle {
    property alias timerLImg: timer
    property alias container: container
    property real lengthR: 550
    property alias scr1: scroll1
    property alias scr2: scroll2
    property alias scr3: scroll3
    property alias name: name.text
    property alias passwd: passwd.text
    property real dpJust: 1.0 //默认为1，即控件不变，当分辨率为800*600时启用调整
    property bool hasClicked: false
    property string myqq: ""
    implicitWidth: imgL.width + lengthR
    implicitHeight: 500
    color: "#fff3f3f5"

    Connections {
        target: funcc
        onRegisterResult: {
            hasClicked = false
            if (result) {
                container.currentIndex = 1
                scr1.visible = false
                scr2.visible = true
            } else {
                container.currentIndex = 2
                scr1.visible = false
                scr3.visible = true
            }
            myqq = myQQ
        }
    }

    Rectangle {
        id: imgL
        width: 300
        height: 500
        anchors.top: parent.top
        anchors.left: parent.left
        Image {
            id: img
            MouseMoveCustomForInner {
                isLoginInf: false
            }
            anchors.fill: parent
            source: "qrc:images/imgRegister/img1.png"
            // fillMode: Image.PreserveAspectFit
        }
        Timer {
            id: timer
            interval: 5000
            running: true
            repeat: true
            onTriggered: {
                if (img.source.toString() == "qrc:/images/imgRegister/img1.png")
                    img.source = "qrc:/images/imgRegister/img2.png"
                else if (img.source.toString(
                             ) == "qrc:/images/imgRegister/img2.png")
                    img.source = "qrc:/images/imgRegister/img3.png"
                else
                    img.source = "qrc:/images/imgRegister/img1.png"
            }
        }
    }
    Container {
        id: container
        anchors.left: imgL.right
        anchors.top: parent.top
        currentIndex: 0
        contentItem: ListView {
            model: container.contentModel
            snapMode: ListView.SnapOneItem
            highlightRangeMode: ListView.StrictlyEnforceRange
            orientation: ListView.Vertical
            highlightMoveVelocity: 3000
            currentIndex: container.currentIndex
            interactive: false
        }
        //用于动态调整scrollbarview的contentHeight属性
        function scrollBarHeightChanged(visible, height) {
            if (visible) {
                scroll1.contentHeight += height
            } else {
                scroll1.contentHeight -= height
            }
        }
        ScrollView {
            id: scroll1
            // property alias scrollHeight: contentHeight
            visible: true
            width: lengthR
            height: 500
            Flickable {
                id: flick
                boundsBehavior: Flickable.StopAtBounds
            }
            MouseMoveCustomForInner {
                isLoginInf: false
            }
            spacing: 20
            //anchors.fill: parent //不能对container子项进行布局
            contentHeight: 252 + 278
            ScrollBar.vertical.policy: contentHeight
                                       > 500 ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.width: 20
            RegisterInfLangMinClose {}
            ColumnLayout {
                id: col1
                x: 95 * dpJust
                y: 90 * dpJust
                Layout.alignment: Qt.AlignHCenter
                Text {
                    Layout.alignment: Qt.AlignLeft
                    text: qsTr("欢迎注册QQ")
                    font.bold: true
                    font.pixelSize: 24
                }
                Text {
                    Layout.alignment: Qt.AlignLeft
                    text: qsTr("每一天，乐在沟通。")
                    font.pixelSize: 20
                }
            }
            ColumnLayout {
                id: col2
                x: 95 * dpJust
                y: 180 * dpJust
                spacing: 20
                ColumnLayout {
                    spacing: 2
                    Rectangle {
                        width: 340
                        height: 38
                        TextField {
                            id: name
                            anchors.fill: parent
                            placeholderText: qsTr("昵称")
                            focus: true
                            font.pixelSize: 20
                            onActiveFocusChanged: {
                                if (name.activeFocus == false) {
                                    var isSpace = true
                                    if (name.text == '')
                                        nameTipText.text = "昵称不能为空"
                                    else
                                        nameTipText.text = "昵称不能为空格"
                                    for (var i = 0; i < name.length; ) {
                                        var t = name.getText(i, ++i)
                                        if (t !== ' ') {
                                            isSpace = false
                                            break
                                        }
                                    }
                                    if (isSpace)
                                        nameTip.visible = true
                                } else
                                    nameTip.visible = false
                            }
                        }
                    }
                    RowLayout {
                        id: nameTip
                        visible: false
                        spacing: 0
                        Rectangle {
                            radius: 90
                            width: 18
                            height: 18
                            Text {
                                text: qsTr("!")
                                anchors.centerIn: parent
                                color: "white"
                                font.pixelSize: 17
                            }
                            color: "red"
                        }
                        Label {
                            id: nameTipText
                            text: "昵称不能为空格"
                            font.pixelSize: 16
                            color: "red"
                        }
                        onVisibleChanged: container.scrollBarHeightChanged(
                                              //利用visible属性实时更新contentHeight
                                              visible, height)
                    }
                }
                ColumnLayout {
                    spacing: 2
                    Rectangle {
                        width: 340
                        height: 38
                        TextField {
                            id: passwd
                            property bool isNSpace: true
                            property bool is8_16Char: false
                            property bool isMTwoRune: false
                            anchors.fill: parent
                            echoMode: TextInput.Password //密码模式会自动关闭中文输入法
                            validator: RegExpValidator {
                                regExp: /[0-9A-Za-z`~!@#$%^&*()_-+={}\[\]:;\"\'<>,.?\\\/\s]{40}/
                            } //密码为40个字符包含`~!@#$%^&*()-_=+{}[]:;"'|\<\,>.?/以及空格,可以去掉但显得空格提示多余
                            placeholderText: qsTr("密码")
                            font.pixelSize: 20
                            onActiveFocusChanged: {
                                if (passwd.activeFocus == false) {
                                    console.log("passwd t->", passwd.text)
                                    if (passwd.getText(0,
                                                       passwd.length) === '') {
                                        passwdEmptyTip.visible = true
                                        spaceTip.visible = false
                                        lengthTip.visible = false
                                        twoKindTip.visible = false
                                    } else {
                                        passwdEmptyTip.visible = false
                                        if (!isNSpace) {
                                            spaceTip.visible = true
                                            lengthTip.visible = false
                                            twoKindTip.visible = false
                                        } else if (!is8_16Char) {
                                            spaceTip.visible = false
                                            lengthTip.visible = true
                                            twoKindTip.visible = false
                                        } else if (!isMTwoRune) {
                                            spaceTip.visible = false
                                            lengthTip.visible = false
                                            twoKindTip.visible = true
                                        } else {
                                            spaceTip.visible = false
                                            lengthTip.visible = false
                                            twoKindTip.visible = false
                                        }
                                    }
                                } else {
                                    spaceTip.visible = true
                                    lengthTip.visible = true
                                    twoKindTip.visible = true
                                    passwdEmptyTip.visible = false
                                }
                            }
                            onTextChanged: {
                                isNSpace = true
                                is8_16Char = false
                                isMTwoRune = true
                                var kindIntRune = 1 //%2、3、5为0则分别代表包含有数字、字母、符号
                                var l = passwd.length
                                if (l >= 8)
                                    is8_16Char = true
                                for (var i = 0; i < l; ) {
                                    //密码为40个字符含`~!@#$%^&*()-_=+{}[]:;"'|\<\,>.?/以及空格
                                    var t = passwd.getText(i, ++i)
                                    var digit = /[0-9]/
                                    var letter = /[a-zA-Z]/
                                    var symbol = /[`~!@#$%^&*()_-+={}\[\]:;\"\'<>,.?\\\/]/
                                    if (t === '\x20')
                                        isNSpace = false
                                    if (digit.test(t)) {
                                        kindIntRune *= 2
                                    } else if (letter.test(t))
                                        kindIntRune *= 3
                                    else if (symbol.test(t))
                                        kindIntRune *= 5
                                }
                                if (isNSpace) {
                                    spaceTipText.text = "✔"
                                } else
                                    spaceTipText.text = "!"
                                if (is8_16Char)
                                    lengthTipText.text = "✔"
                                else
                                    lengthTipText.text = "!"
                                if (((kindIntRune % 2 == 0 & kindIntRune % 3
                                      != 0 & kindIntRune % 5 != 0)
                                     | (kindIntRune % 2 != 0 & kindIntRune
                                        % 3 == 0 & kindIntRune % 5
                                        != 0) | (kindIntRune % 2 != 0
                                                 & kindIntRune % 3 != 0 & kindIntRune
                                                 % 5 == 0) & kindIntRune != 1) | kindIntRune == 1)
                                    isMTwoRune = false
                                if (isMTwoRune)
                                    twoKindTipText.text = "✔"
                                else
                                    twoKindTipText.text = "!"
                            }
                        }
                    }
                    RowLayout {
                        id: passwdEmptyTip
                        visible: false
                        spacing: 0
                        Rectangle {
                            radius: 90
                            width: 18
                            height: 18
                            Text {
                                text: qsTr("!")
                                anchors.centerIn: parent
                                color: "white"
                                font.pixelSize: 17
                            }
                            color: "red"
                        }
                        Label {
                            text: "密码不能为空"
                            font.pixelSize: 16
                            color: "red"
                        }
                        onVisibleChanged: container.scrollBarHeightChanged(
                                              visible, height)
                    }
                    RowLayout {
                        id: spaceTip
                        visible: false
                        spacing: 0
                        Rectangle {
                            radius: 90
                            width: 18
                            height: 18
                            Text {
                                id: spaceTipText
                                text: "✔"
                                anchors.centerIn: parent
                                color: "white"
                                font.pixelSize: 17
                            }
                            //"#ff44f86c"=浅绿色 "#ffabc8f8"=浅蓝色 密码框颜色变化设置
                            color: passwd.activeFocus ? (passwd.isNSpace ? "#ff44f86c" : "#ffabc8f8") : visible ? "red" : "transparent"
                        }
                        Label {
                            id: spaceTipLabel
                            text: "不能包含空格"
                            font.pixelSize: 16
                            color: passwd.activeFocus ? "black" : "red"
                        }
                        onVisibleChanged: container.scrollBarHeightChanged(
                                              visible, height)
                    }
                    RowLayout {
                        id: lengthTip
                        visible: false
                        spacing: 0
                        Rectangle {
                            radius: 90
                            width: 18
                            height: 18
                            Text {
                                id: lengthTipText
                                text: "!"
                                anchors.centerIn: parent
                                color: "white"
                                font.pixelSize: 17
                            }
                            //"#ff44f86c"=浅绿色 "#ffabc8f8"=浅蓝色 密码框颜色变化设置
                            color: passwd.activeFocus ? (passwd.is8_16Char ? "#ff44f86c" : "#ffabc8f8") : visible ? "red" : "transparent"
                        }
                        Label {
                            id: lengthTipLabel
                            text: "长度为8-16个字符"
                            font.pixelSize: 16
                            color: passwd.activeFocus ? "black" : "red"
                        }
                        onVisibleChanged: container.scrollBarHeightChanged(
                                              visible, height)
                    }
                    RowLayout {
                        id: twoKindTip
                        visible: false
                        spacing: 0
                        Rectangle {
                            radius: 90
                            width: 18
                            height: 18
                            Text {
                                id: twoKindTipText
                                text: "!"
                                anchors.centerIn: parent
                                color: "white"
                                font.pixelSize: 17
                            }
                            //"#ff44f86c"=浅绿色 "#ffabc8f8"=浅蓝色 密码框颜色变化设置
                            color: passwd.activeFocus ? (passwd.isMTwoRune ? "#ff44f86c" : "#ffabc8f8") : visible ? "red" : "transparent"
                        }
                        Label {
                            id: twoKindTipLabel
                            text: "必须包含字母、数字、符号中至少2种"
                            font.pixelSize: 16
                            color: passwd.activeFocus ? "black" : "red"
                        }
                        onVisibleChanged: container.scrollBarHeightChanged(
                                              visible, height)
                    }
                }
                GridLayout {
                    columns: 2
                    rowSpacing: 10
                    columnSpacing: 2
                    Rectangle {
                        width: 160
                        height: 30
                        ComboBox {
                            editable: true
                            id: numberCBox
                            anchors.fill: parent
                            textRole: "prefixNum"
                            currentIndex: 0
                            // rightPadding: 30
                            contentItem: TextField {
                                id: ct
                                clip: true
                                leftPadding: 0
                                rightPadding: numberCBox.indicator.width + numberCBox.spacing
                                text: numberCBox.displayText
                                font: numberCBox.font
                                verticalAlignment: Text.AlignVCenter
                                background: Rectangle {
                                    color: "transparent"
                                }
                                onTextChanged: {
                                    if (!numberPop.visible)
                                        retrun
                                    tempModel.clear()
                                    ct.text.match()
                                }
                                onPressed: {
                                    numberPop.visible = true
                                }
                                onReleased: {
                                    numberPop.visible = true
                                }

                                MouseArea {
                                    width: 20
                                    height: 30
                                    anchors.right: parent.right
                                    propagateComposedEvents: true
                                    onPressed: {
                                        mouse.accepted = true
                                        if (numberPop.visible)
                                            numberPop.close()
                                        else
                                            numberPop.open()
                                    }
                                }
                            }

                            model: ListModel {
                                id: numberModel
                                ListElement {
                                    country: "中国"
                                    prefixNum: "+86"
                                }
                                ListElement {
                                    country: "中国香港特别行政区"
                                    prefixNum: "+852"
                                }
                                ListElement {
                                    country: "中国澳门特别行政区"
                                    prefixNum: "+853"
                                }
                                ListElement {
                                    country: "中国台湾地区"
                                    prefixNum: "+886"
                                }
                            }
                            delegate: ItemDelegate {
                                width: 340
                                height: 25
                                contentItem: Text {
                                    text: country + " " + prefixNum
                                    color: "#000000"
                                    font: numberCBox.font
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }
                                highlighted: numberCBox.highlightedIndex === index
                            }

                            indicator: Canvas {
                                id: canvas
                                x: numberCBox.width - width - numberCBox.rightPadding
                                y: numberCBox.topPadding + (numberCBox.availableHeight - height) / 2
                                width: 12
                                height: 10
                                contextType: "2d"

                                Connections {
                                    target: numberPop
                                    onVisibleChanged: {
                                        canvas.requestPaint()
                                    }
                                }

                                onPaint: {
                                    context.reset()
                                    if (!numberPop.visible) {
                                        context.moveTo(0, 0)
                                        context.lineTo(width, 0)
                                        context.lineTo(width / 2, height)
                                        context.closePath()
                                    } else {
                                        context.moveTo(width / 2, 0)
                                        context.lineTo(0, height)
                                        context.lineTo(width, height)
                                    }
                                    context.closePath()
                                    context.fillStyle = "lightgray"
                                    context.fill()
                                }
                            }

                            background: Rectangle {
                                implicitWidth: 160
                                implicitHeight: 30
                                border.color: numberCBox.activeFocus ? "blue" : "lightgray"
                                border.width: numberCBox.visualFocus ? 2 : 1
                                radius: 2
                            }

                            popup: Popup {
                                id: numberPop
                                y: numberCBox.height - 2
                                width: 340
                                height:50
                                padding: 1

                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: numberCBox.delegateModel
                                    currentIndex: numberCBox.highlightedIndex

                                    ScrollIndicator.vertical: ScrollIndicator {}
                                }

                                background: Rectangle {
                                    border.color: "lightgray"
                                    radius: 2
                                }
                            }
                        }
                    }

                    Rectangle {
                        width: 170
                        height: 30
                        visible: true
                        TextField {
                            id: numberEdit
                            anchors.fill: parent
                            placeholderText: qsTr("手机号码")
                            font.pixelSize: 20
                            onActiveFocusChanged: {
                                if (activeFocus) {
                                    numberEffectTip.text = "可通过该手机号码找回密码"
                                    numberEffectTip.color = "gray"
                                    rowVerify.visible = true
                                } else {
                                    var digit = /[0-9]{+}/
                                    if (digit.test(numberEdit.text)) {
                                        numberEffectTip.text = "手机号码格式不正确"
                                        numberEffectTip.color = "red"
                                        return
                                    }
                                    rowVerify.visible = false
                                }
                            }
                        }
                    }
                    Label {
                        id: numberEffectTip
                        anchors.topMargin: 2
                        visible: true
                        text: "可通过该手机号码找回密码"
                        font.pixelSize: 14
                        color: "gray"
                    }
                }
                RowLayout {
                    id: rowVerify
                    spacing: 10
                    visible: false
                    Rectangle {
                        width: 160
                        height: 30
                        TextField {
                            anchors.fill: parent
                            placeholderText: "短信验证码"
                            font.pixelSize: 20
                        }
                    }
                    Rectangle {
                        width: 170
                        height: 30
                        radius: 15
                        Button {
                            id: sendBtn
                            property color backColor: "#ff3587ff"
                            anchors.fill: parent
                            visible: true
                            hoverEnabled: true
                            onHoveredChanged: {
                                if (hovered) {
                                    backColor = Qt.darker("#ff3587ff", 1.2)
                                    tip.visible = true
                                } else
                                    backColor = "#ff3587ff"
                            }
                            ToolTip {
                                id: tip
                                x: 50
                                y: -15
                                width: 80
                                height: 30
                                text: "该功能未实现！"
                                delay: 200
                                timeout: 2000
                                contentItem: Text {
                                    text: tip.text
                                    font: tip.font
                                    color: "red"
                                }
                                background: Rectangle {
                                    radius: 8
                                    color: "white"
                                    border.color: "blue"
                                }
                            }
                            Text {
                                anchors.centerIn: parent
                                text: qsTr("发送验证码")
                            }
                            background: Rectangle {
                                implicitWidth: 160
                                implicitHeight: 30
                                color: sendBtn.backColor
                            }
                        }
                    }
                    onVisibleChanged: container.scrollBarHeightChanged(visible,
                                                                       height)
                }
                Rectangle {
                    width: 340
                    height: 40
                    radius: 15
                    Button {
                        property color backColor: "#ff3587ff"
                        property bool isReady: true
                        id: registerBtn
                        Text {
                            anchors.fill: parent
                            text: qsTr("立即注册")
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 23
                        }
                        background: Rectangle {
                            implicitWidth: 340
                            implicitHeight: 40
                            color: registerBtn.backColor
                        }
                        onHoveredChanged: {
                            if (hovered)
                                backColor = Qt.darker("#ff3587ff", 1.2)
                            else
                                backColor = "#ff3587ff"
                        }
                        onClicked: {
                            isReady = true //默认准备好
                            if (passwd.text == '') {
                                passwdEmptyTip.visible = true
                                isReady = false
                            }
                            if (name.text == '') {
                                nameTip.visible = true
                                isReady = false
                            }
                            if (servicesCBox.checked)
                                servicesTip.visible = false
                            else {
                                servicesTip.visible = true
                                isReady = false
                            }
                            if (nameTip.visible | !passwd.isNSpace
                                    | !passwd.is8_16Char | !passwd.isMTwoRune)
                                isReady = false
                            if (isReady) {
                                console.log("register")

                                if (pop.opened) {
                                    pop.close()
                                }
                                // flick.contentY = scroll1.contentHeight - 500
                                actions.myQQRegAct.trigger()
                            }
                        }
                    }
                }
                RowLayout {
                    id: servicesTip
                    visible: false
                    spacing: 0
                    Rectangle {
                        radius: 90
                        width: 18
                        height: 18
                        Text {
                            text: "!"
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: 17
                        }
                        color: "red"
                    }
                    Label {
                        text: "请先同意服务条款"
                        font.pixelSize: 16
                        color: "red"
                    }
                    onVisibleChanged: container.scrollBarHeightChanged(visible,
                                                                       height)
                }
                RowLayout {
                    CheckBox {
                        id: servicesCBox
                        text: "我已阅读并同意相关服务条款和隐私政策"
                        contentItem: Text {
                            rightPadding: servicesCBox.indicator.width + servicesCBox.spacing
                            text: servicesCBox.text
                            font: servicesCBox.font
                            color: "gray"
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                        indicator: Rectangle {
                            implicitWidth: 18
                            implicitHeight: 18
                            color: "transparent"
                            border.color: "transparent"
                            Text {
                                x: 2
                                y: 5
                                horizontalAlignment: Text.AlignHCenter
                                text: qsTr("✔")
                                color: "#ff3587ff"
                                font.pixelSize: 16
                                visible: servicesCBox.checked
                            }
                            Rectangle {
                                x: 0
                                y: 7
                                width: 13
                                height: 13
                                color: "transparent"
                                visible: true
                                border.color: "gray"
                            }
                        }
                        // palette: palette.text = "#ff3587ff"
                        leftPadding: servicesCBox.indicator.width + servicesCBox.spacing
                    }
                    Button {
                        id: btn
                        text: "▼"
                        font.pixelSize: 16
                        onClicked: {
                            if (text === "▼") {
                                text = "▲"
                                scroll1.contentHeight += 60
                                pop.open()
                            } else {
                                text = "▼"
                                pop.close()
                                scroll1.contentHeight -= 60
                            }
                        }
                        contentItem: Text {
                            text: btn.text
                            font: btn.font
                            color: "gray"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            implicitWidth: 15
                            implicitHeight: 10
                            color: "transparent"
                        }
                    }
                    Popup {
                        id: pop
                        y: servicesCBox.height
                        clip: true
                        visible: false
                        focus: true
                        padding: 1
                        width: 100
                        height: 60
                        background: Rectangle {
                            color: "transparent"
                            border.color: "transparent"
                        }

                        // modal: true
                        // closePolicy: Popup.CloseOnPressOutsideParent//默认 closed on outside parent
                        closePolicy: Popup.NoAutoClose
                        ColumnLayout {
                            anchors.fill: parent
                            Label {
                                text: qsTr("《MyQQ号码规则》")
                                color: "#ff3587ff"
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log(" text:《MyQQ号码规则》clicked")
                                    }
                                }
                            }
                            Label {
                                text: qsTr("《隐私政策》")
                                color: "#ff3587ff"
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log(" text:《隐私政策》clicked")
                                    }
                                }
                            }
                            Label {
                                text: qsTr("《MyQQ空间服务协议》")
                                color: "#ff3587ff"
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log(" text:《MyQQ空间服务协议》clicked")
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Label {
                x: 130 * dpJust
                y: scroll1.contentHeight - 32
                text: "Copyright © 1998-" + new Date().toLocaleString(
                          Qt.LocalDate, Locale.ShortFormat).slice(
                          0, 4) + " MyCompany All Rights Reserved"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "gray"
                font.pixelSize: 12 //默认就为12，与控件高度保持一致
            }
        }
        ScrollView {
            id: scroll2
            visible: false
            width: lengthR
            height: 500
            MouseMoveCustomForInner {
                isLoginInf: false
            }
            spacing: 20
            contentHeight: 500 //默认500
            ScrollBar.vertical.policy: contentHeight
                                       > 500 ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.width: 20
            RegisterInfLangMinClose {}
            Rectangle {
                x: parent.width / 2 - width / 2 //水平居中
                y: 100
                width: 60
                height: 60
                radius: 90
                color: "#ff64e79d" //"#ff3587ff"
                Text {
                    anchors.centerIn: parent
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: qsTr("✔")
                    color: "white"
                    font.pixelSize: 40
                }
            }
            Label {
                x: parent.width / 2 - width / 2 //水平居中
                y: 170
                text: qsTr("注册成功")
                font.pixelSize: 23
            }

            ColumnLayout {
                x: parent.width / 2 - width / 2 //水平居中
                y: 223
                spacing: 30
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 0
                    Label {
                        text: "保存MyQQ号码"
                        font.pixelSize: 20
                    }
                    Rectangle {
                        width: 15
                        height: 20
                        Button {
                            anchors.fill: parent
                            flat: true
                            Image {
                                anchors.fill: parent
                                width: 15
                                height: 20
                                source: "qrc:images/imgRegister/nodebook.png"
                            }
                            action: actions.nodebookAct
                        }
                    }
                }
                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "请妥善保管好你的MyQQ"
                    color: "red"
                    font.pixelSize: 16
                }
                Rectangle {
                    Layout.alignment: Qt.AlignHCenter
                    width: 340
                    height: 40
                    radius: 15
                    Button {
                        id: loginBtn
                        property color backColor: "#ff3587ff"
                        Text {
                            anchors.fill: parent
                            text: qsTr("立即登录")
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 23
                        }
                        background: Rectangle {
                            implicitWidth: 340
                            implicitHeight: 40
                            color: loginBtn.backColor
                        }
                        onHoveredChanged: {
                            if (hovered)
                                backColor = Qt.darker("#ff3587ff", 1.2)
                            else
                                backColor = "#ff3587ff"
                        }
                        action: actions.immediateLogAct
                    }
                }
            }
            Label {
                x: 130 * dpJust
                y: scroll2.contentHeight - 32
                text: "Copyright © 1998-" + new Date().toLocaleString(
                          Qt.LocalDate, Locale.ShortFormat).slice(
                          0, 4) + " MyCompany All Rights Reserved"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "gray"
                font.pixelSize: 12 //默认就为12，与控件高度保持一致
            }
        }
        ScrollView {
            id: scroll3
            visible: false
            width: lengthR
            height: 500
            MouseMoveCustomForInner {
                isLoginInf: false
            }
            spacing: 20
            contentHeight: 500 //默认500
            ScrollBar.vertical.policy: contentHeight
                                       > 500 ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.width: 20
            RegisterInfLangMinClose {}
            Rectangle {
                x: parent.width / 2 - width / 2 //水平居中
                y: 100
                width: 60
                height: 60
                radius: 90
                color: "red"
                Text {
                    anchors.centerIn: parent
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: qsTr("!")
                    color: "white"
                    font.pixelSize: 40
                }
            }
            Label {
                x: parent.width / 2 - width / 2 //水平居中
                y: 170
                text: qsTr("注册失败")
                font.pixelSize: 23
            }
            Label {
                x: parent.width / 2 - width / 2 //水平居中
                y: 223
                Layout.alignment: Qt.AlignHCenter
                text: "由于一些原因，服务器繁忙..."
                color: "red"
                font.pixelSize: 20
            }

            Button {
                property color backColor: "#ff3587ff"
                id: returnBtn
                x: parent.width / 2 - width / 2
                y: 340
                visible: true
                hoverEnabled: true
                onClicked: {
                    actions.immediateLogAct.trigger()
                }

                onHoveredChanged: {
                    if (hovered) {
                        backColor = Qt.darker("#ff3587ff", 1.2)
                    } else
                        backColor = "#ff3587ff"
                }

                Text {
                    anchors.centerIn: parent
                    text: qsTr("去登录")
                    font.pointSize: 20
                    color: "white"
                }
                background: Rectangle {
                    implicitWidth: 140
                    implicitHeight: 45
                    color: sendBtn.backColor
                }
            }
            Label {
                x: 130 * dpJust
                y: scroll3.contentHeight - 32
                text: "Copyright © 1998-" + new Date().toLocaleString(
                          Qt.LocalDate, Locale.ShortFormat).slice(
                          0, 4) + " MyCompany All Rights Reserved"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "gray"
                font.pixelSize: 12 //默认就为12，与控件高度保持一致
            }
        }
    }
}
