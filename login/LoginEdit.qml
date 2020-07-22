import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0
import "qrc:/"

//账号与密码输入组件
FocusScope {
    property alias iconL: iconL.source //左边图标路径别名
    property alias iconR: iconR.source //右边图标路径别名
    property alias edit: edit //编辑设置
    property alias btnR: btn //右边图标按钮
    property alias underL: underline //下横线
    property alias btnRColor: overlay.color //按钮图标颜色
    property int cursorFrom: 0
    property alias btnRMouse: btnRMouse
    Column {
        spacing: 1
        height: 30
        width: 250
        Row {
            spacing: 5
            Layout.alignment: Qt.AlignCenter
            Rectangle {
                width: 20
                height: 20
                Image {
                    id: iconL
                    source: "qrc:images/qq灰色小图标.png"
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                }
            }
            Rectangle {
                width: 200
                height: 25
                TextInput {
                    id: edit
                    anchors.fill: parent
                    focus: true //注意整个控件默认赋予的焦点，唯一
                    clip: true //限制内容不超过控件
                    font.pixelSize: 16
                    font.family: "宋体"
                    font.weight: Font.Black
                    echoMode: TextInput.Normal
                    selectionColor: "#ff12b7f5"
                    verticalAlignment: TextInput.AlignVCenter
                    validator: RegExpValidator {
                        regExp: /.{40}/
                    } //正则匹配40个任意字符
                    MouseForShape {
                        shapeInside: Qt.IBeamCursor
                        shapeOutside: Qt.ArrowCursor
                        onCursorShapeChanged: cursorFrom = cursorShape
                    }
                }
            }
            Rectangle {
                width: 20
                height: 20
                Button {
                    id: btn
                    anchors.fill: parent
                    hoverEnabled: true
                    flat: true
                    background: Rectangle {
                        implicitWidth: 20
                        implicitHeight: 20
                        color: "transparent"
                    }
                    onHoveredChanged: {
                        if (hoverEnabled) {
                            if (hovered) {
                                underline.color = "gray"
                                overlay.color = "gray"
                            } else {
                                underline.color = "lightgray"
                                overlay.color = "lightgray"
                            }
                        }
                    }
                    MouseForShape {
                        id: btnRMouse
                    }

                    Image {
                        id: iconR
                        source: "qrc:images/bottom.png"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectCrop
                    }
                    ColorOverlay {
                        id: overlay
                        anchors.fill: iconR
                        source: iconR
                        color: "#d3d3d3" //初始化为浅灰色
                    }
                }
            }
        }
        Rectangle {
            property alias hoverEnabled: underlineMouse.hoverEnabled
            id: underline
            height: 1
            width: parent.width
            color: "lightgray"
            MouseArea {
                id: underlineMouse
                width: parent.width
                height: 1
                // cursorShape: Qt.SplitVCursor//范围小于鼠标形状无效
                hoverEnabled: true
                onContainsMouseChanged: {
                    if (containsMouse) {
                        underline.color = "gray"
                    } else {
                        underline.color = "lightgray"
                    }
                }
            }
        }
    }
}
