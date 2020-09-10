import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Window 2.11
import QtQuick.Layouts 1.1
import "qrc:/"

Window {
    property alias setAn: setAn
    property alias finishAn: finishAn
    property alias isMouseInside: mouseArea1.containsMouse
    property int win: 1
    property bool popIsOpen: cityCbBoxPop.visible
    property int isFinishedToCraw: 0 //2表示完成

    //给各个天气显示控件赋值，数据来源于index处的qqMainWin.cityWeatherModel
    //通过加载器加载的部件不能在onCompleted信号里利用loader修改属性，所以这个函数不能在func部件中
    function assignValueAboutWeather(index) {
        textForTemper.text = qqMainWin.cityWeatherModel.get(index).temperature
        weatherLabel.text = qqMainWin.cityWeatherModel.get(
                    index).weather + " " + qqMainWin.cityWeatherModel.get(
                    index).wind + " " + qqMainWin.cityWeatherModel.get(
                    index).windGrade + "级"
        airLabel.text = "空气" + qqMainWin.cityWeatherModel.get(
                    index).aql + "  PM2.5 " + qqMainWin.cityWeatherModel.get(
                    index).pm
        day1.temperature = qqMainWin.cityWeatherModel.get(index).today
        day1.img = func.getImgPathForWeather(qqMainWin.cityWeatherModel.get(
                                                 index).weather)
        day2.temperature = qqMainWin.cityWeatherModel.get(index).tomorrow
        day2.img = func.getImgPathForWeather(qqMainWin.cityWeatherModel.get(
                                                 index).wea2)
        day3.temperature = qqMainWin.cityWeatherModel.get(index).today
        day3.img = func.getImgPathForWeather(qqMainWin.cityWeatherModel.get(
                                                 index).wea3)
    }

    id: weatherWin
    width: 236
    height: 246
    visible: true
    flags: Qt.Tool | Qt.FramelessWindowHint|Qt.WindowStaysOnTopHint//保持和父窗口同水平
    color: "#00000000"
    onVisibilityChanged: {
        if (visible) {
            cityCbBox.currentIndex = 0
        }
    }
//初始化3天的天气模型数据
    onIsFinishedToCrawChanged: {
        var index = cityCbBox.currentIndex
        if (2 === isFinishedToCraw) {
            qqMainWin.cityWeatherModel.setProperty(
                        index, "temperature", qqMainWin.arrayWea.temperature)
            qqMainWin.cityWeatherModel.setProperty(index, "weather",
                                                   funcc._3daysdata(0, 0, 0))
            qqMainWin.cityWeatherModel.setProperty(index, "wind",
                                                   qqMainWin.arrayWea.wind)
            qqMainWin.cityWeatherModel.setProperty(index, "windGrade",
                                                   qqMainWin.arrayWea.windGrade)
            qqMainWin.cityWeatherModel.setProperty(index, "aql", func.aql(
                                                       qqMainWin.arrayWea.pm))
            qqMainWin.cityWeatherModel.setProperty(index, "pm",
                                                   qqMainWin.arrayWea.pm)
            qqMainWin.cityWeatherModel.setProperty(index, "today",
                                                   funcc._3daysdata(0, 0, 1))
            qqMainWin.cityWeatherModel.setProperty(index, "tomorrow",
                                                   funcc._3daysdata(0, 1, 1))
            qqMainWin.cityWeatherModel.setProperty(index, "wea2",
                                                   funcc._3daysdata(0, 1, 0))
            qqMainWin.cityWeatherModel.setProperty(index, "after2days",
                                                   funcc._3daysdata(0, 2, 1))
            qqMainWin.cityWeatherModel.setProperty(index, "wea3",
                                                   funcc._3daysdata(0, 2, 0))

            recOne.visible = true
            assignValueAboutWeather(cityCbBox.currentIndex)
        }
    }

    Connections {
        target: funcc
        onCrawWeatherUrlFinished: {
            if (0 === weatherWin.isFinishedToCraw)
                weatherWin.isFinishedToCraw = 1
            else if (1 === weatherWin.isFinishedToCraw)
                weatherWin.isFinishedToCraw = 2
        }
    }
    Connections {
        target: func
        onMakeRequestFinished: {
            console.log("func")
            if (0 === weatherWin.isFinishedToCraw)
                weatherWin.isFinishedToCraw = 1
            else if (1 === weatherWin.isFinishedToCraw)
                weatherWin.isFinishedToCraw = 2
        }
    }

    Rectangle {
        property alias startAn1: rotateAn1.running
        property alias cwRotateAn1: cwRotateAn1.running

        id: recOne
        width: 236
        height: 246
        color: "lightblue"
        visible: true

        onVisibleChanged: {
            cityCbBox.backColor = "transparent"
        }

        AnimatedImage {
            id: animation
            width: 236
            height: 246
            source: "qrc:/images/mainInterface/weather/多云.gif"
            speed: 0.2
        }
        Button {
            id: setBtn
            width: 22
            height: 28
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 5
            anchors.rightMargin: 5
            background: Image {
                source: "qrc:/images/mainInterface/weather/set.png"
            }
            onClicked: {
                win = 2
                setAn.restart()
            }
        }

        ColumnLayout {
            x: 15
            y: 10
            spacing: 3
            RowLayout {
                height: 35
                spacing: 0
                Label {
                    id: textForTemper

                    font.pointSize: 24
                    Layout.alignment: Qt.AlignBottom
                }
                Label {
                    text: qsTr("℃")
                    font.pointSize: 18
                    Layout.alignment: Qt.AlignBottom
                }
            }

            ComboBox {
                property int popWidth: 80
                property int wid: 0
                property string backColor: "transparent"
                id: cityCbBox
                flat: true
                editable: true
                font.pixelSize: 15
                textRole: "city"
                currentIndex: 0
                model: qqMainWin.cityWeatherModel
                onActivated: {
                    console.log("active weather")
                    backColor = "transparent"
                }
                onCurrentIndexChanged: {
                    weatherWin.isFinishedToCraw = 0
                    if (qqMainWin.cityWeatherModel.get(
                                currentIndex).weather === "") {
                        var url = qqMainWin.cityWeatherModel.get(
                                    currentIndex).url
                        console.log("url:", url)
                        funcc.crawWeatherUrl(url)
                        func.makeRequest(url, qqMainWin.arrayWea)
                        timer10sToCrawWeater.restart()
                        recOne.visible = false
                    } else {
                        recOne.visible = false
                        weatherWin.assignValueAboutWeather(currentIndex)
                        recOne.visible = true
                    }
                }

                delegate: ItemDelegate {
                    id: cityItem
                    highlighted: cityCbBox.highlightedIndex === index
                    contentItem: Label {
                        text: city
                        leftPadding: 0
                        color: "#000000"
                        font: cityCbBox.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        implicitHeight: cityCbBox.height
                        implicitWidth: cityCbBoxPop.width //与Pop一致，保证颜色一致
                        color: cityItem.highlighted ? "#45a6cf" : "#edffec"
                    }
                }

                background: Rectangle {
                    implicitHeight: 25
                    implicitWidth: cityCbBox.wid
                    color: cityCbBox.backColor
                }
                contentItem: Text {
                    leftPadding: 0
                    rightPadding: cityCbBox.indicator.width + cityCbBox.spacing
                    text: cityCbBox.displayText
                    font: cityCbBox.font
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    onContentWidthChanged: {
                        cityCbBox.wid = contentWidth
                    }
                }
                indicator: Canvas {
                    id: canvas
                    x: cityCbBox.width - width - cityCbBox.rightPadding
                    y: cityCbBox.topPadding + (cityCbBox.availableHeight - height) / 2
                    width: 8
                    height: 5
                    contextType: "2d"

                    onPaint: {
                        context.reset()
                        context.moveTo(0, 0)
                        context.lineTo(width, 0)
                        context.lineTo(width / 2, height)
                        context.closePath()
                        context.fillStyle = "#000000"
                        context.fill()
                    }
                }
                popup: Popup {
                    id: cityCbBoxPop
                    y: cityCbBox.height - 2
                    padding: 0
                    closePolicy: Popup.CloseOnPressOutside
                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight
                        model: cityCbBoxPop.visible ? cityCbBox.delegateModel : null
                        currentIndex: cityCbBox.highlightedIndex
                        ScrollIndicator.vertical: ScrollIndicator {}
                    }
                    background: Rectangle {
                        implicitWidth: cityCbBox.popWidth
                        implicitHeight: contentItem.implicitHeight
                        border.color: Qt.rgba(66, 92, 110, 1.0)
                        radius: 2
                        color: "#edffec"
                    }
                }
            }

            Label {
                id: weatherLabel
                font.pixelSize: 15
            }
            Label {
                id: airLabel
                font.pixelSize: 15
            }
        }
        RowLayout {
            id: forecastLay
            y: 141
            spacing: 0
            TemperatureBtn {
                id: day1
                whichDay: "今天"
            }
            TemperatureBtn {
                id: day2
                whichDay: "明天"
            }
            TemperatureBtn {
                id: day3
                whichDay: "后天"
            }
        }
        Rectangle {
            width: parent.width
            height: 1
            opacity: 0.4
            x: 0
            y: 140
            color: "gray"
            border.width: {
                top: 1
                bottom: 1
                left: 0
                right: 0
            }
            border.color: Qt.rgba(200, 200, 200, 1.0)
        }
        transform: Rotation {
            origin: {
                origin.x = weatherWin.width / 2
                origin.y = weatherWin.height / 2
            }
            axis {
                x: 0
                y: 1
                z: 0
            }
            NumberAnimation on angle {
                //设置按钮的前半部分旋转动画
                running: false
                id: rotateAn1
                from: 0
                to: 90
                duration: 500
                loops: 1
            }
            NumberAnimation on angle {
                //设置界面的OKCNCEL按钮的后半部分旋转动画
                running: false
                id: cwRotateAn1
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
                target: recOne
                property: "startAn1"
                to: true
                duration: 0
            }

            PauseAnimation {
                duration: 500
            }
            PropertyAnimation {
                target: recOne
                property: "visible"
                to: false
                duration: 0
            }
            PropertyAnimation {
                target: recTwo
                property: "startAcwAn2"
                to: true
                duration: 0
            }
            PropertyAnimation {
                target: recTwo
                property: "visible"
                to: true
                duration: 0
            }
        }
        MouseArea {
            property bool boxIspressed: false
            id: mouseArea1
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onPressed: {
                mouse.accepted = false //当其false时,这个鼠标丢失released信号
                if (!(cityCbBox.x < mouseX
                      && (cityCbBox.x + cityCbBox.width > mouseX)
                      && cityCbBox.y < mouseY
                      && (cityCbBox.y + cityCbBox.height > mouseY)))
                    cityCbBox.backColor = "transparent"
            }

            onPositionChanged: {

                if (weatherWin.popIsOpen) {
                    cityCbBox.backColor = "#edffec"
                } else if (cityCbBox.x < mouseX
                           && (cityCbBox.x + cityCbBox.width > mouseX)
                           && cityCbBox.y < mouseY
                           && (cityCbBox.y + cityCbBox.height > mouseY)) {
                    cityCbBox.backColor = "#edffec"
                } else
                    cityCbBox.backColor = "transparent"
                if (mouseY > 142) {
                    if (mouseX < 79) {
                        day1.back.border.width = 1
                        day2.back.border.width = 0
                        day3.back.border.width = 0
                    } else if (mouseX < 158) {
                        day1.back.border.width = 0
                        day2.back.border.width = 1
                        day3.back.border.width = 0
                    } else {
                        day1.back.border.width = 0
                        day2.back.border.width = 0
                        day3.back.border.width = 1
                    }
                } else {
                    day1.back.border.width = 0
                    day2.back.border.width = 0
                    day3.back.border.width = 0
                }
            }

            onHoveredChanged: {
                if (!containsMouse) {
                    day1.back.border.width = 0
                    day2.back.border.width = 0
                    day3.back.border.width = 0
                    if (weatherWin.popIsOpen) {
                        cityCbBox.backColor = "#edffec"
                    }
                    if (win === 1)
                        actions.hideWeatherAct.trigger()
                }
            }
        }
    }

    Rectangle {
        property alias startAcwAn2: acwRotateAn2.running //控制是否开启从-90度到0逆时针旋转
        property alias startCwAn2: cwRotateAn2.running //控制是否开启从0度到-90顺时针旋转
        id: recTwo
        visible: false
        width: 236
        height: 246
        color: "#6cb2d9"
        ColumnLayout {
            x: 8
            y: 5
            spacing: 10

            Label {
                font.pointSize: 13
                text: qsTr("设置")
                color: "white"
            }
            ComboBox {
                property string backColor: "#4b7c97"
                property string color: "#ffffff"
                property int textAlign: 1
                id: textForSeachCity
                editable: true
                Component.onCompleted: {
                    tt.setColor()
                    if (tt.text === "只允许添加3个城市")
                        ttMouse.visible = true
                    else
                        ttMouse.visible = false
                }

                onEditTextChanged: {
                    console.log("editT=", editText, tt.text)
                    //通过高亮索引控制避免选择引起的editText变化影响
                    if (highlightedIndex != -1)
                        return
                    if (editText === "") {
                        //通过editText === ""避免模型清零带来的影响
                        // citySeachPop.close()
                        return
                    }
                    if (editText === "输入需要显示天气的城市" || editText === "只允许添加3个城市")
                        return
                    funcc.clearForCityList()
                    funcc.findCityEvent(editText)
                    citySeachModel.clear()
                    var cityCount = funcc.getCityCount()
                    if (cityCount == 0) {
                        textAlign = 4
                        citySeachModel.append({
                                                  "city": "未找到相应城市信息"
                                              })
                    } else {
                        textAlign = 1
                        for (var i = 0; i < cityCount; i++) {
                            citySeachModel.append({
                                                      "city": funcc.indexForCityList(
                                                                  i, 0)
                                                  })
                        }
                    }
                    citySeachPop.open()
                }
                onActivated: {
                    var index = textForSeachCity.highlightedIndex
                    if (index === -1)
                        index = 0

                    var city = citySeachModel.get(index).city
                    tt.text = "" //text控制显示文本
                    if (city === "未找到相应城市信息")
                        return
                    var count = qqMainWin.cityWeatherModel.count

                    var url = funcc.getWeatherUrl(city)
                    if (url === "")
                        return
                    funcc.setCityData(city, count - 1, 0)
                    funcc.setCityData(url, count - 1, 1)
                    qqMainWin.cityWeatherModel.append({
                                                          "temperature": "",
                                                          "city": city,
                                                          "weather": "",
                                                          "wind": "",
                                                          "windGrade": "",
                                                          "aql": "",
                                                          "pm": "",
                                                          "today": "",
                                                          "tomorrow": "",
                                                          "wea2": "",
                                                          "after3days": "",
                                                          "wea3": "",
                                                          "url": url
                                                      })

                    citySeachModel.clear()
                    tt.setColor()
                }

                model: ListModel {
                    id: citySeachModel
                }
                delegate: ItemDelegate {
                    id: citySeachDelegate
                    contentItem: Label {
                        id: delegateLab
                        text: city
                        color: (delegateLab.text
                                === "未找到相应城市信息") ? "#000000" : (highlighted ? "#ffffff" : "#000000")
                        font.pointSize: 14
                        elide: Text.ElideRight
                        horizontalAlignment: textForSeachCity.textAlign
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        implicitHeight: 50
                        implicitWidth: textForSeachCity.width
                        color: (delegateLab.text
                                === "未找到相应城市信息") ? "#ffffff" : (highlighted ? "#378fcf" : "#ffffff")
                    }
                    highlighted: textForSeachCity.highlightedIndex
                                 === -1 ? index === 0 : textForSeachCity.highlightedIndex === index
                }

                indicator: Label {
                    opacity: 0.0
                }

                contentItem: TextField {
                    signal setColor
                    //控制控件背景色和前景色变化
                    id: tt
                    leftPadding: 5
                    rightPadding: textForSeachCity.indicator.width + textForSeachCity.spacing
                    font: textForSeachCity.font
                    color: textForSeachCity.color
                    horizontalAlignment: textForSeachCity.textAlign
                    verticalAlignment: Text.AlignVCenter
                    background: backRec
                    onActiveFocusChanged: {
                        setColor()
                    }

                    onSetColor: {
                        if (activeFocus) {
                            if (tt.text === "只允许添加3个城市")
                                return
                            textForSeachCity.backColor = "#ffffff"
                            textForSeachCity.color = "#000000"
                            tt.text = ""
                            textForSeachCity.editable = true
                        } else {
                            textForSeachCity.backColor = "#4b7c97"
                            if (qqMainWin.cityWeatherModel.count === 4) {
                                textForSeachCity.color = "#lightgray"
                                tt.text = "只允许添加3个城市"
                                textForSeachCity.editable = false //不能控制编辑性
                            } else {
                                textForSeachCity.color = "#ffffff"
                                tt.text = "输入需要显示天气的城市"
                                textForSeachCity.editable = true //不能控制编辑性
                            }
                        }
                    }
                    MouseArea {
                        id: ttMouse
                        anchors.fill: parent
                        visible: false
                        propagateComposedEvents: true
                        onClicked: {
                            if (tt.text === "只允许添加3个城市")
                                visible = true
                            else
                                visible = false
                            // console.log(mouse.accepted)
                        }
                    }
                }

                background: Rectangle {
                    id: backRec
                    implicitHeight: 26
                    implicitWidth: 220
                    radius: 5
                    color: textForSeachCity.backColor
                }
                popup: Popup {
                    id: citySeachPop
                    visible: false
                    y: textForSeachCity.y + 2
                    width: textForSeachCity.width
                    implicitHeight: contentItem.implicitHeight
                    padding: 0
                    contentItem: ListView {
                        id: listV
                        clip: true
                        implicitHeight: (contentHeight > 150) ? 180 : contentHeight
                        model: textForSeachCity.popup.visible ? textForSeachCity.delegateModel : null
                        focus: true
                        currentIndex: textForSeachCity.highlightedIndex
                        ScrollIndicator.vertical: ScrollIndicator {}
                    }
                    background: Rectangle {
                        color: "#ffffff"
                    }
                }
            }

            ColumnLayout {
                width: 220
                height: 128
                Rectangle {
                    implicitHeight: 32
                    implicitWidth: 220
                    radius: 3
                    WeatherCityRec {
                        id: cityLocal
                        tex: qqMainWin.cityWeatherModel.count > 0 ? qqMainWin.cityWeatherModel.get(
                                                                        0).city : ""
                        img.source: "qrc:/images/mainInterface/weather/geo.png"
                        icoIsVisible: true
                        icoVisible: true
                        isSelected: true
                        onHoveredSmallIco: {
                            if (hovered)
                                img.source = "qrc:/images/mainInterface/weather/geo1.png"
                            else
                                img.source = "qrc:/images/mainInterface/weather/geo.png"
                        }
                        onIsSelectedChanged: {
                            if (isSelected) {
                                city1.isSelected = false
                                city2.isSelected = false
                                city3.isSelected = false
                            }
                        }
                    }
                }

                Rectangle {
                    implicitHeight: 32
                    implicitWidth: 220
                    radius: 3
                    WeatherCityRec {
                        id: city1
                        tex: qqMainWin.cityWeatherModel.count > 1 ? qqMainWin.cityWeatherModel.get(
                                                                        1).city : ""
                        img.source: "qrc:/images/mainInterface/weather/cross.png"
                        onHoveredSmallIco: {
                            if (hovered)
                                icoRec.color = Qt.darker(color, 1.5)
                            else
                                icoRec.color = color
                        }
                        onIsSelectedChanged: {
                            if (isSelected) {
                                cityLocal.isSelected = false
                                city2.isSelected = false
                                city3.isSelected = false
                            }
                        }
                        onClickedIco: {
                            qqMainWin.cityWeatherModel.remove(1, 1)
                            if (isSelected) {
                                cityLocal.isSelected = true
                            }
                        }
                    }
                }
                Rectangle {
                    implicitHeight: 32
                    implicitWidth: 220
                    radius: 3
                    WeatherCityRec {
                        id: city2
                        tex: qqMainWin.cityWeatherModel.count > 2 ? qqMainWin.cityWeatherModel.get(
                                                                        2).city : ""
                        img.source: "qrc:/images/mainInterface/weather/cross.png"
                        onHoveredSmallIco: {
                            if (hovered)
                                icoRec.color = Qt.darker(color, 1.5)
                            else
                                icoRec.color = color
                        }
                        onIsSelectedChanged: {
                            if (isSelected) {
                                cityLocal.isSelected = false
                                city1.isSelected = false
                                city3.isSelected = false
                            }
                        }
                        onClickedIco: {
                            qqMainWin.cityWeatherModel.remove(2, 1)
                            if (isSelected) {
                                cityLocal.isSelected = true
                            }
                        }
                    }
                }
                Rectangle {
                    implicitHeight: 32
                    implicitWidth: 220
                    radius: 3
                    WeatherCityRec {
                        id: city3
                        tex: qqMainWin.cityWeatherModel.count > 3 ? qqMainWin.cityWeatherModel.get(
                                                                        3).city : ""
                        img.source: "qrc:/images/mainInterface/weather/cross.png"
                        onHoveredSmallIco: {
                            if (hovered)
                                icoRec.color = Qt.darker(color, 1.5)
                            else
                                icoRec.color = color
                        }
                        onIsSelectedChanged: {
                            if (isSelected) {
                                city1.isSelected = false
                                city2.isSelected = false
                                cityLocal.isSelected = false
                            }
                        }
                        onClickedIco: {
                            qqMainWin.cityWeatherModel.remove(1, 1)
                            if (isSelected) {
                                cityLocal.isSelected = true
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 37
            color: "#8cc3e1"
            Button {
                property int index: 0
                id: returnBtn
                anchors.centerIn: parent
                width: 70
                height: 30
                text: "完成"
                font.pointSize: 13
                contentItem: Text {
                    font: returnBtn.font
                    text: returnBtn.text
                }
                onClicked: {
                    win = 1
                    finishAn.restart()
                    if (cityLocal.isSelected)
                        index = 0
                    else if (city1.isSelected)
                        index = 1
                    else if (city2.isSelected)
                        index = 2
                    else
                        index = 3
                    timer2s.restart()
                }
            }
        }

        transform: Rotation {
            origin: {
                origin.x = weatherWin.width / 2
                origin.y = weatherWin.height / 2
            }
            axis {
                x: 0
                y: 1
                z: 0
            }
            NumberAnimation on angle {
                //设置旋转效果的后半部分
                running: false
                id: acwRotateAn2
                from: -90
                to: 0
                duration: 500
                loops: 1
            }
            NumberAnimation on angle {
                //确定和取消旋转效果的前半部分
                running: false
                id: cwRotateAn2
                from: 0
                to: -90
                duration: 500
                loops: 1
            }
        }
        SequentialAnimation on visible {
            //OkCancel按钮的旋转动画
            id: finishAn
            running: false
            loops: 1
            //先可视化底层部件，以激活底层hover变化
            PropertyAnimation {
                target: recOne
                property: "visible"
                to: true
                duration: 0
            }
            PropertyAnimation {
                target: recTwo
                property: "startCwAn2"
                to: true
                duration: 0
            }

            PauseAnimation {
                duration: 500
            }
            PropertyAnimation {
                target: recTwo
                property: "visible"
                to: false
                duration: 0
            }
            PropertyAnimation {
                target: recOne
                property: "cwRotateAn1"
                to: true
                duration: 0
            }
        }
    }
    Timer {
        id: timer2s
        onTriggered: {
            cityCbBox.currentIndex = returnBtn.index
        }
    }
    Timer {
        id: timer10sToCrawWeater
        interval: 6000
        onTriggered: {
            if (2 !== weatherWin.isFinishedToCraw) {
                recOne.visible = true
                cityCbBox.currentIndex = 0
            }
        }
    }
    Component.onCompleted: {
        funcc.readWeatherFile("../user/" + mainWin.myqq + "/weather/city")
        assignValueAboutWeather(0)
//初始化各个城市数据
        for (var i = 0; i < 3; i++) {
            var city = funcc.getCityData(i, 0)
            if (city !== "") {
                qqMainWin.cityWeatherModel.append({
                                                      "temperature": "",
                                                      "city": funcc.getCityData(
                                                                  i, 0),
                                                      "weather": "",
                                                      "wind": "",
                                                      "windGrade": "",
                                                      "aql": "",
                                                      "pm": "",
                                                      "today": "",
                                                      "tomorrow": "",
                                                      "wea2": "",
                                                      "after3days": "",
                                                      "wea3": "",
                                                      "url": funcc.getCityData(
                                                                 i, 1)
                                                  })
            }
        }
        var length = 0
        for (i = 0; i < qqMainWin.cityWeatherModel.count; i++) {
            if (length < qqMainWin.cityWeatherModel.get(i).city.length)
                length = qqMainWin.cityWeatherModel.get(i).city.length
        }
        cityCbBox.popWidth = length * 15
    }
}
