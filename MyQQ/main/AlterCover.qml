import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import "qrc:/"

//资料界面的更改封面界面
Window {
    signal updateCover
    id: win
    width: 386
    height: 507

    flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
    color: "lightgray" //边界颜色
    visible: true
    title: "更改封面"

    //关闭处理
    onClosing: {
        console.log("onClosing:interface for cover")
        hide()
        funcc.closeCoverWidget() //释放widget资源
        loaderForAlterCover.source = "" //释放win资源
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
        x: 1
        y: 1
        width: 384
        height: 505
        //header
        Rectangle {
            id: header
            width: 384
            height: 30
            Image {
                id: headerImg
                sourceSize: Qt.size(384, 30)
                source: "qrc:/images/mainInterface/alterCover.png"
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

                    background: Image {
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
                Button {
                    id: closeBtn
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
                        color: closeBtn.hovered ? (closeBtn.pressed ? Qt.darker(
                                                                          "#d44027",
                                                                          1.2) : "#d44027") : "#12b7f5"
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
        y: win.height - 37
        background: Rectangle {
            implicitHeight: 35
            implicitWidth: win.width - 2
            color: "#e5eff7"
        }
        RowLayout {
            x: bottomFrame.width / 2 + 4
            spacing: 15
            Layout.alignment: Qt.AlignTop
            Button {
                id: okBtn
                text: "确认"
                onClicked: {
                    funcc.emitCoverOKClicked(funcc.myQQ) //fa发送信号给更改封面视图区
                    console.log("clicked okBtn,now win will be closed")
                    updateCover()
                    win.close()
                    console.log("win was closed")
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
            Button {
                id: cancelBtn
                text: "取消"
                onClicked: {
                    close()
                }
                background: Rectangle {
                    implicitWidth: 72
                    implicitHeight: 22
                    color: cancelBtn.hovered ? (cancelBtn.pressed ? Qt.darker(
                                                                        "#12b7f5",
                                                                        1.25) : Qt.darker(
                                                                        "#12b7f5",
                                                                        1.10)) : "#12b7f5"
                }
            }
        }
    }
}
