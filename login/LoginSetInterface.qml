import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import "qrc:/"

//只是界面,没有设计功能
Rectangle {
    id: setInf
    width: 430
    height: 330
    color: "white"
    anchors.margins: 1

    border {
        color: "lightgray"
        width: 1
    }
    property alias startAcwAn: acwRotateAn.running //控制是否开启从-90度到0逆时针旋转
    property alias startCwAn: cwRotateAn.running //控制是否开启从0度到-90顺时针旋转
    property alias okCancelAn: okCancelAn //确定和取消按钮的动画
    property alias imgMinBright: bright //最小化按钮图标亮度
    property alias ok: okBtn //确定按钮
    MouseMoveCustomForInner {
        isLoginInf: false
    }
    BorderImage {
        anchors.margins: 1
        anchors.fill: parent
        source: "qrc:images/云.png"
        border.left: 5
        border.top: 5
        border.right: 5
        border.bottom: 5
    }
    Button {
        id: closeBtn
        anchors.margins: 1
        anchors.top: parent.top
        anchors.right: parent.right
        hoverEnabled: true
        background: Image {
            source: closeBtn.pressed ? "qrc:images/qqSetClose.png" : (closeBtn.hovered ? "qrc:images/qqSetCloseC.png" : "qrc:images/qqSetClose.png")
        }
        onClicked: {
            Qt.quit()
        }
        ToolTip {
            id: closeTip
            visible: closeBtn.hovered
            delay: 500
            text: "关闭"
            x: 20
            y: 34
        }
    }
    Button {
        id: minBtn
        anchors.margins: 1
        anchors.top: parent.top
        anchors.right: closeBtn.left
        hoverEnabled: true
        background: Image {
            id: imgMin
            source: "qrc:images/qqSetMinimum.png"
        }
        ToolTip {
            id: minTip
            visible: minBtn.hovered
            delay: 500
            text: "最小化"
            x: 20
            y: 34
        }
        BrightnessContrast {
            id: bright
            brightness: minBtn.pressed ? 0.5 : (minBtn.hovered ? 0.3 : 0.0)
            anchors.fill: imgMin
            source: imgMin
        }
        onClicked: {
            mainWin.hide()
        }
    }

    ColumnLayout {
        spacing: 10
        x: 21
        y: 61
        Text {
            font.pixelSize: 14
            text: qsTr("网路设置")
        }
        GridLayout {
            columns: 3
            layoutDirection: Qt.RightToLeft
            rowSpacing: 5
            columnSpacing: 5
            RowLayout {
                spacing: 1
                Label {
                    font.pixelSize: 12
                    text: qsTr("端口:")
                }
                FocusScope {
                    width: 50
                    height: 24
                    Rectangle {
                        width: parent.width
                        height: parent.height
                        TextField {
                            id: portEdit
                            width: parent.width
                            height: parent.height
                            focus: true
                            //enabled: false
                            font.pixelSize: 12
                            validator: IntValidator {
                                bottom: 0
                                top: 99999
                            }
                        }
                    }
                }
            }
            RowLayout {
                spacing: 1
                Label {
                    text: qsTr("地址:")
                    font.pixelSize: 12
                }
                Rectangle {
                    width: 100
                    height: 24
                    TextField {
                        id: addressEdit
                        anchors.fill: parent
                        hoverEnabled: true
                        font.pixelSize: 12
                        validator: RegExpValidator {
                            regExp: /.{20}/
                        } //正则匹配20个‘[^\n]’的任意字符
                    }
                }
            }

            RowLayout {
                spacing: 1
                Label {
                    font.pixelSize: 12
                    text: qsTr("类型:")
                }
                Rectangle {
                    width: 130
                    height: 24
                    ComboBox {
                        anchors.fill: parent
                        font.pixelSize: 11
                        // flat: true
                        model: ["不使用代理", "HTTP代理", "SOCKS5代理", "使用浏览器设置"]
                        currentIndex: 0
                        Rectangle {
                            anchors.fill: parent
                            anchors.topMargin: -1
                            anchors.bottomMargin: -1
                        }
                    }
                }
            }
            RowLayout {
                spacing: 1
                Label {
                    text: qsTr("域:")
                    font.pixelSize: 12
                }
                Rectangle {
                    width: 50
                    height: 24
                    TextField {
                        id: domEdit
                        anchors.fill: parent
                        // hoverEnabled: true
                        font.pixelSize: 12
                        validator: RegExpValidator {
                            regExp: /.{20}/
                        } //正则匹配20个‘[^\n]’的任意字符
                    }
                }
            }
            RowLayout {
                spacing: 1
                Label {
                    text: qsTr("密码:")
                    font.pixelSize: 12
                }
                Rectangle {
                    width: 100
                    height: 24
                    TextField {
                        id: passwdEdit
                        anchors.fill: parent
                        // hoverEnabled: true
                        font.pixelSize: 12
                        validator: RegExpValidator {
                            regExp: /.{23}/
                        } //正则匹配23个‘[^\n]’的任意字符
                        echoMode: TextInput.Password
                    }
                }
            }
            RowLayout {
                spacing: 1
                Label {
                    text: qsTr("用户名:")
                    font.pixelSize: 12
                }
                Rectangle {
                    id: userEdit
                    width: 130
                    height: 24
                    TextField {
                        anchors.fill: parent
                        // hoverEnabled: true
                        font.pixelSize: 12
                        validator: RegExpValidator {
                            regExp: /.{20}/
                        } //正则匹配20个‘[^\n]’的任意字符
                    }
                }
            }
            Rectangle {
                width: 50
                height: 24
                Button {
                    id: testBtn
                    anchors.fill: parent
                    text: "测试"
                }
            }
        }
        Text {
            text: qsTr("登入服务器")
            font.pixelSize: 14
        }
        RowLayout {
            layoutDirection: Qt.RightToLeft
            RowLayout {
                spacing: 1
                Text {
                    text: qsTr("端口:")
                }
                FocusScope {
                    width: 50
                    height: 24
                    Rectangle {
                        anchors.fill: parent
                        TextField {
                            id: portEdit1
                            anchors.fill: parent
                            font.pixelSize: 12
                            validator: IntValidator {
                                bottom: 0
                                top: 99999
                            }
                        }
                    }
                }
            }
            RowLayout {
                spacing: 1
                Text {
                    text: qsTr("地址:")
                }
                ComboBox {
                    model: ["183.60.48.174"]
                    font.pixelSize: 11
                    currentIndex: 0
                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 24
                        color: "white"
                    }
                }
            }
            RowLayout {
                spacing: 1
                Label {
                    text: "  类型:"
                    font.pixelSize: 12
                }
                ComboBox {
                    model: ["不使用高级选项", "UDP类型", "TCP类型"]
                    font.pixelSize: 11
                    currentIndex: 0
                    background: Rectangle {
                        implicitWidth: 130
                        implicitHeight: 24
                        color: "white"
                    }
                }
            }
        }
    }
    Frame {
        anchors.margins: 1
        anchors.bottom: parent.bottom
        RowLayout {
            width: 110
            height: 30
            anchors.right: parent.right
            Button {
                id: okBtn
                text: qsTr("确定")
                KeyNavigation.tab: cancelBtn
                Keys.onReturnPressed: {
                    actions.okCancelAct.trigger()
                }

                background: Rectangle {
                    implicitWidth: 50
                    implicitHeight: 24
                    border.color: "darkblue"
                    border.width: okBtn.activeFocus ? 1 : 0
                    color: okBtn.pressed ? Qt.lighter(
                                               "white") : (okBtn.hovered ? "blue" : "white")
                }
                onClicked: actions.okCancelAct.trigger() //只能绑定一个
            }
            Button {
                id: cancelBtn
                text: qsTr("取消")
                KeyNavigation.tab: okBtn
                Keys.onReturnPressed: {
                    actions.okCancelAct.trigger()
                }

                background: Rectangle {
                    implicitWidth: 50
                    implicitHeight: 24
                    border.color: "darkblue"
                    border.width: cancelBtn.activeFocus ? 1 : 0
                    color: cancelBtn.pressed ? Qt.lighter(
                                                   "white") : (cancelBtn.hovered & !cancelBtn.pressed ? "blue" : "white")
                }
                onClicked: actions.okCancelAct.trigger() //只是外貌设计
            }
        }
        background: Rectangle {
            implicitWidth: 428
            implicitHeight: 28
            color: "lightblue"
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
            //设置旋转效果的后半部分
            running: false
            id: acwRotateAn
            from: -90
            to: 0
            duration: 500
            loops: 1
        }
        NumberAnimation on angle {
            //确定和取消旋转效果的前半部分
            running: false
            id: cwRotateAn
            from: 0
            to: -90
            duration: 500
            loops: 1
        }
    }
    SequentialAnimation on visible {
        //OkCancel按钮的旋转动画
        id: okCancelAn
        running: false
        loops: 1
        PropertyAnimation {
            target: setInf
            property: "startCwAn"
            to: true
            duration: 0
        }

        PauseAnimation {
            duration: 500
        }
        PropertyAnimation {
            target: setInf
            property: "visible"
            to: false
            duration: 0
        }
        PropertyAnimation {
            target: loginInf
            property: "cwRotateAn"
            to: true
            duration: 0
        }
        PropertyAnimation {
            target: loginInf
            property: "visible"
            to: true
            duration: 0
        }
    }
}
