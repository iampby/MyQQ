import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import "qrc:/"
//加载js
import "../chinese-lunar.js" as CLunar

//用户个人资料
Window {
    property alias loaderForAlterCover: loaderForAlterCover
    property alias loaderForEditInfo: loaderForEditInfo
    property alias countryModel: countryModel
    property alias provinceModel: provinceModel
    property alias cityModel: cityModel
    property alias countyModel: countyModel
    property var bir: [] //生日 3length
    property string birthday: "" //生日 x年x月（公历）
    property string age: "" //年纪
    property string bloodGroup: "" //血型
    property string constellation: "" //星座
    property string zodiac: "" //生肖
    property string qage: "" //Q龄
    property string personalStatement: "" //个人说明
    property string educationExperence: "" //教育经历
    property string profession: "" //职业
    property string corporation: "" //公司
    property string phone: "" //电话号码
    property string location1: "" //所在地1
    property string location2: "" //所在地2
    property string location3: "" //所在地3
    property string location4: "" //所在地4
    property string home1: "" //故乡1
    property string home2: "" //故乡2
    property string home3: "" //故乡3
    property string home4: "" //故乡4
    signal updateInfo(var obj)
    id: indivadualWin
    width: 722
    height: 522
    visible: true //显示图标
    title: "我的资料"
    flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
    color: "lightgray" //边界颜色
    //更改封面完成处理
    Connections {
        target: loaderForAlterCover.item
        onUpdateCover: {
            console.log("updateCover() function has been called")
            var path = "../user/" + funcc.myQQ + "/cover"
            leftImg.source = "" //刷新
            leftImg.source = "file:" + path
            funcc.updateCover(path)
            console.log("updated Cover")
        }
    }
    //获取my个人数据
    Connections {
        target: funcc
        //(QVariantMap obj)
        onEmitPersonalJsonInfo: {
            console.log("onEmitPersonalJsonInfo", obj)
            var birarr = obj["birthday"].split("-")
            if (birarr.length < 3 | birarr === undefined)
                console.log("birthday format is not normal")
            else {
                for (var i = 0; i < birarr.length; ++i) {
                    bir[i] = parseInt(birarr[i])
                }
            }
            birthday = bir[1] + "月" + bir[2] + "日(公历)"
            labMyQQ.text = qqMainWin.myqq //MyQQ
            labSex.text = qqMainWin.sex
            age = func.getAge(birarr)
            //生日
            constellation = func.getConstellation(birarr) //星座
            var buttonCount = 0 //计数，如果大于4显示资料按钮
            //所在地
            location1 = obj["location1"]
            var temp = ""
            if (location1 !== undefined && location1 !== "") {
                location2 = obj["location2"]
                location3 = obj["location3"]
                location4 = obj["location4"]
                if (location2.length !== 0)
                    temp = location2
                if (location3.length !== 0)
                    temp += " " + location3
                if (location4.length !== 0)
                    temp += " " + location4
                whereInfo.aftLabel.text = temp
                buttonCount += 1
            }
            console.log("where is ", temp)
            //手机
            temp = obj["phone"]
            if (temp !== undefined && temp !== "0") {
                phone = temp
                buttonCount += 1
            }
            console.log("phone number is ", phone)
            //Q龄
            var QAge = obj["registerDateTime"].split("-") //分割日期
            if (QAge != undefined && QAge.length > 2) {
                QAge = func.getQAge(QAge)
                qage = QAge + "龄"
                buttonCount += 1
            } else {
                console.log("Q age is not normal or null")
            }
            console.log("Q age is ", QAge)
            //家乡
            home1 = obj["home1"]
            if (home1 !== undefined && home1 !== "") {
                home2 = obj["home2"]
                home3 = obj["home3"]
                home4 = obj["home4"]
                temp = "" //归零
                if (home2.length !== 0)
                    temp = home2
                if (home3.length !== 0)
                    temp += " " + home3
                if (home4.length !== 0)
                    temp += " " + home4
                homeInfo.aftLabel.text = temp
                buttonCount += 1
            }
            console.log("home is ", temp)
            //职业
            var prof = obj["profession"]
            if (prof !== undefined && prof !== "") {
                profession = prof
                buttonCount += 1
            }
            console.log("profession is ", prof)
            //公司
            var corp = obj["corporation"]
            if (corp !== undefined && corp !== "") {
                corporation = corp
                buttonCount += 1
            }
            console.log("corporation is ", corp)
            //教育经历
            var edu = obj["education"]
            if (edu !== undefined && edu !== "") {
                educationExperence = edu
                buttonCount += 1
            }
            console.log("education experience is ", edu)
            //个人说明
            var perss = obj["personalStatement"]
            if (perss !== undefined && perss !== "") {
                personalStatement = perss
                buttonCount += 1
            }
            console.log("personalStatement is ", perss)
            //获取生肖
            var f = CLunar._chineseLunar
            var clyear = f.solarToLunar(new Date(birarr[0], birarr[1],
                                                 birarr[2])) //农历对象
            clyear = clyear["year"] //农历年
            var zodiac = f.animalName(clyear)
            indivadualWin.zodiac = "属" + zodiac //生肖
            console.log("zodiac is ", zodiac)
            //是否显示资料按钮
            if (buttonCount > 4) {
                //显示资料按钮
                separInfo.visible = false
                moreInfoItem.visible = true
            } else {
                showNotEmptyInfo() //显示所有非空资料卡
            }
        }
        //QVector<QString> names
        onEmitPersonalCoverAndPhoto: {
            console.log(" onEmitPersonalCoverAndPhoto")
            if (cover != "")
                leftImg.source = "file:../user/" + mainWin.myqq + "/" + cover
            var length = walls.length
            var id = 0
            for (var i = 0; i < length; ++i) {
                if (images.setPixmap3(
                            id,
                            "../user/" + mainWin.myqq + "/photoWall/" + walls[i])) {
                    ++id
                } else {
                    console.log("initialization failed to add a photo wall pixmap")
                }
            }
            length = id //更新图片个数
            if (length === 0)
                return
            //0 个返回
            wallContiner.setCurrentIndex(1) //显示照片墙视图
            if (length % 3 == 0) {
                imgHeader1.sourceSize = Qt.size(216, 216)
                imgHeader1.source = "image://wall/0"
                imgHeader1.x = 0
                imgHeader1.y = 0
                imgHeader2.sourceSize = Qt.size(108, 108)
                imgHeader2.source = "image://wall/1"
                imgHeader2.x = 216
                imgHeader2.y = 0
                imgHeader3.sourceSize = Qt.size(108, 108)
                imgHeader3.source = "image://wall/2"
                imgHeader3.x = 216
                imgHeader3.y = 108
                imgHeader1.visible = true
                imgHeader2.visible = true
                imgHeader3.visible = true
                //第二部分添加图片
                if (length > 3) {
                    var h = 108 * parseInt(
                                (length % 3 != 0 ? length / 3 : length / 3 - 1))
                    twoPartWall.height = h
                    wallContiner.contentHeight = h + 216
                    console.log(h)
                    twoPartWall.visible = true
                    for (i = 3; i < length; ++i) {
                        twoPartPhoWallModel.append({
                                                       "url": "image://wall/" + i
                                                   })
                    }
                }
            } else if (length % 3 == 2) {
                imgHeader1.sourceSize = Qt.size(160, 216)
                imgHeader1.source = "image://wall/0"
                imgHeader1.x = 0
                imgHeader1.y = 0
                imgHeader2.sourceSize = Qt.size(160, 216)
                imgHeader2.source = "image://wall/1"
                imgHeader2.x = 160
                imgHeader2.y = 0
                imgHeader1.visible = true
                imgHeader2.visible = true
                imgHeader3.visible = false
                imgHeader3.source = "" //归零
                //第二部分添加图片
                if (length > 2) {
                    h = 108 * parseInt(
                                (length % 3 != 0 ? length / 3 : length / 3 - 1))
                    twoPartWall.height = h
                    wallContiner.contentHeight = h + 216
                    console.log(h)
                    twoPartWall.visible = true
                    for (i = 2; i < length; ++i) {
                        twoPartPhoWallModel.append({
                                                       "url": "image://wall/" + i
                                                   })
                    }
                }
            } else if (length % 3 == 1) {
                imgHeader1.sourceSize = Qt.size(320, 216)
                imgHeader1.source = "image://wall/0"
                imgHeader1.x = 0
                imgHeader1.y = 0

                imgHeader1.visible = true
                imgHeader2.visible = false
                imgHeader3.visible = false
                imgHeader2.source = "" //归零
                imgHeader3.source = "" //归零
                //第二部分添加图片
                if (length > 1) {
                    h = 108 * parseInt(
                                (length % 3 != 0 ? length / 3 : length / 3 - 1))
                    twoPartWall.height = h
                    wallContiner.contentHeight = h + 216
                    console.log(h)
                    twoPartWall.visible = true
                    for (i = 1; i < length; ++i) {
                        twoPartPhoWallModel.append({
                                                       "url": "image://wall/" + i
                                                   })
                    }
                }
            }
            console.log("photo wall initialization is of success")
            cont2.count = length
        }
    }

    //关闭处理
    onClosing: {
        console.log("invidualData onClosing")
        indivadualWin.opacity = 0.0 //躲藏 不释放资源
        close.accepted = false //拒绝close
        indivadualWin.flags = Qt.FramelessWindowHint | Qt.Widget
    }
    //窗口可视化变化处理
    onOpacityChanged: {
        if (opacity == 1.0) {
            console.log("invidiualData win is visible")
            if (infoCount > 4) {
                infoBtn.text = qsTr("更多资料")
                infoBtn.rightText = "▼"
                separInfo.visible = false
                moreInfoItem.visible = true
            } else {
                moreInfoItem.visible = false
                showNotEmptyInfo()
            }
        } else {
            console.log("invidiualData win is't  visible")
        }
    }

    //显示所有非空资料
    function showNotEmptyInfo() {
        separInfo.visible = true
        whereInfo.visible = !whereInfo.isEmpty
        phoneInfo.visible = !phoneInfo.isEmpty
        qAgeInfo.visible = !qAgeInfo.isEmpty
        homeInfo.visible = !homeInfo.isEmpty
        professInfo.visible = !professInfo.isEmpty
        educationInfo.visible = !educationInfo
        statementInfo.visible = !statementInfo.isEmpty
        console.log(" whereInfo.visible", whereInfo.visible)
        console.log(" phoneInfo.visible", phoneInfo.visible)
        console.log("  qAgeInfo.visible", qAgeInfo.visible)
        console.log(" homeInfo.visible ", homeInfo.visible)
        console.log(" professInfo.visible ", professInfo.visible)
        console.log(" educationInfo.visible ", educationInfo.visible)
        console.log(" statementInfo.visible ", statementInfo.visible)
    }

    //处理国家数据
    function addCountryData(arr) {
        console.log("addCountryData()")
        var length = arr.length
        if ((length & 1) == 1) {
            console.log("warning:country data is not normal")
            return
        }

        for (var i = 0; i < length; ) {
            countryModel.append({
                                    "id": arr[i],
                                    "name": arr[i + 1]
                                })
            i += 2
        }
    }
    //处理省级数据
    function addProvinceData(arr) {
        console.log("addProvinceData()")
        var length = arr.length
        if ((length % 3) != 0) {
            console.log("warning:province data is not normal")
            return
        }

        for (var i = 0; i < length; ) {
            provinceModel.append({
                                     "id": arr[i],
                                     "name": arr[i + 1],
                                     "fid": arr[i + 2]
                                 })
            i += 3
        }
    }
    //处理市级数据
    function addCityData(arr) {
        console.log("addCityData()")
        var length = arr.length
        if ((length % 3) != 0) {
            console.log("warning:city data is not normal")
            return
        }

        for (var i = 0; i < length; ) {
            cityModel.append({
                                 "id": arr[i],
                                 "name": arr[i + 1],
                                 "fid": arr[i + 2]
                             })
            i += 3
        }
    }
    //处理县级数据
    function addCountyData(arr) {
        console.log("addCountyData()")
        var length = arr.length
        if ((length % 3) != 0) {
            console.log("warning:county data is not normal")
            return
        }

        for (var i = 0; i < length; ) {
            countyModel.append({
                                   "id": arr[i],
                                   "name": arr[i + 1],
                                   "fid": arr[i + 2]
                               })
            i += 3
        }
    }
    //更新修改的资料
    onUpdateInfo: {
        var temp = obj["birthday"]
        var index
        console.log(temp)
        if (temp.length < 8) {
            console.log("warning:altered  birthday information is not corrected")
        } else {
            bir[0] = temp.substring(0, 4)
            index = temp.indexOf("月")
            if (index === -1) {
                console.log("warning:altered  birthday information is not corrected")
            } else {
                bir[1] = parseInt(temp.substring(5, index))
                bir[2] = parseInt(temp.substring(index + 1, temp.length - 1))
            }
        }
        birthday = bir[1] + "月" + bir[2] + "日(公历)"
        qqMainWin.sex = obj["sex"]
        qqMainWin.signature = obj["signature"]
        labSex.text = qqMainWin.sex
        age = func.getAge(bir)
        //生日
        constellation = func.getConstellation(bir) //星座
        var buttonCount = 0 //计数，如果大于4显示资料按钮
        //所在地
        location1 = obj["where1"]
        temp = ""
        if (location1 !== undefined && location1 !== "") {
            location2 = obj["where2"]
            location3 = obj["where3"]
            location4 = obj["where4"]
            if (location2.length !== 0)
                temp = location2
            if (location3.length !== 0)
                temp += " " + location3
            if (location4.length !== 0)
                temp += " " + location4
            whereInfo.aftLabel.text = temp
            buttonCount += 1
        } else {
            whereInfo.aftLabel.text = ""
            location1 = ""
            location2 = ""
            location3 = ""
            location4 = ""
        }
        console.log("where is ", temp)
        //手机
        temp = obj["phone"]
        if (temp !== undefined && temp !== "0" && temp !== "") {
            phone = temp
            buttonCount += 1
        } else {
            phone = "0"
        }
        console.log("phone number is ", phone)
        //Q龄
        buttonCount += 1
        //家乡
        home1 = obj["home1"]
        if (home1 !== undefined && home1 !== "") {
            home2 = obj["home2"]
            home3 = obj["home3"]
            home4 = obj["home4"]
            temp = "" //归零
            if (home2.length !== 0)
                temp = home2
            if (home3.length !== 0)
                temp += " " + home3
            if (home4.length !== 0)
                temp += " " + home4
            homeInfo.aftLabel.text = temp
            buttonCount += 1
        } else {
            homeInfo.aftLabel.text = ""
            home1 = ""
            home2 = ""
            home3 = ""
            home4 = ""
        }

        console.log("home is ", temp)
        //职业
        var prof = obj["profession"]
        if (prof !== undefined && prof !== "") {
            profession = prof
            buttonCount += 1
        } else {
            profession = ""
        }

        console.log("profession is ", prof)
        //公司
        var corp = obj["corporation"]
        if (corp !== undefined && corp !== "") {
            corporation = corp
            buttonCount += 1
        } else {
            corporation = ""
        }

        console.log("corporation is ", corp)
        //教育经历


        /* var edu = obj["education"]
      if (edu !== undefined && edu !== "") {
          educationExperence = edu
          buttonCount += 1
      }else{
          educationExperence = " "
      }
*/
        // console.log("education experience is ", edu)
        //个人说明
        var perss = obj["statement"]
        if (perss !== undefined && perss !== "") {
            personalStatement = perss
            buttonCount += 1
        } else {
            personalStatement = ""
        }

        console.log("personalStatement is ", perss)
        //获取生肖
        var f = CLunar._chineseLunar
        var clyear = f.solarToLunar(new Date(bir[0], bir[1], bir[2])) //农历对象
        clyear = clyear["year"] //农历年
        var zodiac = f.animalName(clyear)
        indivadualWin.zodiac = "属" + zodiac //生肖
        console.log("zodiac is ", zodiac)
        //是否显示资料按钮
        if (buttonCount > 4) {
            //显示资料按钮
            if (separInfo.visible) {
                showNotEmptyInfo()
                infoBtn.text = "收起资料"
                infoBtn.rightText = "▲"
            } else {
                separInfo.visible = false
                infoBtn.text = "更多资料"
                infoBtn.rightText = "▼"
            }
            moreInfoItem.visible = true
        } else {
            showNotEmptyInfo() //显示所有非空资料卡
        }
    }
    //移动鼠标
    MouseCustomForWindow {
        onSendPos: {
            indivadualWin.x += movedCoordinate.x
            indivadualWin.y += movedCoordinate.y
        }
    }
    //躯干
    Rectangle {
        id: body
        x: 1
        y: 1
        width: indivadualWin.width - 2
        height: indivadualWin.height - 2
        //左边封面
        Rectangle {
            id: leftRec
            width: 358
            height: 358
            Image {
                id: leftImg
                anchors.fill: parent
                cache: false
                sourceSize: Qt.size(358, 358)
                source: "qrc:/images/mainInterface/userInfoDefault.png"
            }
            //更换封面
            Button {
                property int xTip: 0
                property int yTip: 0
                id: renewCover
                x: 12
                y: 12
                width: 70
                height: 18
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        fDlog.open()
                    }
                    onPositionChanged: {
                        if (!renewCover.visible) {
                            renewCover.xTip = mouseX
                            renewCover.yTip = mouseY
                        }
                    }
                    onHoveredChanged: {
                        if (containsMouse)
                            renewCoverTip.visible = true
                        else
                            renewCoverTip.visible = false
                    }
                    onContainsMouseChanged: {
                        if (containsMouse) {
                            cursorShape = Qt.PointingHandCursor
                        } else
                            cursorShape = Qt.ArrowCursor
                    }
                }

                background: Rectangle {
                    implicitHeight: 18
                    implicitWidth: 70
                    color: "transparent"
                    Image {
                        id: garment
                        y: 2
                        sourceSize: Qt.size(15, 13)
                        source: "qrc:/images/mainInterface/userInfoClothing.png"
                    }
                    ColorOverlay {
                        anchors.fill: garment
                        source: garment
                        color: renewCover.hovered ? "lightblack" : "white"
                    }
                    Text {
                        id: coverText
                        x: 18
                        y: 2
                        color: renewCover.hovered ? "lightblack" : "white"
                        font.pointSize: 10
                        text: qsTr("更换封面")

                        layer.enabled: true
                        layer.effect: Glow {
                            color: Qt.rgba(200, 200, 200)
                            samples: 12
                            spread: 0.2
                        }
                    }
                }
            }
            ToolTip {
                id: renewCoverTip
                x: renewCover.x + renewCover.xTip
                y: renewCover.x + renewCover.yTip + 25
                width: 60
                height: 22
                delay: 1000
                Text {
                    anchors.centerIn: parent
                    text: "更换封面"
                    color: "gray"
                    horizontalAlignment: Text.AlignVCenter
                    verticalAlignment: Text.AlignHCenter
                }
            }
        }
        Rectangle {
            id: lfetBRec
            y: 358
            width: leftRec.width
            height: indivadualWin.height - 2 - leftRec.height
            color: "#5a898a"
            //头像区
            Rectangle {
                x: 59
                y: 45
                width: 240
                height: 60
                color: "transparent"
                //头像
                Rectangle {
                    property int posx: 0
                    property int posy: 0
                    id: recHead
                    y: 4
                    width: 56
                    height: 56
                    radius: 90
                    clip: true
                    color: "transparent"
                    border.color: "#23cdfe"
                    border.width: 0
                    MouseArea {
                        id: mouseHeadImg
                        width: parent.width
                        height: parent.height
                        hoverEnabled: true
                        onPressed: {
                            if (signatureTF.focus) {
                                //如果可以 个性签名编辑就能完成
                                signatureTF.focus = !signatureTF.focus
                            }
                            actions.openAlterHImgAct.trigger()
                        }
                        onHoveredChanged: {
                            if (containsMouse) {
                                cursorShape = Qt.PointingHandCursor
                                recHead.border.width = 2
                                headTip.show("点击更换头像")
                            } else {
                                cursorShape = Qt.ArrowCursor
                                recHead.border.width = 0
                                headTip.close()
                            }
                        }
                        onPositionChanged: {
                            if (!headTip.visible) {
                                recHead.posx = mouseX
                                recHead.posy = mouseY
                            }
                        }
                    }
                    Rectangle {
                        x: 2
                        y: 2
                        width: 52
                        height: 52
                        radius: 90
                        clip: true
                        Image {
                            id: imgHead
                            antialiasing: true
                            asynchronous: true
                            cache: false
                            fillMode: Image.PreserveAspectCrop
                            source: inCenterLoader.item.headSource
                            visible: false
                            sourceSize: Qt.size(parent.size, parent.size)
                        }
                    }
                    //遮罩
                    OpacityMask {
                        x: 2
                        y: 2
                        width: 52
                        height: 52
                        source: imgHead
                        maskSource: recHead
                    }
                }
                //头像提示
                ToolTip {
                    id: headTip
                    x: recHead.x + recHead.posx
                    y: recHead.y + recHead.posy + 20
                    width: 85
                    height: 25
                    delay: 1000
                    visible: false
                    font.pointSize: 10
                }
                //昵称
                Label {
                    id: nameLab
                    height: 24
                    width: 170
                    x: recHead.x + recHead.width + 10
                    y: recHead.y
                    font.pointSize: 13
                    font.family: song.name
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    maximumLineCount: 1
                    text: inCenterLoader.item.name
                    color: "#ffffff"
                    MouseForShape {
                        shapeInside: Qt.SizeVerCursor
                        shapeOutside: Qt.ArrowCursor
                    }
                }
                //个性签名
                TextField {
                    property int posx: 0
                    property int posy: 0
                    id: signatureTF
                    x: nameLab.x
                    y: nameLab.y + nameLab.height
                    hoverEnabled: true
                    placeholderText: "编辑个性签名"
                    placeholderTextColor: "white"
                    text: sigMetrics.elidedText
                    color: "white"
                    font.pointSize: 10
                    font.family: song.name
                    verticalAlignment: Text.AlignVCenter

                    onActiveFocusChanged: {
                        if (activeFocus) {
                            text = sigMetrics.text
                            selectAll()
                            color = "black"
                        } else {
                            inCenterLoader.item.signature = text //失去焦点 把文本赋值给个性签名
                            text = sigMetrics.elidedText
                            color = "white"
                        }
                    }
                    onTextChanged: {
                        if (text.length > 80) {
                            text = text.substring(0, 80) //只能为80字
                        }
                    }
                    onEditingFinished: {
                        signature = text
                        focus = false
                        sigBgd.focus = true
                    }
                    background: Rectangle {
                        id: sigBgd
                        implicitWidth: headRec.width - signatureTF.x - weatherRec.width - 3
                        implicitHeight: 16
                        color: signatureTF.activeFocus ? "white" : "transparent"
                        border.width: signatureTF.hovered ? (signatureTF.activeFocus ? 0 : 1) : 0
                        border.color: "dimgray"
                        Keys.onReturnPressed: {
                            if (!signatureTF.activeFocus)
                                signatureTF.focus = true
                        }
                    }

                    MouseArea {
                        width: parent.width
                        height: parent.height
                        hoverEnabled: true
                        onPressed: {
                            mouse.accepted = false
                            if (signatureTF.focus) {
                                signatureTF.focus = !signatureTF.focus
                            }
                        }

                        onHoveredChanged: {
                            console.log("signature tip hovered change")
                            if (containsMouse) {
                                sigTip.visible = true
                            } else {
                                tipTimer1sForSig.restart()
                            }
                        }
                        onPositionChanged: {
                            if (!sigTip.visible) {
                                signatureTF.posx = mouseX
                                signatureTF.posy = mouseY
                            }
                            mouse.accepted = true
                        }
                    }
                    TextMetrics {
                        id: sigMetrics
                        font: signatureTF.font
                        text: inCenterLoader.item.signature
                        elideWidth: signatureTF.width
                        elide: Text.ElideRight
                    }
                }
                //个性签名提示
                ToolTip {
                    id: sigTip
                    x: signatureTF.x + signatureTF.posx
                    y: signatureTF.y + signatureTF.posy + 20
                    delay: 1000
                    visible: false
                    background: Label {
                        property string str: "个性签名更新:" + signature
                        property int bw: 0
                        property int bh: 0
                        id: sigTipBack
                        // elide: Text.ElideRight
                        font.pointSize: 10
                        color: "gray"
                        wrapMode: Text.Wrap
                        maximumLineCount: 2
                        lineHeight: 1.1
                        verticalAlignment: Text.AlignVCenter
                        onStrChanged: {
                            var one = "个性签名更新:", two = "" //第一行字符 第二行字符

                            var bytes = func.getCharByteLength(one) //初始化累计字节数
                            var length = str.length
                            for (var i = 7; i < length; i++) {
                                var temp = str.charAt(i)
                                console.log("temp", temp, bytes)
                                var l = func.getCharByteLength(temp) //单个字符字节数
                                if (bytes + l < 29) {
                                    //如果小于29个字节则为第一行字符
                                    one += temp
                                    bytes += l
                                } else {
                                    two = str.substring(i, length) //设置第二行字符
                                    break
                                }
                            }
                            sigTipBack.text = one + "\n"
                            sigTipMetc.text = one
                            sigTipBack.bw = sigTipMetc.width + 3
                            if (two !== "") {
                                sigTipBack.bh = (sigTipMetc.height + 4) * 2
                                sigTipMetc.text = two //用于获取省略的字符
                                sigTipBack.text += sigTipMetc.elidedText
                            } else {
                                sigTipBack.bh = sigTipMetc.height + 4
                            }
                        }
                        background: Rectangle {
                            height: sigTipBack.bh
                            width: sigTipBack.bw
                            color: "#fefee1"
                        }
                        TextMetrics {
                            id: sigTipMetc
                            font: sigTipBack.font
                            elideWidth: sigTipBack.bw - 3 //省略字符宽度为提示背景宽度
                            elide: Text.ElideRight
                        }
                    }
                }
            }
        }
        //右边
        Item {
            id: right
            x: 358
            clip: true
            width: parent.width - 358
            height: parent.height

            Button {
                property int posx: 0
                property int posy: 0
                id: minBtn
                x: right.width - 64
                width: 32
                height: 33
                onClicked: {
                    indivadualWin.showMinimized()
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPositionChanged: {
                        //记录tip坐标
                        if (!minTip.visible) {
                            minBtn.posx = mouseX
                            minBtn.posy = mouseY
                        }
                    }
                    onPressed: {
                        mouse.accepted = false
                    }
                    onHoveredChanged: {
                        if (containsMouse) {
                            minTip.show("最小化")
                        } else {
                            minTip.close()
                        }
                    }
                }
                background: Rectangle {
                    implicitWidth: 32
                    implicitHeight: 33
                    color: "#5e6368"
                    Image {
                        id: minImg
                        source: "qrc:/images/mainInterface/userInfoMin.png"
                        sourceSize: Qt.size(32, 33)
                    }
                    ColorOverlay {
                        anchors.fill: minImg
                        source: minImg
                        color: minBtn.hovered ? (minBtn.pressed ? "#dedede" : Qt.lighter(
                                                                      "#dedede",
                                                                      1.10)) : "white"
                    }
                }
                //最小化提示
                ToolTip {
                    id: minTip
                    x: minBtn.posx
                    y: minBtn.posy + 20
                    delay: 1000
                    visible: false
                    font.pointSize: 10
                    Text {
                        anchors.fill: parent
                        text: minTip.text
                        font: minTip.font
                        color: "lightgray"
                    }
                    background: Rectangle {
                        height: minTip.height
                        width: minTip.width
                        border.width: 1
                        border.color: "lightgray"
                    }
                }
            }
            Button {
                property int posx: 0
                property int posy: 0
                id: closeBtn
                x: right.width - 32
                width: 32
                height: 33
                onClicked: {
                    indivadualWin.close()
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPositionChanged: {
                        //记录tip坐标
                        if (!closeTip.visible) {
                            closeBtn.posx = mouseX
                            closeBtn.posy = mouseY
                        }
                    }
                    onPressed: {
                        mouse.accepted = false
                    }

                    onHoveredChanged: {
                        if (containsMouse) {
                            closeTip.show("关闭")
                        } else {
                            closeTip.close()
                        }
                    }
                }

                background: Rectangle {
                    implicitWidth: 32
                    implicitHeight: 33
                    color: "#5e6368"
                    Image {
                        id: closeImg
                        source: "qrc:/images/mainInterface/userInfoClose.png"
                        sourceSize: Qt.size(32, 33)
                    }
                    ColorOverlay {
                        anchors.fill: closeImg
                        source: closeImg //ededed dedede ff5439 e04a32
                        color: closeBtn.hovered ? (closeBtn.pressed ? "#e04a32" : Qt.lighter(
                                                                          "#e04a32",
                                                                          1.2)) : "white"
                    }
                }
                //关闭提示
                ToolTip {
                    id: closeTip
                    x: closeBtn.posx
                    y: closeBtn.posy + 20
                    delay: 1000
                    visible: false
                    font.pointSize: 10
                    Text {
                        anchors.fill: parent
                        text: closeTip.text
                        font: closeTip.font
                        color: "lightgray"
                    }
                    background: Rectangle {
                        height: closeTip.height
                        width: closeTip.width
                        border.width: 1
                        border.color: "lightgray"
                    }
                }
            }

            //右边信息
            Flickable {
                id: rightFlick
                x: 20
                y: 40
                clip: true
                interactive: false
                width: right.width - x
                height: right.height - y - 15
                contentWidth: column.width
                contentHeight: column1.y + column1.height
                ScrollBar.vertical: ScrollBar {
                    parent: rightFlick //必须设置父对象才能启动hovered and pressed以及鼠标事件
                    anchors.right: rightFlick.right //绑定右边
                    orientation: Qt.Vertical
                    policy: ScrollBar.AlwaysOn // (contentHeight > container.height
                    //- 40) ? (hovered ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff) : ScrollBar.AlwaysOff
                    size: rightFlick.height / rightFlick.contentHeight
                    hoverEnabled: true
                    active: true
                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: parent.size * rightFlick.height
                        radius: width / 2
                        color: parent.hovered ? Qt.darker("#d7d7d7",
                                                          1.3) : "#d7d7d7"
                    }
                }
                //基本信息
                ColumnLayout {
                    id: column
                    clip: true
                    spacing: 15

                    //penguin bar
                    Item {
                        width: 324
                        height: 16
                        Image {
                            source: "qrc:/images/mainInterface/userInfoPenguin.png"
                        }
                        Label {
                            id: labMyQQ
                            x: 30
                            font.pointSize: 10
                        }
                        //编辑资料按钮
                        Button {
                            id: editMaterialBtn
                            x: 324 - width
                            width: 62
                            height: 16
                            onClicked: {
                                console.log("editMaterialBtn clicked")
                                actions.editMyInfoAct.trigger(indivadualWin)
                            }

                            background: Label {
                                width: 62
                                height: 16
                                text: "编辑资料"
                                font.family: "宋体"
                                font.pointSize: 10
                                color: editMaterialBtn.hovered ? Qt.lighter(
                                                                     "#009bdb",
                                                                     1.15) : "#009bdb"
                            }
                        }
                    }
                    //person bar
                    RowLayout {
                        spacing: 10
                        Image {
                            source: "qrc:/images/mainInterface/userInfoPerson.png"
                        }
                        Label {
                            id: labSex
                            font.pointSize: 10
                        }

                        Label {
                            id: labAge
                            font.pointSize: 10
                            text: indivadualWin.age
                        }
                        Label {
                            id: labBirthday
                            font.pointSize: 10
                            text: indivadualWin.birthday
                        }
                        Label {
                            id: labConstellation
                            font.pointSize: 10
                            text: indivadualWin.constellation
                        }
                        Label {
                            id: labZodiac
                            text: indivadualWin.zodiac
                            font.pointSize: 10
                        }
                    }
                    //grade bar
                    RowLayout {
                        spacing: 10
                        height: gridLay.height
                        Layout.alignment: Qt.AlignTop | Qt.AlignLeft //顶部对其，多行等级图标显示
                        //等级指示图标
                        Image {

                            source: "qrc:/images/mainInterface/userInfoGrade.png"
                        }
                        //等级图标
                        GridLayout {
                            property int posx: 0
                            property int posy: 0
                            id: gridLay
                            columns: 10
                            rowSpacing: 3
                            Repeater {
                                id: gradeRep
                                model: qqMainWin.gradeModel
                                ToolButton {
                                    background: Image {
                                        sourceSize: Qt.size(19, 19)
                                        source: img
                                    }
                                }
                            }
                            ToolTip {

                                id: gradeimgTip
                                x: gridLay.posx
                                y: gridLay.posy + 20
                                visible: false
                                // timeout: 1000
                                delay: 1000
                                background: Label {
                                    font.pointSize: 11
                                    text: "等级:" + grade
                                    verticalAlignment: Text.AlignVCenter
                                    background: Rectangle {
                                        height: parent.height
                                        width: parent.width
                                        color: "#fefee1"
                                    }
                                }
                            }
                            //等级区域鼠标
                            MouseArea {
                                id: gridMouse
                                x: gridLay.x
                                y: gridLay.y
                                width: gridLay.width //占用一行
                                height: gridLay.height
                                hoverEnabled: true
                                onContainsMouseChanged: {
                                    if (containsMouse) {
                                        //归零
                                        gradeimgTip.visible = false //刷新
                                        gradeimgTip.timeout = -1
                                        gradeimgTip.delay = 1000
                                        gradeimgTip.visible = true
                                    } else {
                                        //立即显示 超时1秒关闭
                                        gradeimgTip.timeout = 1000
                                        gradeimgTip.delay = 0
                                        gradeimgTip.visible = false //刷新状态
                                        gradeimgTip.show(
                                                    gradeimgTip.text) //显示超时一秒
                                    }
                                }

                                onPositionChanged: {
                                    if (!gradeimgTip.visible) {
                                        gridLay.posx = mouseX
                                        gridLay.posy = mouseY
                                    }
                                }
                            }
                        }
                    }
                    //separator
                    Rectangle {
                        id: separInfo
                        width: 324
                        height: 1
                        color: "lightgray"
                    }
                }
                //MyQQ  more information set
                Item {
                    id: infoItem
                    y: column.height + 10
                    visible: separInfo.visible
                    width: 324
                    height: infoColumn.height

                    Image {
                        id: imgInfo
                        sourceSize: Qt.size(16, 16)
                        source: "qrc:/images/mainInterface/userInfoQAge.png"
                    }
                    ColumnLayout {
                        id: infoColumn
                        x: 30
                        spacing: 15
                        onHeightChanged: {
                            console.log("??", height)
                        }

                        InfoSmallLabel {
                            id: whereInfo
                            preLabel.text: "所在地"
                            aftLabel.onTextChanged: {
                                console.log("where changed")
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: phoneInfo
                            preLabel.text: "手机"
                            aftLabel.text: indivadualWin.phone
                            aftLabel.onTextChanged: {
                                if (aftLabel.text !== "0"
                                        && aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: qAgeInfo
                            preLabel.text: "Q龄"
                            aftLabel.text: indivadualWin.qage
                            aftLabel.onTextChanged: {
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: homeInfo
                            preLabel.text: "家乡"
                            aftLabel.onTextChanged: {
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: professInfo
                            preLabel.text: "职业"
                            aftLabel.text: indivadualWin.profession
                            aftLabel.onTextChanged: {
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: corporationInfo
                            preLabel.text: "公司"
                            aftLabel.text: indivadualWin.corporation
                            aftLabel.onTextChanged: {
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: educationInfo
                            preLabel.text: "教育经历"
                            aftLabel.text: indivadualWin.educationExperence

                            aftLabel.onTextChanged: {
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: statementInfo
                            preLabel.text: "个人说明"
                            aftLabel.text: indivadualWin.personalStatement
                            aftLabel.onTextChanged: {
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                    }
                }
                ColumnLayout {
                    id: column1
                    y: infoItem.visible ? infoItem.y + infoItem.height + 10 : infoItem.y + 10
                    clip: true
                    spacing: 15

                    //资料按钮
                    Item {
                        id: moreInfoItem
                        visible: false
                        width: 220
                        height: infoBtn.height
                        ToolButton {
                            property string rightText: "▼"
                            id: infoBtn
                            x: 110
                            text: qsTr("更多资料")
                            width: 80
                            height: 26
                            rightPadding: 0
                            leftPadding: 0
                            onClicked: {
                                if (rightText === "▼") {
                                    rightText = "▲"
                                    text = qsTr("收起资料")
                                    showNotEmptyInfo()
                                } else {
                                    rightText = "▼"
                                    text = qsTr("更多资料")
                                    separInfo.visible = false
                                }
                            }
                            contentItem: Item {
                                width: infoBtn.width
                                height: infoBtn.height

                                Text {
                                    //右贴 间距10
                                    id: infoBtnLeft
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    text: infoBtn.text
                                    font: infoBtn.font
                                    color: "gray"
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    //右贴 间距10
                                    y: (infoBtnLeft.height - height) / 2 + infoBtn.y
                                    anchors.right: parent.right
                                    anchors.rightMargin: 10
                                    text: infoBtn.rightText
                                    font.pointSize: 5
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: "gray"
                                }
                            }

                            background: Rectangle {
                                implicitWidth: 80
                                implicitHeight: 26
                                radius: 3
                                color: infoBtn.hovered ? infoBtn.pressed ? "#f0f0f0" : "#f8f8f8" : "transparent"
                            }
                        }
                    }
                    //separator
                    Rectangle {
                        width: 324
                        height: 1
                        color: "lightgray"
                    }

                    //photo wall
                    Item {
                        width: rightFlick.width
                        height: 18
                        Label {
                            text: "照片墙"
                            font.pointSize: 12
                        }
                        Button {
                            id: loadPhotoBtn1
                            x: 324 - width
                            width: 62
                            height: 16
                            visible: !loadPhotoBtn2.visible

                            onClicked: {
                                console.log("editMaterialBtn clicked")
                                mFDlog.open()
                            }

                            background: Label {
                                width: 62
                                height: 16
                                text: "上传照片"
                                font.pointSize: 10
                                color: loadPhotoBtn1.hovered ? Qt.lighter(
                                                                   "#009bdb",
                                                                   1.15) : "#009bdb"
                            }
                        }
                    }
                    //照片墙内容
                    Container {
                        id: wallContiner
                        width: contentWidth
                        height: contentHeight
                        contentWidth: 324
                        contentHeight: 220
                        currentIndex: 0
                        onCurrentIndexChanged: {
                            if (currentIndex == 0) {
                                con1.visible = true
                                cont2.visible = false
                            } else {
                                con1.visible = false
                                cont2.visible = true
                            }
                        }

                        contentItem: Item {}
                        //无照片时状态
                        Rectangle {
                            id: con1
                            width: 324
                            height: 220
                            visible: true
                            Image {
                                sourceSize: Qt.size(324, 220)
                                source: "qrc:/images/mainInterface/wallBack.png"
                            }
                            Button {
                                id: loadPhotoBtn2
                                x: 71
                                y: 86
                                visible: true
                                width: 178
                                height: 46
                                onClicked: mFDlog.open()
                                background: Rectangle {
                                    implicitHeight: 46
                                    implicitWidth: 178
                                    color: loadPhotoBtn2.hovered ? Qt.lighter(
                                                                       "#009bdb",
                                                                       1.15) : "#009bdb"
                                    Image {
                                        sourceSize: Qt.size(178, 46)
                                        source: "qrc:/images/mainInterface/loadPhoto.png"
                                    }
                                }
                            }
                        }
                        //有照片时状态 这里限制只设置最多8个图片 采用手工布局
                        Item {
                            property int count: 0 //图片个数
                            id: cont2
                            width: 324
                            height: 216
                            visible: false
                            //照片墙头部，最多显示三个
                            Item {
                                id: headerWall
                                width: 324
                                height: 216
                                clip: true
                                Image {
                                    id: imgHeader1
                                    fillMode: Image.Stretch
                                    asynchronous: true
                                    visible: false
                                    width: sourceSize.width
                                    height: sourceSize.height
                                }
                                Image {
                                    id: imgHeader2
                                    asynchronous: true
                                    fillMode: Image.Stretch
                                    visible: false
                                    width: sourceSize.width
                                    height: sourceSize.height
                                }
                                Image {
                                    id: imgHeader3
                                    asynchronous: true
                                    fillMode: Image.Stretch
                                    visible: false
                                    width: sourceSize.width
                                    height: sourceSize.height
                                }
                            }
                            GridView {
                                id: twoPartWall
                                y: 216
                                width: 324
                                height: 0
                                interactive: false
                                displayMarginBeginning: 0
                                displayMarginEnd: 0
                                visible: false
                                model: twoPartPhoWallModel
                                cellHeight: 108
                                cellWidth: 108
                                delegate: Image {
                                    source: url
                                    sourceSize: Qt.size(108, 108)
                                    width: 108
                                    height: 108
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //界面
    //更改封面
    Loader {
        id: loaderForAlterCover
        visible: loaderForAlterCover.status == Loader.Ready
    }
    //编辑资料
    Loader {
        id: loaderForEditInfo
        visible: loaderForAlterCover.status == Loader.Ready
    }

    //文件打开框，不要用Qtc++的静态调用文件对话框，Qtc++的静态调用文件对话框不会自动释放资源，应该是里面有指针实现部分，我用这个可是异常延迟一天爆发
    FileDialog {
        id: fDlog
        title: "打开"
        fileMode: FileDialog.OpenFile
        defaultSuffix: "../"
        nameFilters: ["图像文件(*.jpeg;*.jpg;*.png)"]
        onAccepted: {
            loadPhotoBtn2.visible = false
            console.log("You chose: " + fDlog.file)
            actions.alterCoverAct.trigger(fDlog)
        }
        onRejected: {
            console.log("Canceled")
        }
    }
    //上传照片文件对话框 多个
    FileDialog {
        property string temp: "" //一个中转属性  用于qjsvalue 到 qurl的过度
        id: mFDlog
        title: "打开"
        fileMode: FileDialog.OpenFiles
        defaultSuffix: "../"
        nameFilters: ["图像文件(*.jpeg;*.jpg;*.png)"]
        onAccepted: {
            var files = mFDlog.files
            var length = files.length
            var sum = length + cont2.count //总图片数，这个app设计为最大9个图片
            if (sum > 9) {
                length = 9 - cont2.count //允许添加的个数
                sum = 9
            }
            var addFiles = []
            if (length === 0)
                return
            for (var i = length - 1; i >= 0; --i) {
                addFiles[i] = files[i].substring(8, files[i].length)
                console.log("photo wall added a file:", addFiles[i])
            }
            length = images.insertPixmap3(length, addFiles) //获取实际插入的图片数
            sum = length + cont2.count //重置总图片数
            twoPartPhoWallModel.clear() //归零第二部分
            wallContiner.setCurrentIndex(1) //显示照片墙视图
            if (sum % 3 == 0) {
                imgHeader1.sourceSize = Qt.size(216, 216)
                imgHeader1.source = "image://wall/0"
                imgHeader1.x = 0
                imgHeader1.y = 0
                imgHeader2.sourceSize = Qt.size(108, 108)
                imgHeader2.source = "image://wall/1"
                imgHeader2.x = 216
                imgHeader2.y = 0
                imgHeader3.sourceSize = Qt.size(108, 108)
                imgHeader3.source = "image://wall/2"
                imgHeader3.x = 216
                imgHeader3.y = 108
                imgHeader1.visible = true
                imgHeader2.visible = true
                imgHeader3.visible = true
                //第二部分添加图片
                if (sum > 3) {
                    var h = 108 * parseInt(
                                (sum % 3 != 0 ? sum / 3 : sum / 3 - 1))
                    twoPartWall.height = h
                    wallContiner.contentHeight = h + 216
                    console.log(h)
                    twoPartWall.visible = true
                    for (i = 3; i < sum; ++i) {
                        twoPartPhoWallModel.append({
                                                       "url": "image://wall/" + i
                                                   })
                    }
                }
            } else if (sum % 3 == 2) {
                imgHeader1.sourceSize = Qt.size(160, 216)
                imgHeader1.source = "image://wall/0"
                imgHeader1.x = 0
                imgHeader1.y = 0
                imgHeader2.sourceSize = Qt.size(160, 216)
                imgHeader2.source = "image://wall/1"
                imgHeader2.x = 160
                imgHeader2.y = 0
                imgHeader1.visible = true
                imgHeader2.visible = true
                imgHeader3.visible = false
                imgHeader3.source = "" //归零
                //第二部分添加图片
                if (sum > 2) {
                    h = 108 * parseInt((sum % 3 != 0 ? sum / 3 : sum / 3 - 1))
                    twoPartWall.height = h
                    wallContiner.contentHeight = h + 216
                    console.log(h)
                    twoPartWall.visible = true
                    for (i = 2; i < sum; ++i) {
                        twoPartPhoWallModel.append({
                                                       "url": "image://wall/" + i
                                                   })
                    }
                }
            } else if (sum % 3 == 1) {
                imgHeader1.sourceSize = Qt.size(320, 216)
                imgHeader1.source = "image://wall/0"
                imgHeader1.x = 0
                imgHeader1.y = 0

                imgHeader1.visible = true
                imgHeader2.visible = false
                imgHeader3.visible = false
                imgHeader2.source = "" //归零
                imgHeader3.source = "" //归零
                //第二部分添加图片
                if (sum > 1) {
                    h = 108 * parseInt((sum % 3 != 0 ? sum / 3 : sum / 3 - 1))
                    twoPartWall.height = h
                    wallContiner.contentHeight = h + 216
                    console.log(h)
                    twoPartWall.visible = true
                    for (i = 1; i < sum; ++i) {
                        twoPartPhoWallModel.append({
                                                       "url": "image://wall/" + i
                                                   })
                    }
                }
            }
            cont2.count = sum
            funcc.updatePhotoWall(length) //更新照片墙
        }
        onRejected: {
            console.log("Canceled")
        }
    }
    //时钟
    //延迟关闭提示
    //个性签名
    Timer {
        id: tipTimer1sForSig
        interval: 1000
        onTriggered: {
            sigTip.close()
        }
    }
    //照片墙第一行之外图片模型
    ListModel {
        id: twoPartPhoWallModel
    }
    //城市模型，用于编辑资料界面
    ListModel {
        id: countryModel
    }
    ListModel {
        id: provinceModel
    }
    ListModel {
        id: cityModel
    }
    ListModel {
        id: countyModel
    }
    Component.onCompleted: {
        funcc.inintCityData(indivadualWin)
    }
}
