import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import "../"

//头像悬浮时弹出的界面
Window {
    id: win
    width: 386
    height: 618
    flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
    color: "lightgray" //边界颜色
    visible: true
    //移动鼠标
    title: "更换头像"
    onClosing: {
        hide()
        console.log("close")
        funcc.closeWidget() //释放widget资源 必须在loader释放之前
        loaderForAlterHImg.source = "" //相当于deletelater
    }
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
        height: 616
        //header
        Rectangle {
            id: header
            width: 384
            height: 30
            Image {
                id: headerImg
                sourceSize: Qt.size(384, 30)
                source: "qrc:/images/mainInterface/alterHImgHeader.png"
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
                        console.log("???")
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
        ToolButton {
            id: openImgBtn
            padding: 1
            x: 18
            y: header.y + header.height + 10
            // icon: "qrc:/images/mainInterface/openArrow.png"
            onClicked: {
                fDlog.open()
            }

            background: Rectangle {
                implicitHeight: 32
                implicitWidth: 130
                color: openImgBtn.hovered ? "#f8f8f8" : "transparent"
                border.width: 1
                border.color: "darkgray"
                Image {
                    x: 15
                    y: 1
                    sourceSize: Qt.size(16, 30)
                    source: "qrc:/images/mainInterface/openArrow.png"
                }
                Text {
                    x: 32
                    height: 32
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("打开本地照片")
                    font.pointSize: 11
                }
            }
        }
    }
    FileDialog {
        id: fDlog
        title: "打开"
        fileMode: FileDialog.OpenFile
        defaultSuffix: "../"
        nameFilters: ["Images(*.jpeg;*.jpg;*.png)"]
        onAccepted: {
            console.log("You chose: " + fDlog.file)
            funcc.openFile(fDlog.file)
        }
        onRejected: {
            console.log("Canceled")
        }
    }
}
