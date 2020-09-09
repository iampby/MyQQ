import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import QtQml 2.12
import "qrc:/"

//编辑资料界面
Window {
    property var cwin: null //用于子部件
    property alias okBtn: okBtn //用于子部件
    signal inintHandleData
    signal addEduInfo(string school, string detail, string flags)
    id: win
    width: 386
    height: 727

    flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
    color: "lightgray" //边界颜色
    visible: true
    title: "编辑资料"

    //关闭处理
    onClosing: {
        console.log("onClosing:interface for editing information")
        hide() //不退出app
        if (cwin != null)
            cwin.close()
        loaderForAlterInfo.item.loaderForEditInfo.source = "" //释放win资源
    }
    //初始化处理数据
    onInintHandleData: {
        var bir = [] //生日 3 length
        var bloodGroup = "" //血型
        var personalStatement = "" //个人说明
        var educationExperence = "" //教育经历
        var profession = "" //职业
        var corporation = "" //公司
        var phone = "" //电话号码
        var location1 = "" //所在地1
        var location2 = "" //所在地2
        var location3 = "" //所在地3
        var location4 = "" //所在地4
        var home1 = "" //故乡1
        var home2 = "" //故乡2
        var home3 = "" //故乡3
        var home4 = "" //故乡4
        bir = indivadualWin.bir
        bloodGroup = indivadualWin.bloodGroup
        personalStatement = indivadualWin.personalStatement
        // educationExperence = indivadualWin.educationExperence
        profession = indivadualWin.profession
        corporation = indivadualWin.corporation
        phone = indivadualWin.phone
        location1 = indivadualWin.location1
        location2 = indivadualWin.location2
        location3 = indivadualWin.location3
        location4 = indivadualWin.location4
        home1 = indivadualWin.home1
        home2 = indivadualWin.home2
        home3 = indivadualWin.home3
        home4 = indivadualWin.home4
        fieldName.text = qqMainWin.name
        //教育经历
        var temp
        var i
        var length
        length = indivadualWin.eduModel.count
        for (i = 0; i < length; ++i) {
            eduModel.append({
                                "r_school": indivadualWin.eduModel.get(
                                                i).r_school,
                                "r_detail": indivadualWin.eduModel.get(
                                                i).r_detail,
                                "r_flags": indivadualWin.eduModel.get(
                                               i).r_flags,
                                "r_index": i
                            })
        }

        //性别设置
        switch (qqMainWin.sex) {
        case '女':
            sexCBox.currentIndex = 1
            break
        case '男':
            sexCBox.currentIndex = 0
            break
        default:
            sexCBox.currentIndex = -1
            break
        }
        console.log("sexCBox.currentIndex=", sexCBox.currentIndex)
        switch (bloodGroup) {
        case "":
            bloodCBox.currentIndex = 0
            break
        case "A型":
            bloodCBox.currentIndex = 1
            break
        case "B型":
            bloodCBox.currentIndex = 2
            break
        case "O型":
            bloodCBox.currentIndex = 3
            break
        case "AB型":
            bloodCBox.currentIndex = 4
            break
        case "其它血型":
            bloodCBox.currentIndex = 5
            break
        default:
            bloodCBox.currentIndex = -1
            break
        }
        if (bir[0] === undefined || bir[0] === "") {
            console.log(" the year part of birthday is lacked")
        }
        var year = bir[0] + "年"
        var index = ageCbox.find(year)
        if (index !== -1)
            ageCbox.currentIndex = index
        else
            ageCbox.currentIndex = (ageCbox.count - 1)
        var day = bir[2] + "日"
        var mon = bir[1] + ("月")
        index = monthCbox.find(mon)
        if (index !== -1)
            monthCbox.currentIndex = index
        else
            monthCbox.currentIndex = 0

        index = dayCbox.find(day)
        if (index !== -1) {
            dayCbox.currentIndex = index
        } else {
            console.log("the day part of birthday is not found value is ",
                        bir[2])
            dayCbox.currentIndex = 0
        }
        editTag.text = qqMainWin.signature
        switch (profession) {
        case "计算机/互联网/通信":
            professCBox.currentIndex = 0
            break
        case "生产/工艺/制造":
            professCBox.currentIndex = 1
            break
        case "医疗/治理/制药":
            professCBox.currentIndex = 2
            break
        case "金融/银行/投资/保险":
            professCBox.currentIndex = 3
            break
        case "商业/服务业/个体经营":
            professCBox.currentIndex = 4
            break
        case "文化/广告/传媒":
            professCBox.currentIndex = 5
            break
        case "娱乐/艺术/表演":
            professCBox.currentIndex = 6
            break
        case "律师/法务":
            professCBox.currentIndex = 7
            break
        case "教育/培训":
            professCBox.currentIndex = 8
            break
        case "公务员/行政/事业单位":
            professCBox.currentIndex = 9
            break
        case "模特":
            professCBox.currentIndex = 10
            break
        case "空姐":
            professCBox.currentIndex = 11
            break
        case "学生":
            professCBox.currentIndex = 12
            break
        case "其他":
            professCBox.currentIndex = 13
            break
        default:
            professCBox.currentIndex = -1
            break
        }
        fieldCorporation.text = corporation //公司
        fieldPhone.text = (phone === "0" ? "" : phone)
        editStatement.text = personalStatement
        console.log("education erperence:", educationExperence)
        //添加城市数据
        //禁用它，一切变化交个索引变化信号解决
        where2Cbox.enabled = false
        where3Cbox.enabled = false
        where4Cbox.enabled = false
        home2Cbox.enabled = false
        home3Cbox.enabled = false
        home4Cbox.enabled = false
        //所在地处理
        length = indivadualWin.countryModel.count
        console.log("1")
        index = -1
        for (i = 0; i < length; ++i) {
            temp = indivadualWin.countryModel.get(i).name
            if (temp === location1) {
                index = i
                break
            }
        }
        if (index === -1) {
            console.log("location is a empty information ")
            where1Cbox.currentIndex = -1
        } else {
            where1Cbox.currentIndex = index
            if (location2 !== "") {
                length = provinceModel1.count
                index = -1
                for (i = 0; i < length; ++i) {
                    temp = provinceModel1.get(i).name
                    if (temp === location2) {
                        index = i
                        break
                    }
                }
                if (index === -1) {
                    where2Cbox.currentIndex = -1
                } else {
                    where2Cbox.currentIndex = index
                    if (location3 !== "") {
                        length = cityModel1.count
                        index = -1
                        for (i = 0; i < length; ++i) {
                            temp = cityModel1.get(i).name
                            if (temp === location3) {
                                index = i
                                break
                            }
                        }
                        if (index === -1) {
                            where3Cbox.currentIndex = -1
                        } else {
                            where3Cbox.currentIndex = index
                            if (location4 !== "") {
                                length = countyModel1.count
                                index = -1
                                for (i = 0; i < length; ++i) {
                                    temp = countyModel1.get(i).name
                                    if (temp === location4) {
                                        index = i
                                        break
                                    }
                                }
                                if (index === -1) {
                                    where4Cbox.currentIndex = -1
                                } else {
                                    where4Cbox.currentIndex = index
                                }
                            }
                        }
                    }
                }
            }
        }
        //故乡处理
        console.log("2")
        index = -1
        length = indivadualWin.countryModel.count
        for (i = 0; i < length; ++i) {
            try {
                temp = indivadualWin.countryModel.get(i).name
            } catch (e) {
                console.log("warning:", e.message)
            }
            if (temp === home1) {
                index = i
                break
            }
        }
        if (index === -1) {
            console.log("location is a empty information ")
            home1Cbox.currentIndex = -1
        } else {
            home1Cbox.currentIndex = index
            if (home2 !== "") {
                length = provinceModel2.count
                index = -1
                for (i = 0; i < length; ++i) {
                    temp = provinceModel2.get(i).name
                    if (temp === home2) {
                        index = i
                        break
                    }
                }
                if (index === -1) {
                    home2Cbox.currentIndex = -1
                } else {
                    home2Cbox.currentIndex = index
                    if (home3 !== "") {
                        length = cityModel2.count
                        index = -1
                        for (i = 0; i < length; ++i) {
                            temp = cityModel2.get(i).name
                            if (temp === home3) {
                                index = i
                                break
                            }
                        }
                        if (index === -1) {
                            home3Cbox.currentIndex = -1
                        } else {
                            home3Cbox.currentIndex = index
                            if (home4 !== "") {
                                length = countyModel2.count
                                index = -1
                                for (i = 0; i < length; ++i) {
                                    temp = countyModel2.get(i).name
                                    if (temp === home4) {
                                        index = i
                                        break
                                    }
                                }
                                if (index === -1) {
                                    home4Cbox.currentIndex = -1
                                } else {
                                    home4Cbox.currentIndex = index
                                }
                            }
                        }
                    }
                }
            }
        }
        console.log("end")
        okBtn.enabled = false //初始化为false
    }
    //添加教育信息
    onAddEduInfo: {
        console.log("onAddEduInfo")
        console.log("school:", school, "detail:", detail, "flags:", flags)
        var index = eduModel.count
        eduModel.append({
                            "r_school": school,
                            "r_detail": detail,
                            "r_flags": flags,
                            "r_index": index
                        })
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
        height: 688
        //header 标题栏
        MouseCustomForWindow {
            onSendPos: {
                win.x += movedCoordinate.x
                win.y += movedCoordinate.y
            }
        }
        Rectangle {
            id: header
            width: 384
            height: 30
            Image {
                id: headerImg
                sourceSize: Qt.size(384, 30)
                source: "qrc:/images/mainInterface/alterEditInfo.png"
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
        //基础信息
        ScrollView {
            y: 45
            height: 640
            width: 384
            clip: true
            contentHeight: cont2.y + cont2.height
            ScrollBar.vertical: ScrollBar {}
            MouseCustomForWindow {
                onSendPos: {
                    win.x += movedCoordinate.x
                    win.y += movedCoordinate.y
                }
            }
            Item {
                id: foundationInfo
                x: 15
                width: 350
                height: 160
                Label {
                    text: "基础信息"
                    font.family: "黑体"
                    font.pixelSize: 13
                    font.wordSpacing: 0
                    font.bold: true
                }
                ColumnLayout {
                    y: 20
                    spacing: 15
                    //分割条
                    Rectangle {
                        width: 350
                        height: 1
                        color: "lightgray"
                    }
                    //昵称与性别
                    RowLayout {
                        spacing: 15
                        RowLayout {
                            spacing: 10
                            Label {
                                id: labName
                                text: "昵   称"
                                font.family: "新宋体"
                                font.pixelSize: 14
                                color: "gray"
                            }
                            TextField {
                                id: fieldName
                                font.family: "新宋体"
                                onTextChanged: {
                                    okBtn.enabled = true
                                }

                                background: Rectangle {
                                    implicitWidth: 108
                                    implicitHeight: 22
                                    radius: 3
                                    color: "white"
                                    border.color: fieldName.hovered ? "#1583dd" : "lightgray"
                                    border.width: fieldName.hovered ? 2 : 1
                                }
                            }
                        }
                        RowLayout {
                            spacing: 10
                            Label {
                                text: "性   别"
                                font.family: "新宋体"
                                font.pixelSize: 14
                                color: "gray"
                            }
                            //性别框
                            ComboBox {
                                id: sexCBox
                                font.family: "新宋体"
                                font.pixelSize: 15
                                currentIndex: -1
                                editable: false
                                onCurrentTextChanged: {
                                    okBtn.enabled = true
                                }

                                contentItem: Label {
                                    leftPadding: 10
                                    topPadding: 3
                                    text: sexCBox.displayText
                                    font: sexCBox.font
                                    color: "black"
                                }

                                model: ["男", "女"]

                                delegate: ItemDelegate {
                                    id: sexDet
                                    width: 104
                                    height: 20

                                    background: Rectangle {
                                        color: sexDet.highlighted ? "#378fcf" : "#ffffff"
                                        border.color: "white"
                                        Text {
                                            y: (parent.height - height) / 2
                                            x: 8
                                            text: modelData
                                            color: sexDet.highlighted ? "white" : "black"
                                            font: sexCBox.font
                                            elide: Text.ElideRight
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    highlighted: sexCBox.highlightedIndex === index
                                }

                                indicator: Canvas {
                                    id: sexCanvas
                                    x: sexCBox.width - width - sexCBox.rightPadding
                                    y: sexCBox.topPadding + (sexCBox.availableHeight - height) / 2
                                    width: 10
                                    height: 10
                                    contextType: "2d"

                                    Connections {
                                        target: sexPop
                                        onVisibleChanged: {
                                            sexCanvas.requestPaint()
                                        }
                                    }

                                    onPaint: {
                                        var context = getContext("2d")
                                        context.reset()
                                        if (!sexPop.visible) {
                                            context.moveTo(0, height / 4)
                                            context.lineTo(width, height / 4)
                                            context.lineTo(width / 2,
                                                           height / 4 * 3)
                                        } else {
                                            context.moveTo(width / 2,
                                                           height / 4)
                                            context.lineTo(0, height / 4 * 3)
                                            context.lineTo(width,
                                                           height / 4 * 3)
                                        }
                                        context.closePath()
                                        context.fillStyle = "#434343"
                                        context.fill()
                                    }
                                }

                                background: Rectangle {
                                    implicitWidth: 108
                                    implicitHeight: 22
                                    border.color: sexCBox.hovered ? "#1583dd" : "lightgray"
                                    border.width: sexCBox.hovered ? 2 : 1
                                    radius: 3
                                }

                                popup: Popup {
                                    id: sexPop
                                    y: sexCBox.height + 2
                                    width: 108
                                    implicitHeight: contentHeight + 4
                                    padding: 2
                                    clip: true
                                    contentItem: ListView {
                                        clip: true
                                        implicitHeight: contentHeight
                                        model: sexCBox.delegateModel
                                        currentIndex: sexCBox.highlightedIndex
                                        delegate: sexCBox.delegate
                                    }
                                    background: Rectangle {
                                        color: "white"
                                        border.color: "#1583dd"
                                        border.width: 2
                                        radius: 2
                                    }
                                }
                            }
                        }
                    }
                    //生日与血型
                    RowLayout {
                        spacing: 15
                        RowLayout {
                            spacing: 10
                            Label {
                                id: labBir
                                text: "生   日"
                                font.family: "新宋体"
                                font.pixelSize: 14
                                color: "gray"
                            }
                            ComboBox {
                                id: birCBox
                                font.family: "新宋体"
                                font.pixelSize: 15
                                displayText: ageCbox.displayText + monthCbox.displayText
                                             + dayCbox.displayText
                                editable: false
                                // rightPadding: 30
                                contentItem: Label {
                                    leftPadding: 10
                                    topPadding: 3
                                    clip: true
                                    text: birCBox.displayText
                                    font: birCBox.font
                                    color: "black"
                                    onTextChanged: {
                                        okBtn.enabled = true
                                    }
                                }
                                indicator: null
                                background: Rectangle {
                                    implicitWidth: 108
                                    implicitHeight: 22
                                    border.color: "lightgray"
                                    radius: 3
                                }
                                popup: Popup {
                                    id: birPop
                                    y: birCBox.height + 2
                                    width: 232
                                    height: 48
                                    implicitHeight: contentHeight
                                    padding: 1
                                    clip: true //12b7f5

                                    contentItem: Item {
                                        width: birPop.width
                                        height: birPop.height
                                        RowLayout {
                                            x: 10
                                            y: 20
                                            spacing: 6
                                            //年份复合框
                                            ComboBox {
                                                id: ageCbox
                                                font.family: "新宋体"
                                                font.pixelSize: 14
                                                currentIndex: -1
                                                displayText: currentText + "年"
                                                editable: false
                                                contentItem: Item {
                                                    width: 35
                                                    height: 20
                                                    clip: true
                                                    Label {
                                                        width: 35
                                                        height: 20
                                                        leftPadding: 3
                                                        topPadding: 3
                                                        clip: true
                                                        text: ageCbox.displayText
                                                        font: ageCbox.font
                                                        color: "black"
                                                    }
                                                }

                                                model: yearModel

                                                delegate: ItemDelegate {
                                                    id: ageDet
                                                    width: 56
                                                    height: 20
                                                    leftPadding: 0
                                                    background: Rectangle {
                                                        color: ageDet.highlighted ? "#378fcf" : "#ffffff"
                                                        border.color: "white"
                                                        Text {
                                                            y: (parent.height - height) / 2
                                                            x: 5
                                                            text: modelData + "年"
                                                            color: ageDet.highlighted ? "white" : "black"
                                                            font: ageCbox.font
                                                            clip: true
                                                            verticalAlignment: Text.AlignVCenter
                                                        }
                                                    }
                                                    highlighted: ageCbox.highlightedIndex === index
                                                }

                                                indicator: Canvas {
                                                    id: ageCanvas
                                                    x: ageCbox.width - width - ageCbox.rightPadding
                                                    y: ageCbox.topPadding
                                                       + (ageCbox.availableHeight - height) / 2
                                                    width: 10
                                                    height: 10

                                                    Connections {
                                                        target: agePop
                                                        onVisibleChanged: {
                                                            ageCanvas.requestPaint()
                                                        }
                                                    }

                                                    onPaint: {
                                                        var context = getContext(
                                                                    "2d")
                                                        context.reset()
                                                        if (!agePop.visible) {
                                                            context.moveTo(
                                                                        0,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        width,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        width / 2,
                                                                        height / 4 * 3)
                                                            context.closePath()
                                                        } else {
                                                            context.moveTo(
                                                                        width / 2,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        0,
                                                                        height / 4 * 3)
                                                            context.lineTo(
                                                                        width,
                                                                        height / 4 * 3)
                                                        }
                                                        context.closePath()
                                                        context.fillStyle = "#434343"
                                                        context.fill()
                                                    }
                                                }

                                                background: Rectangle {
                                                    implicitWidth: 58
                                                    implicitHeight: 20
                                                    border.color: ageCbox.hovered ? "#1583dd" : "lightgray"
                                                    border.width: ageCbox.hovered ? 2 : 1
                                                    radius: 3
                                                }

                                                popup: Popup {
                                                    id: agePop
                                                    y: ageCbox.height + 2
                                                    width: 60
                                                    height: 124
                                                    padding: 2
                                                    clip: true

                                                    contentItem: ListView {
                                                        clip: true
                                                        implicitHeight: contentHeight
                                                        model: ageCbox.delegateModel
                                                        currentIndex: ageCbox.highlightedIndex
                                                        delegate: ageCbox.delegate
                                                    }
                                                    background: Rectangle {
                                                        color: "white"
                                                        border.color: "#1583dd"
                                                        border.width: 2
                                                        radius: 2
                                                    }
                                                }
                                            }
                                            //月份复合框
                                            ComboBox {
                                                id: monthCbox
                                                font.family: "新宋体"
                                                font.pixelSize: 14
                                                currentIndex: -1
                                                displayText: currentText + "月"
                                                editable: false
                                                onCurrentIndexChanged: {
                                                    console.log("monthCbox index changed")
                                                    var index = currentIndex.toString()
                                                    console.log("index=",
                                                                index & 1)
                                                    switch (true) {
                                                    case index === 1:
                                                        var year = parseInt(
                                                                    ageCbox.displayText.substring(
                                                                        0, 4))
                                                        if ((year % 4 == 0
                                                             && year % 100 != 0)
                                                                || year % 400 == 0) {
                                                            var day = 29
                                                        } else {
                                                            day = 28
                                                        }
                                                        break
                                                    case (index & 1) != 1:
                                                        day = 31
                                                        break
                                                    case (index & 1) == 1:
                                                        day = 30
                                                        break
                                                    default:
                                                        console.log("default")
                                                        day = 30
                                                    }
                                                    var temp = dayCbox.count
                                                    if (day !== temp) {
                                                        if (day > temp) {
                                                            temp = dayCbox.currentIndex
                                                        } else {
                                                            temp = 0
                                                        }
                                                        dayModel.clear()
                                                        for (var i = 1; i <= day; ++i) {
                                                            dayModel.append({
                                                                                "modelData": i
                                                                            })
                                                        }
                                                        console.log(" dayCbox.currentIndex =",
                                                                    temp)
                                                        dayCbox.currentIndex = temp //索引调整
                                                    }
                                                }

                                                contentItem: Item {
                                                    width: 36
                                                    height: 20
                                                    clip: true
                                                    Label {
                                                        width: 36
                                                        height: 20
                                                        leftPadding: 3
                                                        topPadding: 3
                                                        clip: true
                                                        text: monthCbox.displayText
                                                        font: monthCbox.font
                                                        color: "black"
                                                    }
                                                }

                                                model: monthModel

                                                delegate: ItemDelegate {
                                                    id: monthDet
                                                    width: 56
                                                    height: 20

                                                    background: Rectangle {
                                                        color: monthDet.highlighted ? "#378fcf" : "#ffffff"
                                                        border.color: "white"
                                                        Text {
                                                            y: (parent.height - height) / 2
                                                            x: 8
                                                            text: modelData + "月"
                                                            color: monthDet.highlighted ? "white" : "black"
                                                            font: monthCbox.font
                                                            clip: true
                                                            verticalAlignment: Text.AlignVCenter
                                                        }
                                                    }
                                                    highlighted: monthCbox.highlightedIndex
                                                                 === index
                                                }

                                                indicator: Canvas {
                                                    id: monCanvas
                                                    x: monthCbox.width - width
                                                       - monthCbox.rightPadding
                                                    y: monthCbox.topPadding
                                                       + (monthCbox.availableHeight - height) / 2
                                                    width: 10
                                                    height: 10

                                                    Connections {
                                                        target: monPop
                                                        onVisibleChanged: {
                                                            monCanvas.requestPaint()
                                                        }
                                                    }

                                                    onPaint: {
                                                        var context = getContext(
                                                                    "2d")
                                                        context.reset()
                                                        context.reset()
                                                        if (!monPop.visible) {
                                                            context.moveTo(
                                                                        0,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        width,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        width / 2,
                                                                        height / 4 * 3)
                                                            context.closePath()
                                                        } else {
                                                            context.moveTo(
                                                                        width / 2,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        0,
                                                                        height / 4 * 3)
                                                            context.lineTo(
                                                                        width,
                                                                        height / 4 * 3)
                                                        }
                                                        context.closePath()
                                                        context.fillStyle = "#434343"
                                                        context.fill()
                                                    }
                                                }

                                                background: Rectangle {
                                                    implicitWidth: 58
                                                    implicitHeight: 20
                                                    border.color: monthCbox.hovered ? "#1583dd" : "lightgray"
                                                    border.width: monthCbox.hovered ? 2 : 1
                                                    radius: 3
                                                }

                                                popup: Popup {
                                                    id: monPop
                                                    y: monthCbox.height + 2
                                                    width: 60
                                                    height: 124
                                                    padding: 2
                                                    clip: true
                                                    contentItem: ListView {
                                                        clip: true
                                                        implicitHeight: contentHeight
                                                        model: monthCbox.delegateModel
                                                        currentIndex: monthCbox.highlightedIndex
                                                        delegate: monthCbox.delegate
                                                    }
                                                    background: Rectangle {
                                                        color: "white"
                                                        border.color: "#1583dd"
                                                        border.width: 2
                                                        radius: 2
                                                    }
                                                }
                                            }
                                            //日复合框
                                            ComboBox {
                                                id: dayCbox
                                                font.family: "新宋体"
                                                font.pixelSize: 14
                                                currentIndex: -1
                                                displayText: currentText + "日"
                                                editable: false
                                                contentItem: Item {
                                                    width: 36
                                                    height: 20
                                                    clip: true
                                                    Label {
                                                        width: 36
                                                        height: 20
                                                        leftPadding: 3
                                                        topPadding: 3
                                                        clip: true
                                                        text: dayCbox.displayText
                                                        font: dayCbox.font
                                                        color: "black"
                                                    }
                                                }

                                                model: dayModel

                                                delegate: ItemDelegate {
                                                    id: dayDet
                                                    width: 56
                                                    height: 20

                                                    background: Rectangle {
                                                        color: dayDet.highlighted ? "#378fcf" : "#ffffff"
                                                        border.color: "white"
                                                        Text {
                                                            y: (parent.height - height) / 2
                                                            x: 8
                                                            text: modelData + "日"
                                                            color: dayDet.highlighted ? "white" : "black"
                                                            font: dayCbox.font
                                                            clip: true
                                                            verticalAlignment: Text.AlignVCenter
                                                        }
                                                    }
                                                    highlighted: dayCbox.highlightedIndex === index
                                                }

                                                indicator: Canvas {
                                                    id: dayCanvas
                                                    x: dayCbox.width - width - dayCbox.rightPadding
                                                    y: dayCbox.topPadding
                                                       + (dayCbox.availableHeight - height) / 2
                                                    width: 10
                                                    height: 10
                                                    Connections {
                                                        target: dayPop
                                                        onVisibleChanged: {
                                                            dayCanvas.requestPaint()
                                                        }
                                                    }
                                                    onPaint: {
                                                        var context = getContext(
                                                                    "2d")
                                                        context.reset()
                                                        if (!dayPop.visible) {
                                                            context.moveTo(
                                                                        0,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        width,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        width / 2,
                                                                        height / 4 * 3)
                                                            context.closePath()
                                                        } else {
                                                            context.moveTo(
                                                                        width / 2,
                                                                        height / 4)
                                                            context.lineTo(
                                                                        0,
                                                                        height / 4 * 3)
                                                            context.lineTo(
                                                                        width,
                                                                        height / 4 * 3)
                                                        }
                                                        context.closePath()
                                                        context.fillStyle = "#434343"
                                                        context.fill()
                                                    }
                                                }

                                                background: Rectangle {
                                                    implicitWidth: 58
                                                    implicitHeight: 20
                                                    border.color: dayCbox.hovered ? "#1583dd" : "lightgray"
                                                    border.width: dayCbox.hovered ? 2 : 1
                                                    radius: 3
                                                }

                                                popup: Popup {
                                                    id: dayPop
                                                    y: dayCbox.height + 2
                                                    width: 60
                                                    height: 124
                                                    padding: 2
                                                    clip: true
                                                    contentItem: ListView {
                                                        clip: true
                                                        implicitHeight: contentHeight
                                                        model: dayCbox.delegateModel
                                                        currentIndex: dayCbox.highlightedIndex
                                                        delegate: dayCbox.delegate
                                                    }
                                                    background: Rectangle {
                                                        color: "white"
                                                        border.color: "#1583dd"
                                                        border.width: 2
                                                        radius: 2
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    background: Rectangle {
                                        border.color: "#0093d1"
                                    }
                                }
                            }
                        }
                        //血型
                        RowLayout {
                            spacing: 10
                            Label {
                                text: "血   型"
                                font.family: "新宋体"
                                font.pixelSize: 14
                                color: "gray"
                            }
                            //性别框
                            ComboBox {
                                id: bloodCBox
                                font.family: "新宋体"
                                font.pixelSize: 15
                                currentIndex: -1
                                editable: false
                                onCurrentTextChanged: {
                                    okBtn.enabled = true
                                }

                                contentItem: Label {
                                    leftPadding: 10
                                    topPadding: 3
                                    text: bloodCBox.displayText
                                    font: bloodCBox.font
                                    color: "black"
                                }

                                model: ["", "A型", "B型", "O型", "AB型", "其它血型"]

                                delegate: ItemDelegate {
                                    id: bloodDet
                                    width: 104
                                    height: 20

                                    background: Rectangle {
                                        color: bloodDet.highlighted ? "#378fcf" : "#ffffff"
                                        border.color: "white"
                                        Text {
                                            y: (parent.height - height) / 2
                                            x: 8
                                            text: modelData
                                            color: bloodDet.highlighted ? "white" : "black"
                                            font: bloodCBox.font
                                            elide: Text.ElideRight
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    highlighted: bloodCBox.highlightedIndex === index
                                }

                                indicator: Canvas {
                                    id: bloodCanvas
                                    x: bloodCBox.width - width - bloodCBox.rightPadding
                                    y: bloodCBox.topPadding
                                       + (bloodCBox.availableHeight - height) / 2
                                    width: 10
                                    height: 10
                                    contextType: "2d"

                                    Connections {
                                        target: bloodPop
                                        onVisibleChanged: {
                                            bloodCanvas.requestPaint()
                                        }
                                    }

                                    onPaint: {
                                        context.reset()
                                        if (!bloodPop.visible) {
                                            context.moveTo(0, height / 4)
                                            context.lineTo(width, height / 4)
                                            context.lineTo(width / 2,
                                                           height / 4 * 3)
                                            context.closePath()
                                        } else {
                                            context.moveTo(width / 2,
                                                           height / 4)
                                            context.lineTo(0, height / 4 * 3)
                                            context.lineTo(width,
                                                           height / 4 * 3)
                                        }
                                        context.closePath()
                                        context.fillStyle = "#434343"
                                        context.fill()
                                    }
                                }

                                background: Rectangle {
                                    implicitWidth: 108
                                    implicitHeight: 22
                                    border.color: bloodCBox.hovered ? "#1583dd" : "lightgray"
                                    border.width: bloodCBox.hovered ? 2 : 1
                                    radius: 3
                                }

                                popup: Popup {
                                    id: bloodPop
                                    y: bloodCBox.height + 2
                                    width: 108
                                    implicitHeight: contentHeight + 4
                                    padding: 2
                                    clip: true
                                    contentItem: ListView {
                                        clip: true
                                        implicitHeight: contentHeight
                                        model: bloodCBox.delegateModel
                                        currentIndex: bloodCBox.highlightedIndex
                                        delegate: bloodCBox.delegate
                                    }
                                    background: Rectangle {
                                        color: "white"
                                        border.color: "#1583dd"
                                        border.width: 2
                                        radius: 2
                                    }
                                }
                            }
                        }
                    }
                    //签名
                    RowLayout {
                        spacing: 10
                        Label {
                            text: "签   名"
                            font.family: "新宋体"
                            font.pixelSize: 14
                            color: "gray"
                        }
                        //签名编辑区域
                        //用Item打破布局的约束
                        Item {
                            width: 290
                            height: 44
                            ScrollView {
                                id: viewTag
                                anchors.fill: parent
                                hoverEnabled: true
                                TextArea {
                                    id: editTag
                                    padding: 4
                                    wrapMode: TextEdit.WrapAnywhere
                                    font.family: "新宋体"
                                    font.pixelSize: 13
                                    onTextChanged: {
                                        okBtn.enabled = true
                                    }
                                }
                                background: Rectangle {
                                    border.width: viewTag.hovered ? 2 : 1
                                    border.color: viewTag.hovered ? "#1583dd" : "lightgray"
                                }
                            }
                        }
                    }
                }
            }
            //教育经历
            Label {
                id: cont1
                x: 15
                y: 188
                text: "填写教育经历"
                font.family: "黑体"
                font.pixelSize: 13
                font.wordSpacing: 0
                font.bold: true
            }
            //添加教育按钮
            Button {
                id: addEduExperBtn
                x: 269
                y: 188
                width: 85
                height: 15
                onClicked: {
                    console.log("addEduExperBtn clicked")
                    //提示等待修改完成
                    if (!eduView.isReadyToWrite) {
                        if (eduTipComp.status === Component.Ready) {
                            console.log("eduTipWinComp status is Component.Ready")
                            var tipObj = eduTipComp.createObject(win)
                            if (!tipObj) {
                                console.log("warning: eduTipWin instantiation failed")
                                return
                            }
                            tipObj.show()
                        } else
                            eduView.isReadyToWrite = true
                        return
                    }

                    eduView.isReadyToWrite = false
                    if (addEduInfoComp.status === Component.Ready) {
                        console.log("addEduInfoComp status is Component.Ready")
                        var obj = addEduInfoComp.createObject(win)
                        if (!obj) {
                            eduView.isReadyToWrite = true
                            console.log(" addEduInfoComp object creation fails")
                            return
                        }
                        cwin = obj //赋值用于清理资源
                        obj.show()
                        obj.raise()
                        obj.requestActivate()
                    } else
                        eduView.isReadyToWrite = true
                }
                background: Label {
                    width: 85
                    height: 15
                    text: "+添加教育经历"
                    font.family: "宋体"
                    font.pointSize: 10
                    color: editMaterialBtn.hovered ? Qt.lighter(
                                                         "#009bdb",
                                                         1.15) : "#009bdb"
                }
            }
            //分割条
            Rectangle {
                x: 15
                y: 210
                width: 350
                height: 1
                color: "lightgray"
            }
            //添加教育标签
            Item {
                id: labEdu
                visible: !eduView.visible
                y: 211
                width: 384
                height: 65
                Label {
                    x: 112
                    y: 18
                    text: "尚未添加教育经历"
                    font.family: "新宋体"
                    font.pixelSize: 14
                    color: "gray"
                }
            }
            //教育经验内容
            ListView {
                y: 211
                property int realCount: 0
                property bool isReadyToWrite: true
                signal clicked(int index)
                signal alter(var item)
                signal del(var item)
                id: eduView
                interactive: false
                visible: false
                model: eduModel
                width: 384
                height: 65
                //修改处理
                onAlter: {
                    console.log("alter education information")
                    //提示等待修改完成
                    if (!eduView.isReadyToWrite) {
                        if (eduTipComp.status === Component.Ready) {
                            console.log("eduTipWinComp status is Component.Ready")
                            var tipObj = eduTipComp.createObject(win)
                            if (!tipObj) {
                                console.log("warning: eduTipWin instantiation failed")
                                return
                            }
                            tipObj.show()
                        } else
                            eduView.isReadyToWrite = true
                        return
                    }
                    eduView.isReadyToWrite = false
                    if (addEduInfoComp.status === Component.Ready) {
                        console.log("addEduInfoComp status is Component.Ready")
                        var obj = addEduInfoComp.createObject(win)
                        if (!obj) {
                            eduView.isReadyToWrite = true
                            console.log(" addEduInfoComp object creation fails")
                            return
                        }
                        obj.fieldSchool.text = item.school
                        var detail = item.detail
                        detail = detail.split(",").filter(function (x) {
                            return x !== ""
                        })
                        var f = item.flags
                        var count = 0
                        if (f.indexOf("1") !== -1) {
                            obj.fieldInstitute.text = detail[count++]
                        }
                        if (f.indexOf("2") !== -1) {
                            var temp = detail[count].substring(
                                        0, detail[count].length - 1)
                            obj.goCBox.currentIndex = obj.goCBox.find(temp)
                            ++count
                        }
                        if (f.indexOf("3") !== -1) {
                            obj.degreeCBox.currentIndex = obj.degreeCBox.find(
                                        detail[count++])
                        }
                        obj.item = item //赋值代表是修改对话框，会在其关闭时复制回来
                        obj.show()
                    } else
                        eduView.isReadyToWrite = true
                }
                //删除处理
                onDel: {
                    console.log("education information deletion")
                    var index = item.index

                    var length = eduModel.count
                    for (var i = index + 1; i < length; ++i) {
                        eduModel.setProperty(i, "r_index", i - 1)
                    }
                    eduModel.remove(index)
                    okBtn.enabled = true
                }

                //点击大控件处理
                onClicked: {
                    console.log("eduView clicked(int index) signal")
                    var conts = contentItem.children
                    var length = conts.length
                    for (var i = 0; i < length; ++i) {
                        var temp = conts[i]
                        if (temp === undefined) {
                            console.log("warning:eduView  acquired a undefined child item")
                        } else {
                            if (temp.index !== index) {
                                temp.isChecked = false
                            }
                        }
                    }
                }

                //可视化控制
                onCountChanged: {
                    if (count > 0)
                        eduView.visible = true
                    else
                        eduView.visible = false
                }

                delegate: EduInfo {
                    id: eduItem
                    school: r_school
                    detail: r_detail
                    flags: r_flags
                    index: r_index
                    onClicked: eduView.clicked(index)
                    //传对象给视图处理添加删除事件
                    addBtn.onClicked: {
                        eduView.alter(eduItem)
                    }
                    delBtn.onClicked: {
                        eduView.del(eduItem)
                    }
                }
            }
            //更多资料标签
            Label {
                id: labMoreInfo
                x: 15
                y: eduView.visible ? (eduView.y + eduView.height + 20) : labEdu.y + 85
                text: "更多资料"
                font.family: "黑体"
                font.pixelSize: 13
                font.wordSpacing: 0
                font.bold: true
            }
            //更多资料内容
            ColumnLayout {
                id: cont2
                x: 15
                y: labMoreInfo.y + labMoreInfo.height + 2
                spacing: 10
                Rectangle {
                    width: 350
                    height: 1
                    color: "lightgray"
                }
                //职业
                RowLayout {
                    spacing: 10
                    Label {
                        text: "职   业"
                        font.family: "新宋体"
                        font.pixelSize: 14
                        color: "gray"
                    }
                    //职业框
                    ComboBox {
                        id: professCBox
                        font.family: "新宋体"
                        font.pixelSize: 15
                        currentIndex: -1
                        editable: false
                        onCurrentTextChanged: {
                            okBtn.enabled = true
                        }
                        contentItem: Label {
                            leftPadding: 10
                            topPadding: 3
                            text: professCBox.displayText
                            font: professCBox.font
                            color: "black"
                        }

                        model: ["计算机/互联网/通信", "生产/工艺/制造", "医疗/治理/制药", "金融/银行/投资/保险", "商业/服务业/个体经营", "文化/广告/传媒", "娱乐/艺术/表演", "律师/法务", "教育/培训", "公务员/行政/事业单位", "模特", "空姐", "学生", "其他"]

                        delegate: ItemDelegate {
                            id: proDet
                            width: 284
                            height: 22

                            background: Rectangle {
                                color: proDet.highlighted ? "#378fcf" : "#ffffff"
                                border.color: "white"
                                Text {
                                    y: (parent.height - height) / 2
                                    x: 8
                                    text: modelData
                                    color: proDet.highlighted ? "white" : "black"
                                    font: professCBox.font
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                            highlighted: professCBox.highlightedIndex === index
                        }

                        indicator: Canvas {
                            id: proCanvas
                            x: professCBox.width - width - professCBox.rightPadding
                            y: professCBox.topPadding + (professCBox.availableHeight - height) / 2
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
                                    context.lineTo(width / 2, height / 4 * 3)
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
                            implicitWidth: 288
                            implicitHeight: 22
                            border.color: professCBox.hovered ? "#1583dd" : "lightgray"
                            border.width: professCBox.hovered ? 2 : 1
                            radius: 3
                        }

                        popup: Popup {
                            id: professPop
                            y: professCBox.height + 2
                            width: 288
                            height: 136
                            padding: 2
                            clip: true
                            contentItem: ListView {
                                clip: true
                                implicitHeight: contentHeight
                                model: professCBox.delegateModel
                                currentIndex: professCBox.highlightedIndex
                                delegate: professCBox.delegate
                            }
                            background: Rectangle {
                                color: "white"
                                border.color: "#1583dd"
                                border.width: 2
                                radius: 2
                            }
                        }
                    }
                }
                //公司
                RowLayout {
                    spacing: 10
                    Label {
                        id: labCorporation
                        text: "公   司"
                        font.family: "新宋体"
                        font.pixelSize: 14
                        color: "gray"
                    }
                    TextField {
                        id: fieldCorporation
                        font.family: "新宋体"
                        onTextChanged: {
                            okBtn.enabled = true
                        }

                        background: Rectangle {
                            implicitWidth: 288
                            implicitHeight: 22
                            radius: 3
                            color: "white"
                            border.color: fieldCorporation.hovered ? "#1583dd" : "lightgray"
                            border.width: fieldCorporation.hovered ? 2 : 1
                        }
                    }
                }
                //所在地
                RowLayout {
                    spacing: 2
                    Label {
                        id: labWhere
                        text: "所在地区"
                        font.family: "新宋体"
                        font.pixelSize: 14
                        color: "gray"
                    }
                    ComboBox {
                        id: whereCBox
                        font.family: "新宋体"
                        font.pixelSize: 15
                        currentIndex: -1
                        editable: false
                        model: null
                        displayText: where1Cbox.displayText + " " + where2Cbox.displayText + " "
                                     + where3Cbox.displayText + " " + where4Cbox.displayText
                        onDisplayTextChanged: {
                            okBtn.enabled = true
                        }
                        contentItem: Label {
                            leftPadding: 10
                            topPadding: 3
                            clip: true
                            text: whereCBox.displayText
                            font: whereCBox.font
                            color: "black"
                        }
                        indicator: null
                        background: Rectangle {
                            implicitWidth: 288
                            implicitHeight: 22
                            border.color: "lightgray"
                            radius: 3
                        }
                        popup: Popup {
                            id: wherePop
                            y: whereCBox.height + 2
                            width: 232
                            height: 60
                            implicitHeight: contentHeight
                            padding: 1
                            clip: true //12b7f5
                            //58 20 6  11 20
                            contentItem: Item {
                                width: wherePop.width
                                height: wherePop.height
                                GridLayout {
                                    x: 15
                                    y: 4
                                    columns: 2
                                    columnSpacing: 24
                                    rowSpacing: 6
                                    //所在地1
                                    ComboBox {
                                        id: where1Cbox

                                        font.family: "新宋体"
                                        font.pixelSize: 14
                                        currentIndex: -1
                                        editable: false
                                        displayText: currentText
                                        textRole: "name"
                                        onCurrentIndexChanged: {
                                            var index = currentIndex

                                            if (index === -1) {
                                                where1Cbox.currentIndex = -1
                                                where2Cbox.enabled = false
                                                provinceModel1.clear()
                                                where3Cbox.enabled = false
                                                cityModel1.clear()
                                                where4Cbox.enabled = false
                                                countyModel1.clear()
                                            } else {
                                                where2Cbox.enabled = true
                                                where3Cbox.enabled = false
                                                where4Cbox.enabled = false

                                                var temp
                                                var length
                                                var i
                                                temp = indivadualWin.countryModel.get(
                                                            index).id
                                                length = indivadualWin.provinceModel.count
                                                //添加省数据到模型
                                                provinceModel1.clear()
                                                for (i = 0; i < length; ++i) {
                                                    if (indivadualWin.provinceModel.get(
                                                                i).fid === temp) {
                                                        provinceModel1.append({
                                                                                  "id": indivadualWin.provinceModel.get(i).id,
                                                                                  "name": indivadualWin.provinceModel.get(i).name,
                                                                                  "fid": temp
                                                                              })
                                                    }
                                                }
                                            }
                                        }
                                        onEnabledChanged: {
                                            where1Canvas.requestPaint()
                                        }

                                        contentItem: Item {
                                            width: 35
                                            height: 20
                                            clip: true
                                            Label {
                                                width: 35
                                                height: 20
                                                leftPadding: 3
                                                topPadding: 3
                                                clip: true
                                                text: where1Cbox.displayText
                                                font: where1Cbox.font
                                                color: "black"
                                            }
                                        }

                                        model: indivadualWin.countryModel

                                        delegate: ItemDelegate {
                                            id: where1Det
                                            width: 84
                                            height: 18
                                            leftPadding: 0
                                            background: Rectangle {
                                                color: where1Det.highlighted ? "#378fcf" : "#ffffff"
                                                border.color: "white"
                                                Text {
                                                    y: (parent.height - height) / 2
                                                    x: 5
                                                    text: name
                                                    color: where1Det.highlighted ? "white" : "black"
                                                    font: where1Cbox.font
                                                    clip: true
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                            highlighted: where1Cbox.highlightedIndex === index
                                        }

                                        indicator: Canvas {
                                            id: where1Canvas
                                            x: where1Cbox.width - width - where1Cbox.rightPadding
                                            y: where1Cbox.topPadding
                                               + (where1Cbox.availableHeight - height) / 2
                                            width: 10
                                            height: 10

                                            Connections {
                                                target: where1Pop
                                                onVisibleChanged: {
                                                    where1Canvas.requestPaint()
                                                }
                                            }
                                            onPaint: {
                                                var context = getContext("2d")
                                                context.reset()
                                                if (!where1Pop.visible) {
                                                    context.moveTo(0,
                                                                   height / 4)
                                                    context.lineTo(width,
                                                                   height / 4)
                                                    context.lineTo(
                                                                width / 2,
                                                                height / 4 * 3)
                                                    context.closePath()
                                                } else {
                                                    context.moveTo(width / 2,
                                                                   height / 4)
                                                    context.lineTo(
                                                                0,
                                                                height / 4 * 3)
                                                    context.lineTo(
                                                                width,
                                                                height / 4 * 3)
                                                }
                                                context.closePath()
                                                if (where1Cbox.enabled)
                                                    context.fillStyle = "#434343"
                                                else
                                                    context.fillStyle = Qt.lighter(
                                                                "#434343", 1.15)
                                                context.fill()
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 88
                                            implicitHeight: 18
                                            border.color: where1Cbox.enabled ? where1Cbox.hovered ? "#1583dd" : "lightgray" : Qt.lighter("lightgray", 1.10)
                                            border.width: (where1Cbox.enabled
                                                           && where1Cbox.hovered) ? 2 : 1
                                            radius: 3
                                        }

                                        popup: Popup {
                                            id: where1Pop
                                            y: where1Cbox.height + 2
                                            width: 88
                                            height: contentHeight > 112 ? 112 : (contentHeight > 0 ? contentHeight + 4 : 0)
                                            padding: 2
                                            clip: true

                                            contentItem: ListView {
                                                clip: true
                                                implicitHeight: contentHeight
                                                model: where1Cbox.delegateModel
                                                currentIndex: where1Cbox.highlightedIndex
                                                delegate: where1Cbox.delegate
                                            }
                                            background: Rectangle {
                                                color: "white"
                                                border.color: "#1583dd"
                                                border.width: 2
                                                radius: 2
                                            }
                                        }
                                    }
                                    //所在地2
                                    ComboBox {
                                        id: where2Cbox
                                        displayText: currentText
                                        font.family: "新宋体"
                                        font.pixelSize: 14
                                        currentIndex: -1
                                        editable: false

                                        textRole: "name"

                                        onCurrentIndexChanged: {
                                            var index = currentIndex
                                            if (index === -1) {
                                                where2Cbox.currentIndex = -1
                                                where3Cbox.enabled = false
                                                cityModel1.clear()
                                                where4Cbox.enabled = false
                                                countyModel1.clear()
                                            } else {
                                                where3Cbox.enabled = true
                                                where4Cbox.enabled = false

                                                var temp
                                                var length
                                                var i
                                                temp = provinceModel1.get(
                                                            index).id
                                                length = indivadualWin.cityModel.count
                                                //添加城市数据到模型
                                                cityModel1.clear()
                                                var id
                                                for (i = 0; i < length; ++i) {
                                                    if (indivadualWin.cityModel.get(
                                                                i).fid === temp) {
                                                        id = indivadualWin.cityModel.get(
                                                                    i).id
                                                        cityModel1.append({
                                                                              "id": id,
                                                                              "name": indivadualWin.cityModel.get(i).name,
                                                                              "fid": temp
                                                                          })
                                                    }
                                                }
                                                //如果长度为1 传县区数据给它，其代表自治市
                                                if (cityModel1.count == 1) {
                                                    where4Cbox.isNeed = false
                                                    length = indivadualWin.countyModel.count
                                                    //添加县区数据到模型
                                                    cityModel1.clear()
                                                    for (i = 0; i < length; ++i) {
                                                        if (indivadualWin.countyModel.get(
                                                                    i).fid === id) {
                                                            cityModel1.append({
                                                                                  "id": indivadualWin.countyModel.get(i).id,
                                                                                  "name": indivadualWin.countyModel.get(i).name,
                                                                                  "fid": id
                                                                              })
                                                        }
                                                    }
                                                } else {
                                                    where4Cbox.isNeed = true
                                                }
                                            }
                                        }
                                        onEnabledChanged: {
                                            where2Canvas.requestPaint()
                                        }

                                        contentItem: Item {
                                            width: 35
                                            height: 20
                                            clip: true
                                            Label {
                                                width: 35
                                                height: 20
                                                leftPadding: 3
                                                topPadding: 3
                                                clip: true
                                                text: where2Cbox.displayText
                                                font: where2Cbox.font
                                                color: "black"
                                            }
                                        }

                                        model: provinceModel1

                                        delegate: ItemDelegate {
                                            id: where2Det
                                            width: 84
                                            height: 18
                                            leftPadding: 0
                                            background: Rectangle {
                                                color: where2Det.highlighted ? "#378fcf" : "#ffffff"
                                                border.color: "white"
                                                Text {
                                                    y: (parent.height - height) / 2
                                                    x: 5
                                                    text: name
                                                    color: where2Det.highlighted ? "white" : "black"
                                                    font: where2Cbox.font
                                                    clip: true
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                            highlighted: where2Cbox.highlightedIndex === index
                                        }

                                        indicator: Canvas {
                                            id: where2Canvas
                                            x: where2Cbox.width - width - where2Cbox.rightPadding
                                            y: where2Cbox.topPadding
                                               + (where2Cbox.availableHeight - height) / 2
                                            width: 10
                                            height: 10

                                            Connections {
                                                target: where2Pop
                                                onVisibleChanged: {
                                                    where2Canvas.requestPaint()
                                                }
                                            }

                                            onPaint: {
                                                var context = getContext("2d")
                                                context.reset()
                                                if (!where2Pop.visible) {
                                                    context.moveTo(0,
                                                                   height / 4)
                                                    context.lineTo(width,
                                                                   height / 4)
                                                    context.lineTo(
                                                                width / 2,
                                                                height / 4 * 3)
                                                    context.closePath()
                                                } else {
                                                    context.moveTo(width / 2,
                                                                   height / 4)
                                                    context.lineTo(
                                                                0,
                                                                height / 4 * 3)
                                                    context.lineTo(
                                                                width,
                                                                height / 4 * 3)
                                                }
                                                context.closePath()
                                                if (where2Cbox.enabled)
                                                    context.fillStyle = "#434343"
                                                else
                                                    context.fillStyle = Qt.lighter(
                                                                "#434343", 1.15)
                                                context.fill()
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 88
                                            implicitHeight: 18
                                            border.color: where2Cbox.enabled ? where2Cbox.hovered ? "#1583dd" : "lightgray" : Qt.lighter("lightgray", 1.10)
                                            border.width: (where2Cbox.enabled
                                                           && where2Cbox.hovered) ? 2 : 1
                                            radius: 3
                                        }

                                        popup: Popup {
                                            id: where2Pop
                                            y: where2Cbox.height + 2
                                            width: 88
                                            height: contentHeight > 112 ? 112 : (contentHeight > 0 ? contentHeight + 4 : 0)
                                            padding: 2
                                            clip: true

                                            contentItem: ListView {
                                                clip: true
                                                implicitHeight: contentHeight
                                                model: where2Cbox.delegateModel
                                                currentIndex: where2Cbox.highlightedIndex
                                                delegate: where2Cbox.delegate
                                            }
                                            background: Rectangle {
                                                color: "white"
                                                border.color: "#1583dd"
                                                border.width: 2
                                                radius: 2
                                            }
                                        }
                                    }
                                    //所在地3
                                    ComboBox {
                                        id: where3Cbox
                                        displayText: currentText
                                        font.family: "新宋体"
                                        font.pixelSize: 14
                                        currentIndex: -1
                                        editable: false
                                        textRole: "name"

                                        onCurrentIndexChanged: {
                                            var index = currentIndex
                                            console.log("???")
                                            if (index === -1) {
                                                where3Cbox.currentIndex = -1
                                                where4Cbox.enabled = false
                                                countyModel1.clear()
                                            } else {
                                                where4Cbox.enabled = true
                                                if (!where4Cbox.isNeed) {
                                                    where4Cbox.enabled = false
                                                    return
                                                }
                                                var temp
                                                var i
                                                var length
                                                temp = cityModel1.get(index).id
                                                length = indivadualWin.countyModel.count

                                                //添加县区数据到模型
                                                countyModel1.clear()
                                                for (i = 0; i < length; ++i) {
                                                    if (indivadualWin.countyModel.get(
                                                                i).fid === temp) {
                                                        countyModel1.append({
                                                                                "id": indivadualWin.countyModel.get(i).id,
                                                                                "name": indivadualWin.countyModel.get(i).name,
                                                                                "fid": temp
                                                                            })
                                                    }
                                                }
                                            }
                                        }
                                        onEnabledChanged: {
                                            where3Canvas.requestPaint()
                                        }

                                        contentItem: Item {
                                            width: 35
                                            height: 20
                                            clip: true
                                            Label {
                                                width: 35
                                                height: 20
                                                leftPadding: 3
                                                topPadding: 3
                                                clip: true
                                                text: where3Cbox.displayText
                                                font: where3Cbox.font
                                                color: "black"
                                            }
                                        }

                                        model: cityModel1

                                        delegate: ItemDelegate {
                                            id: where3Det
                                            width: 84
                                            height: 18
                                            leftPadding: 0
                                            background: Rectangle {
                                                color: where3Det.highlighted ? "#378fcf" : "#ffffff"
                                                border.color: "white"
                                                Text {
                                                    y: (parent.height - height) / 2
                                                    x: 5
                                                    text: name
                                                    color: where3Det.highlighted ? "white" : "black"
                                                    font: where3Cbox.font
                                                    clip: true
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                            highlighted: where3Cbox.highlightedIndex === index
                                        }

                                        indicator: Canvas {
                                            id: where3Canvas
                                            x: where3Cbox.width - width - where3Cbox.rightPadding
                                            y: where3Cbox.topPadding
                                               + (where3Cbox.availableHeight - height) / 2
                                            width: 10
                                            height: 10

                                            Connections {
                                                target: where3Pop
                                                onVisibleChanged: {
                                                    where3Canvas.requestPaint()
                                                }
                                            }

                                            onPaint: {
                                                var context = getContext("2d")
                                                context.reset()
                                                if (!where3Pop.visible) {
                                                    context.moveTo(0,
                                                                   height / 4)
                                                    context.lineTo(width,
                                                                   height / 4)
                                                    context.lineTo(
                                                                width / 2,
                                                                height / 4 * 3)
                                                    context.closePath()
                                                } else {
                                                    context.moveTo(width / 2,
                                                                   height / 4)
                                                    context.lineTo(
                                                                0,
                                                                height / 4 * 3)
                                                    context.lineTo(
                                                                width,
                                                                height / 4 * 3)
                                                }
                                                context.closePath()
                                                if (where3Cbox.enabled)
                                                    context.fillStyle = "#434343"
                                                else
                                                    context.fillStyle = Qt.lighter(
                                                                "#434343", 1.15)
                                                context.fill()
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 88
                                            implicitHeight: 18
                                            border.color: where3Cbox.enabled ? where3Cbox.hovered ? "#1583dd" : "lightgray" : Qt.lighter("lightgray", 1.10)
                                            border.width: (where3Cbox.enabled
                                                           && where3Cbox.hovered) ? 2 : 1
                                            radius: 3
                                        }

                                        popup: Popup {
                                            id: where3Pop
                                            y: where3Cbox.height + 2
                                            width: 88
                                            height: contentHeight > 112 ? 112 : (contentHeight > 0 ? contentHeight + 4 : 0)
                                            padding: 2
                                            clip: true

                                            contentItem: ListView {
                                                clip: true
                                                implicitHeight: contentHeight
                                                model: where3Cbox.delegateModel
                                                currentIndex: where3Cbox.highlightedIndex
                                                delegate: where3Cbox.delegate
                                            }
                                            background: Rectangle {
                                                color: "white"
                                                border.color: "#1583dd"
                                                border.width: 2
                                                radius: 2
                                            }
                                        }
                                    }
                                    //所在地4
                                    ComboBox {
                                        property bool isNeed: true
                                        id: where4Cbox
                                        displayText: currentText
                                        font.family: "新宋体"
                                        font.pixelSize: 14
                                        currentIndex: -1
                                        editable: false
                                        textRole: "name"
                                        onEnabledChanged: {
                                            where4Canvas.requestPaint()
                                        }

                                        contentItem: Item {
                                            width: 35
                                            height: 20
                                            clip: true
                                            Label {
                                                width: 35
                                                height: 20
                                                leftPadding: 3
                                                topPadding: 3
                                                clip: true
                                                text: where4Cbox.displayText
                                                font: where4Cbox.font
                                                color: "black"
                                            }
                                        }

                                        model: countyModel1

                                        delegate: ItemDelegate {
                                            id: where4Det
                                            width: 84
                                            height: 18
                                            leftPadding: 0
                                            background: Rectangle {
                                                color: where4Det.highlighted ? "#378fcf" : "#ffffff"
                                                border.color: "white"
                                                Text {
                                                    y: (parent.height - height) / 2
                                                    x: 5
                                                    text: name
                                                    color: where4Det.highlighted ? "white" : "black"
                                                    font: where4Cbox.font
                                                    clip: true
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                            highlighted: where4Cbox.highlightedIndex === index
                                        }

                                        indicator: Canvas {
                                            id: where4Canvas
                                            x: where4Cbox.width - width - where4Cbox.rightPadding
                                            y: where4Cbox.topPadding
                                               + (where4Cbox.availableHeight - height) / 2
                                            width: 10
                                            height: 10

                                            Connections {
                                                target: where4Pop
                                                onVisibleChanged: {
                                                    where4Canvas.requestPaint()
                                                }
                                            }

                                            onPaint: {
                                                var context = getContext("2d")
                                                context.reset()
                                                if (!where4Pop.visible) {
                                                    context.moveTo(0,
                                                                   height / 4)
                                                    context.lineTo(width,
                                                                   height / 4)
                                                    context.lineTo(
                                                                width / 2,
                                                                height / 4 * 3)
                                                    context.closePath()
                                                } else {
                                                    context.moveTo(width / 2,
                                                                   height / 4)
                                                    context.lineTo(
                                                                0,
                                                                height / 4 * 3)
                                                    context.lineTo(
                                                                width,
                                                                height / 4 * 3)
                                                }
                                                context.closePath()
                                                if (where4Cbox.enabled)
                                                    context.fillStyle = "#434343"
                                                else
                                                    context.fillStyle = Qt.lighter(
                                                                "#434343", 1.15)
                                                context.fill()
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 88
                                            implicitHeight: 18
                                            border.color: where4Cbox.enabled ? where4Cbox.hovered ? "#1583dd" : "lightgray" : Qt.lighter("lightgray", 1.10)
                                            border.width: (where4Cbox.enabled
                                                           && where4Cbox.hovered) ? 2 : 1
                                            radius: 3
                                        }

                                        popup: Popup {
                                            id: where4Pop
                                            y: where4Cbox.height + 2
                                            width: 88
                                            height: contentHeight > 112 ? 112 : (contentHeight > 0 ? contentHeight + 4 : 0)
                                            padding: 2
                                            clip: true

                                            contentItem: ListView {
                                                clip: true
                                                implicitHeight: contentHeight
                                                model: where4Cbox.delegateModel
                                                currentIndex: where4Cbox.highlightedIndex
                                                delegate: where4Cbox.delegate
                                            }
                                            background: Rectangle {
                                                color: "white"
                                                border.color: "#1583dd"
                                                border.width: 2
                                                radius: 2
                                            }
                                        }
                                    }
                                }
                            }
                            background: Rectangle {
                                border.color: "#0093d1"
                            }
                        }
                    }
                }
                //故   乡
                RowLayout {
                    spacing: 10
                    Label {
                        id: labHome
                        text: "故   乡"
                        font.family: "新宋体"
                        font.pixelSize: 14
                        color: "gray"
                    }

                    ComboBox {
                        id: homeCBox
                        font.family: "新宋体"
                        font.pixelSize: 15
                        currentIndex: -1
                        editable: false
                        displayText: home1Cbox.displayText + " " + home2Cbox.displayText + " "
                                     + home3Cbox.displayText + " " + home4Cbox.displayText
                        onDisplayTextChanged: {
                            okBtn.enabled = true
                        }
                        contentItem: Label {
                            leftPadding: 10
                            topPadding: 3
                            clip: true
                            text: homeCBox.displayText
                            font: homeCBox.font
                            color: "black"
                        }
                        indicator: null
                        background: Rectangle {
                            implicitWidth: 288
                            implicitHeight: 22
                            border.color: "lightgray"
                            radius: 3
                        }
                        popup: Popup {
                            id: homePop
                            y: homeCBox.height + 2
                            width: 232
                            height: 60
                            implicitHeight: contentHeight
                            padding: 1
                            clip: true //12b7f5
                            //58 20 6  11 20
                            contentItem: Item {
                                width: homePop.width
                                height: homePop.height
                                GridLayout {
                                    x: 15
                                    y: 4
                                    columns: 2
                                    columnSpacing: 24
                                    rowSpacing: 6
                                    //故乡1
                                    ComboBox {
                                        id: home1Cbox
                                        displayText: currentText
                                        font.family: "新宋体"
                                        font.pixelSize: 14
                                        currentIndex: -1
                                        editable: false
                                        textRole: "name"
                                        onCurrentIndexChanged: {
                                            var index = currentIndex
                                            if (index === -1) {
                                                home1Cbox.currentIndex = -1
                                                home2Cbox.enabled = false
                                                provinceModel2.clear()
                                                home3Cbox.enabled = false
                                                cityModel2.clear()
                                                home4Cbox.enabled = false
                                                countyModel2.clear()
                                            } else {
                                                home2Cbox.enabled = true
                                                home3Cbox.enabled = false
                                                home4Cbox.enabled = false

                                                var temp
                                                var length
                                                var i
                                                temp = indivadualWin.countryModel.get(
                                                            index).id
                                                length = indivadualWin.provinceModel.count
                                                //添加省数据到模型
                                                provinceModel2.clear()
                                                for (i = 0; i < length; ++i) {
                                                    if (indivadualWin.provinceModel.get(
                                                                i).fid === temp) {
                                                        provinceModel2.append({
                                                                                  "id": indivadualWin.provinceModel.get(i).id,
                                                                                  "name": indivadualWin.provinceModel.get(i).name,
                                                                                  "fid": temp
                                                                              })
                                                    }
                                                }
                                            }
                                        }
                                        onEnabledChanged: {
                                            home1Canvas.requestPaint()
                                        }

                                        contentItem: Item {
                                            width: 35
                                            height: 20
                                            clip: true
                                            Label {
                                                width: 35
                                                height: 20
                                                leftPadding: 3
                                                topPadding: 3
                                                clip: true
                                                text: home1Cbox.displayText
                                                font: home1Cbox.font
                                                color: "black"
                                            }
                                        }

                                        model: indivadualWin.countryModel

                                        delegate: ItemDelegate {
                                            id: home1Det
                                            width: 84
                                            height: 18
                                            leftPadding: 0
                                            background: Rectangle {
                                                color: home1Det.highlighted ? "#378fcf" : "#ffffff"
                                                border.color: "white"
                                                Text {
                                                    y: (parent.height - height) / 2
                                                    x: 5
                                                    text: name
                                                    color: home1Det.highlighted ? "white" : "black"
                                                    font: home1Cbox.font
                                                    clip: true
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                            highlighted: home1Cbox.highlightedIndex === index
                                        }

                                        indicator: Canvas {
                                            id: home1Canvas
                                            x: home1Cbox.width - width - home1Cbox.rightPadding
                                            y: home1Cbox.topPadding
                                               + (home1Cbox.availableHeight - height) / 2
                                            width: 10
                                            height: 10

                                            Connections {
                                                target: home1Pop
                                                onVisibleChanged: {
                                                    home1Canvas.requestPaint()
                                                }
                                            }

                                            onPaint: {
                                                var context = getContext("2d")
                                                context.reset()
                                                if (!home1Pop.visible) {
                                                    context.moveTo(0,
                                                                   height / 4)
                                                    context.lineTo(width,
                                                                   height / 4)
                                                    context.lineTo(
                                                                width / 2,
                                                                height / 4 * 3)
                                                    context.closePath()
                                                } else {
                                                    context.moveTo(width / 2,
                                                                   height / 4)
                                                    context.lineTo(
                                                                0,
                                                                height / 4 * 3)
                                                    context.lineTo(
                                                                width,
                                                                height / 4 * 3)
                                                }
                                                context.closePath()
                                                if (home1Cbox.enabled)
                                                    context.fillStyle = "#434343"
                                                else
                                                    context.fillStyle = Qt.lighter(
                                                                "#434343", 1.15)
                                                context.fill()
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 88
                                            implicitHeight: 18
                                            border.color: home1Cbox.enabled ? home1Cbox.hovered ? "#1583dd" : "lightgray" : Qt.lighter("lightgray", 1.10)
                                            border.width: (home1Cbox.enabled
                                                           && home1Cbox.hovered) ? 2 : 1
                                            radius: 3
                                        }

                                        popup: Popup {
                                            id: home1Pop
                                            y: home1Cbox.height + 2
                                            width: 88
                                            height: contentHeight > 112 ? 112 : (contentHeight > 0 ? contentHeight + 4 : 0)
                                            padding: 2
                                            clip: true

                                            contentItem: ListView {
                                                clip: true
                                                implicitHeight: contentHeight
                                                model: home1Cbox.delegateModel
                                                currentIndex: home1Cbox.highlightedIndex
                                                delegate: home1Cbox.delegate
                                            }
                                            background: Rectangle {
                                                color: "white"
                                                border.color: "#1583dd"
                                                border.width: 2
                                                radius: 2
                                            }
                                        }
                                    }
                                    //故乡2
                                    ComboBox {
                                        id: home2Cbox
                                        displayText: currentText
                                        font.family: "新宋体"
                                        font.pixelSize: 14
                                        currentIndex: -1
                                        editable: false
                                        textRole: "name"
                                        onCurrentIndexChanged: {
                                            var index = currentIndex
                                            if (index === -1) {
                                                home2Cbox.currentIndex = -1
                                                home3Cbox.enabled = false
                                                cityModel2.clear()
                                                home4Cbox.enabled = false
                                                countyModel2.clear()
                                            } else {
                                                home3Cbox.enabled = true
                                                home4Cbox.enabled = false

                                                var temp
                                                var length
                                                var i
                                                temp = provinceModel2.get(
                                                            index).id
                                                length = indivadualWin.cityModel.count
                                                //添加城市数据到模型
                                                cityModel2.clear()
                                                var id
                                                for (i = 0; i < length; ++i) {
                                                    if (indivadualWin.cityModel.get(
                                                                i).fid === temp) {
                                                        id = indivadualWin.cityModel.get(
                                                                    i).id
                                                        cityModel2.append({
                                                                              "id": id,
                                                                              "name": indivadualWin.cityModel.get(i).name,
                                                                              "fid": temp
                                                                          })
                                                    }
                                                }
                                                //如果长度为1 传县区数据给它，其代表自治市
                                                if (cityModel2.count == 1) {
                                                    home4Cbox.isNeed = false
                                                    length = indivadualWin.countyModel.count
                                                    //添加县区数据到模型
                                                    cityModel2.clear()
                                                    for (i = 0; i < length; ++i) {
                                                        if (indivadualWin.countyModel.get(
                                                                    i).fid === id) {
                                                            cityModel2.append({
                                                                                  "id": indivadualWin.countyModel.get(i).id,
                                                                                  "name": indivadualWin.countyModel.get(i).name,
                                                                                  "fid": id
                                                                              })
                                                        }
                                                    }
                                                } else {
                                                    home4Cbox.isNeed = true
                                                }
                                            }
                                        }
                                        onEnabledChanged: {
                                            home2Canvas.requestPaint()
                                        }

                                        contentItem: Item {
                                            width: 35
                                            height: 20
                                            clip: true
                                            Label {
                                                width: 35
                                                height: 20
                                                leftPadding: 3
                                                topPadding: 3
                                                clip: true
                                                text: home2Cbox.displayText
                                                font: home2Cbox.font
                                                color: "black"
                                            }
                                        }

                                        model: provinceModel2

                                        delegate: ItemDelegate {
                                            id: home2Det
                                            width: 84
                                            height: 18
                                            leftPadding: 0
                                            background: Rectangle {
                                                color: home2Det.highlighted ? "#378fcf" : "#ffffff"
                                                border.color: "white"
                                                Text {
                                                    y: (parent.height - height) / 2
                                                    x: 5
                                                    text: name
                                                    color: home2Det.highlighted ? "white" : "black"
                                                    font: home2Cbox.font
                                                    clip: true
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                            highlighted: home2Cbox.highlightedIndex === index
                                        }

                                        indicator: Canvas {
                                            id: home2Canvas
                                            x: home2Cbox.width - width - home2Cbox.rightPadding
                                            y: home2Cbox.topPadding
                                               + (home2Cbox.availableHeight - height) / 2
                                            width: 10
                                            height: 10

                                            Connections {
                                                target: home2Pop
                                                onVisibleChanged: {
                                                    home2Canvas.requestPaint()
                                                }
                                            }

                                            onPaint: {
                                                var context = getContext("2d")
                                                context.reset()
                                                if (!home2Pop.visible) {
                                                    context.moveTo(0,
                                                                   height / 4)
                                                    context.lineTo(width,
                                                                   height / 4)
                                                    context.lineTo(
                                                                width / 2,
                                                                height / 4 * 3)
                                                    context.closePath()
                                                } else {
                                                    context.moveTo(width / 2,
                                                                   height / 4)
                                                    context.lineTo(
                                                                0,
                                                                height / 4 * 3)
                                                    context.lineTo(
                                                                width,
                                                                height / 4 * 3)
                                                }
                                                context.closePath()
                                                if (home2Cbox.enabled)
                                                    context.fillStyle = "#434343"
                                                else
                                                    context.fillStyle = Qt.lighter(
                                                                "#434343", 1.15)
                                                context.fill()
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 88
                                            implicitHeight: 18
                                            border.color: home2Cbox.enabled ? home2Cbox.hovered ? "#1583dd" : "lightgray" : Qt.lighter("lightgray", 1.10)
                                            border.width: (home2Cbox.enabled
                                                           && home2Cbox.hovered) ? 2 : 1
                                            radius: 3
                                        }

                                        popup: Popup {
                                            id: home2Pop
                                            y: home2Cbox.height + 2
                                            width: 88
                                            height: contentHeight > 112 ? 112 : (contentHeight > 0 ? contentHeight + 4 : 0)
                                            padding: 2
                                            clip: true

                                            contentItem: ListView {
                                                clip: true
                                                implicitHeight: contentHeight
                                                model: home2Cbox.delegateModel
                                                currentIndex: home2Cbox.highlightedIndex
                                                delegate: home2Cbox.delegate
                                            }
                                            background: Rectangle {
                                                color: "white"
                                                border.color: "#1583dd"
                                                border.width: 2
                                                radius: 2
                                            }
                                        }
                                    }
                                    //故乡3
                                    ComboBox {
                                        id: home3Cbox
                                        displayText: currentText
                                        font.family: "新宋体"
                                        font.pixelSize: 14
                                        currentIndex: -1
                                        editable: false
                                        textRole: "name"
                                        onCurrentIndexChanged: {
                                            var index = currentIndex
                                            if (index === -1) {
                                                home3Cbox.currentIndex = -1
                                                home4Cbox.enabled = false
                                                countyModel2.clear()
                                            } else {
                                                home4Cbox.enabled = true
                                                if (!home4Cbox.isNeed) {
                                                    home4Cbox.enabled = false
                                                    return
                                                }
                                                var temp
                                                var i
                                                var length

                                                temp = cityModel2.get(index).id
                                                length = indivadualWin.countyModel.count
                                                //添加县区数据到模型
                                                countyModel2.clear()
                                                for (i = 0; i < length; ++i) {
                                                    if (indivadualWin.countyModel.get(
                                                                i).fid === temp) {
                                                        countyModel2.append({
                                                                                "id": indivadualWin.countyModel.get(i).id,
                                                                                "name": indivadualWin.countyModel.get(i).name,
                                                                                "fid": temp
                                                                            })
                                                    }
                                                }
                                            }
                                        }
                                        onEnabledChanged: {
                                            home3Canvas.requestPaint()
                                        }

                                        contentItem: Item {
                                            width: 35
                                            height: 20
                                            clip: true
                                            Label {
                                                width: 35
                                                height: 20
                                                leftPadding: 3
                                                topPadding: 3
                                                clip: true
                                                text: home3Cbox.displayText
                                                font: home3Cbox.font
                                                color: "black"
                                            }
                                        }

                                        model: cityModel2

                                        delegate: ItemDelegate {
                                            id: home3Det
                                            width: 84
                                            height: 18
                                            leftPadding: 0
                                            background: Rectangle {
                                                color: home3Det.highlighted ? "#378fcf" : "#ffffff"
                                                border.color: "white"
                                                Text {
                                                    y: (parent.height - height) / 2
                                                    x: 5
                                                    text: name
                                                    color: home3Det.highlighted ? "white" : "black"
                                                    font: home3Cbox.font
                                                    clip: true
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                            highlighted: home3Cbox.highlightedIndex === index
                                        }

                                        indicator: Canvas {
                                            id: home3Canvas
                                            x: home3Cbox.width - width - home3Cbox.rightPadding
                                            y: home3Cbox.topPadding
                                               + (home3Cbox.availableHeight - height) / 2
                                            width: 10
                                            height: 10

                                            Connections {
                                                target: home3Pop
                                                onVisibleChanged: {
                                                    home3Canvas.requestPaint()
                                                }
                                            }

                                            onPaint: {
                                                var context = getContext("2d")
                                                context.reset()
                                                if (!home3Pop.visible) {
                                                    context.moveTo(0,
                                                                   height / 4)
                                                    context.lineTo(width,
                                                                   height / 4)
                                                    context.lineTo(
                                                                width / 2,
                                                                height / 4 * 3)
                                                    context.closePath()
                                                } else {
                                                    context.moveTo(width / 2,
                                                                   height / 4)
                                                    context.lineTo(
                                                                0,
                                                                height / 4 * 3)
                                                    context.lineTo(
                                                                width,
                                                                height / 4 * 3)
                                                }
                                                context.closePath()
                                                if (home3Cbox.enabled)
                                                    context.fillStyle = "#434343"
                                                else
                                                    context.fillStyle = Qt.lighter(
                                                                "#434343", 1.15)
                                                context.fill()
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 88
                                            implicitHeight: 18
                                            border.color: home3Cbox.enabled ? home3Cbox.hovered ? "#1583dd" : "lightgray" : Qt.lighter("lightgray", 1.10)
                                            border.width: (home3Cbox.enabled
                                                           && home3Cbox.hovered) ? 2 : 1
                                            radius: 3
                                        }

                                        popup: Popup {
                                            id: home3Pop
                                            y: home3Cbox.height + 2
                                            width: 88
                                            height: contentHeight > 112 ? 112 : (contentHeight > 0 ? contentHeight + 4 : 0)
                                            padding: 2
                                            clip: true

                                            contentItem: ListView {
                                                clip: true
                                                implicitHeight: contentHeight
                                                model: home3Cbox.delegateModel
                                                currentIndex: home3Cbox.highlightedIndex
                                                delegate: home3Cbox.delegate
                                            }
                                            background: Rectangle {
                                                color: "white"
                                                border.color: "#1583dd"
                                                border.width: 2
                                                radius: 2
                                            }
                                        }
                                    }
                                    //故乡4
                                    ComboBox {
                                        property bool isNeed: true
                                        id: home4Cbox
                                        displayText: currentText
                                        font.family: "新宋体"
                                        font.pixelSize: 14
                                        currentIndex: -1
                                        editable: false
                                        textRole: "name"
                                        onEnabledChanged: {
                                            home3Canvas.requestPaint()
                                        }

                                        contentItem: Item {
                                            width: 35
                                            height: 20
                                            clip: true
                                            Label {
                                                id: tt
                                                width: 35
                                                height: 20
                                                leftPadding: 3
                                                topPadding: 3
                                                clip: true
                                                text: home4Cbox.displayText
                                                font: home4Cbox.font
                                                color: "black"
                                            }
                                        }

                                        model: countyModel2

                                        delegate: ItemDelegate {
                                            id: home4Det
                                            width: 84
                                            height: 18
                                            leftPadding: 0
                                            background: Rectangle {
                                                color: home4Det.highlighted ? "#378fcf" : "#ffffff"
                                                border.color: "white"
                                                Text {
                                                    y: (parent.height - height) / 2
                                                    x: 5
                                                    text: name
                                                    color: home4Det.highlighted ? "white" : "black"
                                                    font: home4Cbox.font
                                                    clip: true
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                            highlighted: home4Cbox.highlightedIndex === index
                                        }

                                        indicator: Canvas {
                                            id: home4Canvas
                                            x: home4Cbox.width - width - home4Cbox.rightPadding
                                            y: home4Cbox.topPadding
                                               + (home4Cbox.availableHeight - height) / 2
                                            width: 10
                                            height: 10

                                            Connections {
                                                target: home4Pop
                                                onVisibleChanged: {
                                                    home4Canvas.requestPaint()
                                                }
                                            }

                                            onPaint: {
                                                var context = getContext("2d")
                                                context.reset()
                                                if (!home4Pop.visible) {
                                                    context.moveTo(0,
                                                                   height / 4)
                                                    context.lineTo(width,
                                                                   height / 4)
                                                    context.lineTo(
                                                                width / 2,
                                                                height / 4 * 3)
                                                    context.closePath()
                                                } else {
                                                    context.moveTo(width / 2,
                                                                   height / 4)
                                                    context.lineTo(
                                                                0,
                                                                height / 4 * 3)
                                                    context.lineTo(
                                                                width,
                                                                height / 4 * 3)
                                                }
                                                context.closePath()
                                                if (home4Cbox.enabled)
                                                    context.fillStyle = "#434343"
                                                else
                                                    context.fillStyle = Qt.lighter(
                                                                "#434343", 1.15)
                                                context.fill()
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 88
                                            implicitHeight: 18
                                            border.color: home4Cbox.enabled ? home4Cbox.hovered ? "#1583dd" : "lightgray" : Qt.lighter("lightgray", 1.10)
                                            border.width: (home4Cbox.enabled
                                                           && home4Cbox.hovered) ? 2 : 1
                                            radius: 3
                                        }

                                        popup: Popup {
                                            id: home4Pop
                                            y: home4Cbox.height + 2
                                            width: 88
                                            height: contentHeight > 112 ? 112 : (contentHeight > 0 ? contentHeight + 4 : 0)
                                            padding: 2
                                            clip: true

                                            contentItem: ListView {
                                                id: home4view
                                                clip: true
                                                implicitHeight: contentHeight
                                                model: home4Cbox.delegateModel
                                                currentIndex: home4Cbox.highlightedIndex
                                                delegate: home4Cbox.delegate
                                            }
                                            background: Rectangle {
                                                color: "white"
                                                border.color: "#1583dd"
                                                border.width: 2
                                                radius: 2
                                            }
                                        }
                                    }
                                }
                            }
                            background: Rectangle {
                                border.color: "#0093d1"
                            }
                        }
                    }
                }
                //手机
                RowLayout {
                    spacing: 10
                    Label {
                        id: labPhone
                        text: "手   机"
                        font.family: "新宋体"
                        font.pixelSize: 14
                        color: "gray"
                    }
                    TextField {
                        id: fieldPhone
                        font.family: "新宋体"
                        onTextChanged: {
                            okBtn.enabled = true
                        }

                        background: Rectangle {
                            implicitWidth: 288
                            implicitHeight: 22
                            radius: 3
                            color: "white"
                            border.color: fieldPhone.hovered ? "#1583dd" : "lightgray"
                            border.width: fieldPhone.hovered ? 2 : 1
                        }
                    }
                }
                //个人说明
                RowLayout {
                    spacing: 2
                    Label {
                        text: "个人说明"
                        font.family: "新宋体"
                        font.pixelSize: 14
                        color: "gray"
                    }
                    //个人说明编辑区域
                    //用Item打破布局的约束
                    Item {
                        width: 290
                        height: 44
                        ScrollView {
                            id: viewStatement
                            anchors.fill: parent
                            hoverEnabled: true
                            TextArea {
                                id: editStatement
                                padding: 4
                                wrapMode: TextEdit.WrapAnywhere
                                font.family: "新宋体"
                                font.pixelSize: 13
                                onTextChanged: {
                                    okBtn.enabled = true
                                }
                            }
                            background: Rectangle {
                                border.width: viewStatement.hovered ? 2 : 1
                                border.color: viewStatement.hovered ? "#1583dd" : "lightgray"
                            }
                        }
                    }
                }
            }
        }
    }

    //底部 按钮栏
    Frame {
        id: bottomFrame
        padding: 6
        x: 1
        y: win.height - 41
        MouseCustomForWindow {
            onSendPos: {
                win.x += movedCoordinate.x
                win.y += movedCoordinate.y
            }
        }
        background: Rectangle {
            implicitHeight: 35
            implicitWidth: win.width - 2
            color: "#dddddd"
        }
        RowLayout {
            x: bottomFrame.width / 2 + 4
            spacing: 15
            Layout.alignment: Qt.AlignTop
            Button {
                id: okBtn
                text: "保存"
                enabled: false
                onClicked: {
                    console.log("save button clicked")
                    var obj = {}
                    var name = fieldName.text.split(" ").filter(function (x) {
                        return x !== ""
                    })
                    if (name.length < 1)
                        return
                    obj.name = fieldName.text
                    obj.sex = sexCBox.currentText
                    obj.birthday = birCBox.displayText
                    obj.bloodGroup = bloodCBox.displayText
                    console.log("bloodGroup", obj.bloodGroup)
                    obj.signature = editTag.text
                    var length = eduModel.count
                    var edu = []
                    for (var i = 0; i < length; ++i) {
                        var temp = {}
                        var tempObj = eduModel.get(i)
                        temp.school = tempObj.r_school
                        temp.detail = tempObj.r_detail
                        temp.flags = tempObj.r_flags
                        edu[i] = temp
                    }
                    obj.edu = edu
                    obj.profession = professCBox.displayText
                    obj.corporation = fieldCorporation.displayText
                    obj.where1 = where1Cbox.currentText
                    obj.where2 = where2Cbox.currentText
                    obj.where3 = where3Cbox.currentText
                    obj.where4 = where4Cbox.currentText
                    obj.home1 = home1Cbox.currentText
                    obj.home2 = home2Cbox.currentText
                    obj.home3 = home3Cbox.currentText
                    obj.home4 = home4Cbox.currentText
                    obj.phone = fieldPhone.text
                    obj.statement = editStatement.text
                    indivadualWin.updateInfo(obj)
                    console.log("edit user's information win was closed")
                    win.close()
                }
                background: Rectangle {
                    implicitWidth: 72
                    implicitHeight: 22
                    border.width: 1
                    radius: 3
                    border.color: okBtn.activeFocus ? "#1583dd" : "lightgray"
                    color: okBtn.hovered ? okBtn.pressed ? Qt.lighter(
                                                               "#bee7fd",
                                                               1.2) : "#bee7fd" : Qt.lighter(
                                                               "#bee7fd", 1.4)
                }
            }
            Button {
                property var tx: null
                id: cancelBtn
                text: "取消"
                onClicked: {
                    console.log("clicked okBtn,now win will be closed")
                    win.close()
                    console.log("win was closed")
                }
                background: Rectangle {
                    implicitWidth: 72
                    implicitHeight: 22
                    radius: 3
                    color: cancelBtn.hovered ? cancelBtn.pressed ? Qt.lighter(
                                                                       "#bee7fd",
                                                                       1.2) : "#bee7fd" : Qt.lighter(
                                                                       "#bee7fd",
                                                                       1.4)
                    border.width: 1
                    border.color: cancelBtn.activeFocus ? "#1583dd" : "lightgray"
                }
            }
        }
    }
    //部件
    //添加教育信息界面
    Component {
        id: addEduInfoComp
        Window {
            property alias fieldSchool: fieldSchool
            property alias fieldInstitute: fieldInstitute
            property alias goCBox: goCBox
            property alias degreeCBox: degreeCBox
            property var item: null //一个信息框，用于修改信息框
            id: addEduWin
            visible: true
            width: 386
            height: 198
            flags: Qt.FramelessWindowHint
            color: "lightgray" //边界颜色
            //关闭处理
            onClosing: {
                console.log("onClosing:interface for editing information")
                hide() //不退出app
                eduView.isReadyToWrite = true
                //释放win资源
                addEduWin.destroy()
            }
            MouseCustomForWindow {
                onSendPos: {
                    addEduWin.x += movedCoordinate.x
                    addEduWin.y += movedCoordinate.y
                }
            }
            //躯干
            Rectangle {
                x: 1
                y: 1
                width: 384
                height: 196
                //header 标题栏
                Rectangle {
                    id: header
                    width: 384
                    height: 30
                    Image {
                        id: headerImg
                        sourceSize: Qt.size(384, 30)
                        source: "qrc:/images/mainInterface/alterEditInfo.png"
                    }
                    RowLayout {
                        x: header.width - 58
                        spacing: 0
                        Button {
                            id: minBtn
                            width: 30
                            height: 30
                            onClicked: {
                                addEduWin.showMinimized()
                            }

                            background: Image {
                                id: minImg
                                sourceSize: Qt.size(30, 30)
                                source: "qrc:/images/mainInterface/addEduHeader.png"
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
                                addEduWin.close()
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
                }
                //信息复合框
                GridLayout {
                    x: 20
                    y: 42
                    columns: 2
                    rowSpacing: 14
                    columnSpacing: 20
                    //学校//116 22
                    RowLayout {
                        spacing: 13
                        Label {
                            text: "*学  校"
                            font.family: "新宋体"
                            font.pixelSize: 14
                            color: "gray"
                        }
                        TextField {
                            id: fieldSchool
                            width: 116
                            height: 22
                            font.family: "新宋体"
                            font.pointSize: 10
                            background: Rectangle {
                                implicitWidth: 116
                                implicitHeight: 22
                                radius: 3
                                color: "white"
                                border.color: "lightgray"
                            }
                        }
                    }
                    RowLayout {
                        spacing: 10
                        Label {
                            text: "院系"
                            font.family: "新宋体"
                            font.pointSize: 10
                            color: "gray"
                        }
                        TextField {
                            id: fieldInstitute
                            font.family: "新宋体"
                            width: 116
                            height: 22
                            font.pointSize: 10
                            background: Rectangle {
                                implicitWidth: 116
                                implicitHeight: 22
                                radius: 3
                                color: "white"
                                border.color: "lightgray"
                            }
                        }
                    }
                    RowLayout {
                        spacing: 10
                        Label {
                            text: "入学时间"
                            font.family: "新宋体"
                            font.pointSize: 10
                            color: "gray"
                        }
                        ComboBox {
                            id: goCBox
                            font.family: "新宋体"
                            font.pixelSize: 12
                            currentIndex: -1
                            editable: false
                            displayText: currentIndex != -1 ? (currentText + "年") : "请选择"
                            contentItem: Label {
                                leftPadding: 10
                                topPadding: 5
                                text: goCBox.displayText
                                font: goCBox.font
                                color: "black"
                            }

                            model: goModel

                            delegate: ItemDelegate {
                                id: goDet
                                width: 114
                                height: 24

                                background: Rectangle {
                                    color: goDet.highlighted ? "#fffcdc" : "#ffffff"
                                    Text {
                                        y: (parent.height - height) / 2
                                        x: 8
                                        text: modelData + "年"
                                        color: "black"
                                        font: goCBox.font
                                        elide: Text.ElideRight
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                highlighted: goCBox.highlightedIndex === index
                            }

                            indicator: Canvas {
                                x: goCBox.width - width - goCBox.rightPadding
                                y: goCBox.topPadding + (goCBox.availableHeight - height) / 2
                                width: 10
                                height: 10
                                contextType: "2d"

                                onPaint: {
                                    context.reset()

                                    context.moveTo(width / 2, height / 4)
                                    context.lineTo(0, height / 4 * 3)
                                    context.lineTo(width, height / 4 * 3)
                                    context.closePath()
                                    context.fillStyle = "#434343"
                                    context.fill()
                                }
                            }

                            background: Rectangle {
                                implicitWidth: 116
                                implicitHeight: 22
                                border.color: goCBox.hovered ? "#1583dd" : "lightgray"
                                border.width: goCBox.hovered ? 2 : 1
                                radius: 3
                            }

                            popup: Popup {
                                y: goCBox.height + 1
                                width: 116
                                height: 98
                                padding: 2
                                clip: true
                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: goCBox.delegateModel
                                    currentIndex: goCBox.highlightedIndex
                                    delegate: goCBox.delegate
                                }
                                background: Rectangle {
                                    color: "white"
                                    border.color: "lightgray"
                                    border.width: 1
                                    radius: 2
                                }
                            }
                        }
                    }
                    RowLayout {
                        spacing: 10
                        Label {
                            text: "学历"
                            font.family: "新宋体"
                            font.pointSize: 10
                            color: "gray"
                        }
                        //学历
                        ComboBox {
                            id: degreeCBox

                            font.family: "新宋体"
                            font.pixelSize: 12
                            currentIndex: -1
                            displayText: currentIndex != -1 ? currentText : "请选择"
                            editable: false
                            contentItem: Label {
                                leftPadding: 10
                                topPadding: 5
                                text: degreeCBox.displayText
                                font: degreeCBox.font
                                color: "black"
                            }

                            model: ["小学", "初中", "高中", "中专", "本科", "研究生", "博士", "博士后"]

                            delegate: ItemDelegate {
                                id: degreeDet
                                width: 114
                                height: 24

                                background: Rectangle {
                                    color: degreeDet.highlighted ? "#fffcdc" : "#ffffff"
                                    Text {
                                        y: (parent.height - height) / 2
                                        x: 8
                                        text: modelData
                                        color: "black"
                                        font: degreeCBox.font
                                        elide: Text.ElideRight
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                highlighted: degreeCBox.highlightedIndex === index
                            }

                            indicator: Canvas {
                                x: degreeCBox.width - width - degreeCBox.rightPadding
                                y: degreeCBox.topPadding + (degreeCBox.availableHeight - height) / 2
                                width: 10
                                height: 10
                                contextType: "2d"

                                onPaint: {
                                    context.reset()

                                    context.moveTo(width / 2, height / 4)
                                    context.lineTo(0, height / 4 * 3)
                                    context.lineTo(width, height / 4 * 3)
                                    context.closePath()
                                    context.fillStyle = "#434343"
                                    context.fill()
                                }
                            }

                            background: Rectangle {
                                implicitWidth: 116
                                implicitHeight: 22
                                border.color: degreeCBox.hovered ? "#1583dd" : "lightgray"
                                border.width: degreeCBox.hovered ? 2 : 1
                                radius: 3
                            }

                            popup: Popup {
                                y: degreeCBox.height + 1
                                width: 116
                                height: 98
                                padding: 2
                                clip: true
                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: degreeCBox.delegateModel
                                    currentIndex: degreeCBox.highlightedIndex
                                    delegate: degreeCBox.delegate
                                }
                                background: Rectangle {
                                    color: "white"
                                    border.color: "lightgray"
                                    border.width: 1
                                    radius: 2
                                }
                            }
                        }
                    }
                }
                //按钮
                RowLayout {
                    spacing: 3
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 20
                    Button {
                        Text {
                            y: (parent.height - height) / 2
                            x: (parent.width - width) / 2
                            text: qsTr("保存")
                            color: "white"
                        }
                        onClicked: {
                            var school = fieldSchool.text.split(" ").filter(
                                        function (x) {
                                            return x !== ""
                                        })
                            if (school.length === 0)
                                return
                            school = school.join(" ")
                            var inst = fieldInstitute.text.split(" ").filter(
                                        function (x) {
                                            return x !== ""
                                        })

                            var flags = ""
                            var detail = []
                            var _1 = inst.join(" ")
                            if (_1 !== "") {
                                detail[detail.length] = _1
                                flags += "1"
                            }
                            if (goCBox.currentIndex != -1) {
                                detail[detail.length] = goCBox.currentText + "级"
                                flags += "2"
                            }
                            if (degreeCBox.currentIndex != -1) {
                                detail[detail.length] = degreeCBox.currentText
                                flags += "3"
                            }
                            detail = detail.join(",")
                            if (item !== null) {
                                item.school = school
                                item.detail = detail
                                item.flags = flags
                            } else
                                win.addEduInfo(school, detail, flags)
                            win.okBtn.enabled = true
                            addEduWin.close()
                        }

                        width: 65
                        height: 25
                        background: Rectangle {
                            implicitHeight: 25
                            implicitWidth: 65
                            color: "#12b7f5"
                        }
                    }
                    Button {
                        text: "取消"
                        width: 65
                        height: 25
                        onClicked: {
                            addEduWin.close()
                        }

                        background: Rectangle {
                            implicitHeight: 25
                            implicitWidth: 65
                            color: "transparent"
                            border.color: "lightgray"
                        }
                    }
                }
            }
            //模型
            //学级模型
            ListModel {
                id: goModel
                Component.onCompleted: {
                    var cur = parseInt(new Date().toJSON().substring(0, 4))
                    var last = parseInt((cur - 70) / 10) * 10
                    for (var i = cur; i >= last; --i) {
                        goModel.append({
                                           "modelData": i
                                       })
                    }
                }
            }
        }
    }
    //正在编辑教育信息提示窗口
    Component {
        id: eduTipComp
        Window {
            id: eduTipWin
            visible: true
            width: 338
            height: 153
            flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
            color: "lightgray" //边界颜色
            title: "提示"
            onClosing: {
                hide()
                eduTipWin.destroy()
            }
            MouseCustomForWindow {
                onSendPos: {
                    eduTipWin.x += movedCoordinate.x
                    eduTipWin.y += movedCoordinate.y
                }
            }
            BorderImage {
                source: "qrc:/images/mainInterface/addEduTipback.png"
                width: 338
                height: 153
                border.left: 5
                border.top: 5
                border.right: 5
                border.bottom: 5
            }
            //关闭
            Button {
                id: closeBtn
                x: 309
                y: 1
                width: 28
                height: 30
                onClicked: {
                    console.log("closed win")
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
            //确认
            Button {
                id: okBtn
                text: "确定"
                x: 254
                y: 122
                width: 69
                height: 24
                onClicked: closeBtn.clicked()
                background: Rectangle {
                    implicitHeight: 24
                    implicitWidth: 69
                    radius: 3
                    color: okBtn.hovered ? okBtn.pressed ? Qt.lighter(
                                                               "#bee7fd",
                                                               1.2) : "#bee7fd" : Qt.lighter(
                                                               "#bee7fd", 1.4)
                }
            }
        }
    }
    //模型
    //公历年份
    ListModel {
        id: yearModel
        Component.onCompleted: {
            var cur = parseInt(new Date().toJSON().substring(0, 4)) //当前年份获取
            for (var i = cur; i > 1990; --i) {
                append({
                           "modelData": i
                       })
            }
        }
    }
    //公历月份
    ListModel {
        id: monthModel
        Component.onCompleted: {
            for (var i = 1; i <= 12; ++i) {
                append({
                           "modelData": i
                       })
            }
        }
    }
    //公历天
    ListModel {
        id: dayModel
    }
    //教育信息模型
    ListModel {
        id: eduModel
        onCountChanged: {
            console.log("countchanged")
            eduView.height = 65 * count
        }
    }
    //province模型
    ListModel {
        id: provinceModel1
    }
    ListModel {
        id: provinceModel2
    }
    //city模型
    ListModel {
        id: cityModel1
    }
    ListModel {
        id: cityModel2
    }
    //county模型
    ListModel {
        id: countyModel1
    }
    ListModel {
        id: countyModel2
    }
}
