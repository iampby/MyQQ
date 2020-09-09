import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import "qrc:/"

//头像悬浮时弹出的界面
Window {
    id: win
    width: 386
    height: 618
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowStaysOnTopHint //显示任务栏
    color: "lightgray" //边界颜色
    visible: true
    title: "更换头像"
    function midFunc() {
        console.log("midfunc invocations")
        inCenterLoader.item.histroyImgModel.isgot = true
        func.addHeadUrl()
    }
    //widget获得焦点 重置
    function alterSelectedIndex() {
        console.log("alterSelectedIndex invocations")
        historyList.selectedIndex = -1 //重置
        historyList.selectedUrl = "-1"
    }

    Connections {
        target: images
        onReadHistoryHeadImg: {
            console.log("fileList", fileList)
            var arr = fileList.split(".")
            console.log("arr", arr)
            var obj = []
            for (var i = 0; i < arr.length - 1; ++i) {
                obj[i] = {
                    "url": arr[i],
                    "index": i + 1
                }
                console.log("i+1:", i + 1)
                console.log(obj[i].url)
            }
            inCenterLoader.item.histroyImgModel.append(obj)
        }
    }

    //关闭处理
    onClosing: {
        hide()
        funcc.closeWidget() //释放widget资源 必须在loader释放之前
        loaderForAlterHImg.source = "" //相当于deletelater
        console.log("AlterHImag is realse")
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
    Label {
        id: label
        x: 18
        y: win.height - 126
        text: qsTr("我使用过的头像：")
    }

    //历史头像列表
    Button {
        id: leftBtn
        x: 3
        y: label.y + label.height + 5
        visible: false
        height: 58
        width: 12
        onClicked: {
            var index = historyList.currentIndex
            index -= 6
            if (index < 6)
                leftBtn.visible = false

            if (index < 0)
                index = 0
            historyList.positionViewAtIndex(index, ListView.Beginning)
            historyList.currentIndex = index
            console.log("current index", historyList.currentIndex)
            rightBtn.visible = true
        }

        background: Rectangle {
            implicitHeight: 58
            implicitWidth: 12
            color: leftBtn.hovered ? leftBtn.pressed ? Qt.darker(
                                                           "#e5e5e5") : "#e5e5e5" : "transparent"
            Text {
                anchors.centerIn: parent
                font.pointSize: 13
                text: qsTr("<")
                color: "gray"
            }
        }
    }
    ListView {
        property int selectedIndex: -1
        property string selectedUrl: "-1"

        id: historyList
        visible: true
        width: 353
        height: 58
        contentHeight: 58
        contentWidth: 0
        x: 18
        y: label.y + label.height + 5
        spacing: 1
        snapMode: ListView.SnapToItem
        highlightRangeMode: ListView.ApplyRange
        clip: true
        currentIndex: 0
        orientation: ListView.Horizontal
        onCountChanged: {
            var length = count / 6
            if (count % 6 != 0)
                length += 1
            contentWidth = length * 353
        }
        onSelectedIndexChanged: {
            if (selectedIndex != -1) {
                var str = selectedUrl
                str = str.substring(str.lastIndexOf("/") + 1, str.length)
                funcc.emitSelectedImg(images.findPixmap(str))
            }
        }
        model: inCenterLoader.item.histroyImgModel
        delegate: Rectangle {
            id: rec
            property alias label: clabel.visible
            implicitHeight: 58
            implicitWidth: 58
            border.color: cbtn.hovered ? "#12b7f5" : "transparent"
            border.width: 2
            Button {
                id: cbtn
                x: 1
                y: 1
                width: 56
                height: 56
                onClicked: {
                    console.log("index:", index, url)
                    historyList.selectedUrl = url
                    historyList.selectedIndex = index
                }

                background: Image {
                    sourceSize: Qt.size(56, 56)
                    asynchronous: true
                    cache: false //更新就写入
                    source: url
                }
            }

            //选中状态小圆圈
            Label {
                id: clabel
                visible: historyList.selectedIndex == index
                width: 16
                height: 16
                text: "√"
                x: 36
                y: 36
                color: "white"
                font.pointSize: 12
                background: Rectangle {
                    implicitHeight: 16
                    implicitWidth: 16
                    color: "#12b7f5"
                    radius: 90
                }
            }
        }
    }

    Button {
        id: rightBtn
        height: 58
        width: 12
        x: win.width - 15
        y: label.y + label.height + 5
        onClicked: {
            var index = historyList.currentIndex, count = historyList.count
            index += 6
            console.log("rightBtn clicked,index Changed", index, count,
                        historyList.count)
            if (index >= 12)
                rightBtn.visible = false

            if (index >= count)
                return
            historyList.positionViewAtIndex(index, ListView.Beginning)
            historyList.currentIndex = index
            leftBtn.visible = true
        }

        background: Rectangle {
            implicitHeight: 58
            implicitWidth: 12
            color: rightBtn.hovered ? rightBtn.pressed ? Qt.darker(
                                                             "#e5e5e5",
                                                             1.15) : "#e5e5e5" : "transparent"
            Text {
                anchors.centerIn: parent
                font.pointSize: 13
                text: qsTr(">")
                color: "gray"
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
                    funcc.emitHeadImgOKClicked(images) //fa发送信号给头像视图区
                    console.log("clicked okBtn,now win will be closed")
                    win.close()
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
    //文件打开框，因为Qtc++的静态调用文件对话框不会自动释放资源,用qml更方便
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
