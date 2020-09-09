import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import "qrc:/"
//加载js
import "../chinese-lunar.js" as CLunar
//日历对象


//好友个人资料或者陌生人资料卡
Window {
    property alias fgIndex: fgCBox.currentIndex
    property alias last: fgCBox.last //前一个索引
    property string number: "0"
    property string name: ""
    property string signature: ""
    property string tag: ""
    property string sex: sex
    property string headImg: ""
    property bool isFirst: false
    property alias eduModel: eduModel
    property var bir: [] //生日 3length
    property string birthday: "" //生日 x年x月（公历）
    property string age: "" //年纪
    property string bloodGroup: "" //血型
    property string constellation: "" //星座
    property string zodiac: "" //生肖
    property string qage: "" //Q龄
    property string personalStatement: "" //个人说明
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

    property string grade: "0"
    property int count: 0
    signal emitPersonalJsonInfo(var obj)
    //获取my个人数据(不包括图片)
    signal emitPersonalCoverAndPhoto(var walls, string cover)
    //获取封面和照片墙
    id: friendsInfoWin
    width: 722
    height: 522
    visible: true //显示图标
    title: tag.length == 0 ? name + "的资料" : tag + "的资料"
    flags: Qt.FramelessWindowHint | Qt.Window //显示任务栏
    color: "lightgray" //边界颜色

    //获取my个人数据
    //(QVariantMap obj)
    onEmitPersonalJsonInfo: {
        console.log("onEmitPersonalJsonInfo")
        var length = friendsModel.length
        //昵称处理
        var t = obj["name"]
        if (t != undefined && t != "") {
            //更新好友昵称
            if (t != name) {
                name = t //赋值
                for (var i = 0; i < length; ++i) {
                    let m = friendsModel[i]
                    let row = m.rowOf(number)
                    if (row !== -1) {
                        console.log("find friend,the number is ", number)
                        m.setData(row, name, 1)
                        console.log("updated show: ", name)
                        console.log("end update")
                        break
                    }
                }
            }
        }
        //签名
        t = obj["signature"]
        if (t != undefined && t != "") {
            //更新好友签名
            if (t != signature) {
                signature = t //赋值
                for (i = 0; i < length; ++i) {
                    let m = friendsModel[i]
                    let row = m.rowOf(number)
                    if (row !== -1) {
                        console.log("find friend,the number is ", number)
                        m.setData(row, signature, 2)
                        console.log("updated show: ", signature)
                        console.log("end update")
                        break
                    }
                }
            }
        }

        t = obj["grade"]
        //等级处理
        if (t != undefined && t != "") {
            //更新好友等级
            if (t != grade) {
                grade = t //赋值
                for (i = 0; i < length; ++i) {
                    let m = friendsModel[i]
                    let row = m.rowOf(number)
                    if (row !== -1) {
                        console.log("find friend,the number is ", number)
                        m.setData(row, grade, 5)
                        console.log("updated show: ", grade)
                        console.log("end update")
                        break
                    }
                }
            }
        }

        func.resetGradeModel(gradeModel, grade)

        sex = obj["sex"] //性别

        var birarr = obj["birthday"].split("-")
        if (birarr.length < 3 | birarr === undefined)
            console.log("birthday format is not normal")
        else {
            for (i = 0; i < birarr.length; ++i) {
                bir[i] = parseInt(birarr[i])
            }
        }
        birthday = bir[1] + "月" + bir[2] + "日(公历)"
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
            //如果是自治市区,4赋给3
            if (location2 == location3) {
                location3 = location4
                location4 = ""
            }
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
        if (QAge !== undefined && QAge.length > 2) {
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
            if (home2 == home3) {
                home3 = home4
                home4 = ""
            }

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
        length = 0
        var edu = obj["education"]
        if (edu !== undefined && edu !== "") {
            var arr = edu.split(";")
            length = arr.length
            console.log(length)
            if (length !== 0)
                educationInfo.isEmpty = false
            //添加教育信息到edumodel
            for (i = 0; i < length; ++i) {
                temp = arr[i].split(",")
                var length1 = temp.length
                if (length1 < 4) {
                    console.log("warning:education information is lacked")
                    continue
                }
                let school = temp[0]
                let institute = temp[1]
                let grade = temp[2]
                let degree = temp[3]
                if (school === undefined || school === "") {
                    console.log("warning:the school part of education information is not found")
                    continue
                }
                var detail = [], flags = "", count = 0
                if (institute !== undefined && institute !== "") {
                    detail[count++] = institute
                    flags += "1"
                }
                if (grade !== undefined && grade !== "") {
                    detail[count++] = grade
                    flags += "2"
                }
                if (degree !== undefined && degree !== "") {
                    detail[count++] = degree
                    flags += "3"
                }
                detail = detail.join(",")
                eduModel.append({
                                    "r_school": school,
                                    "r_detail": detail,
                                    "r_flags": flags
                                })
            }
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
        friendsInfoWin.zodiac = "属" + zodiac //生肖
        console.log("zodiac is ", zodiac)
        friendsInfoWin.count = buttonCount
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
            leftImg.source = "file:../user/" +qqMainWin.myqq+ "/photoWall/"+number+"/"+ cover
        var length = walls.length
        var id = 0
        for (var i = 0; i < length; ++i) {
            //好友id+1+照片墙序号区分各个背景照片
            if (images.setPixmap3(
                        number + "1" + id,
                        "../user/" +qqMainWin.myqq+ "/photoWall/"+number+"/" + walls[i])) {
                ++id
            } else {
                console.log("initialization failed to add a photo wall pixmap")
            }
        }
        length = id //更新图片个数
        //0 个返回 并不现实照片墙
        if (length === 0) {
            wallContiner.setCurrentIndex(-1)
            moreInfoItem.visible = false //全部显示资料片
            showNotEmptyInfo()
            return
        }
        wallContiner.setCurrentIndex(0) //显示照片墙视图
        if (length % 3 == 0) {
            imgHeader1.sourceSize = Qt.size(216, 216)
            imgHeader1.source = "image://wall/" + number + "10"
            imgHeader1.x = 0
            imgHeader1.y = 0
            imgHeader2.sourceSize = Qt.size(108, 108)
            imgHeader2.source = "image://wall/" + number + "11"
            imgHeader2.x = 216
            imgHeader2.y = 0
            imgHeader3.sourceSize = Qt.size(108, 108)
            imgHeader3.source = "image://wall/" + number + "12"
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
                                                   "url": "image://wall/" + number + "1" + i
                                               })
                }
            }
        } else if (length % 3 == 2) {
            imgHeader1.sourceSize = Qt.size(160, 216)
            imgHeader1.source = "image://wall/" + number + "10"
            imgHeader1.x = 0
            imgHeader1.y = 0
            imgHeader2.sourceSize = Qt.size(160, 216)
            imgHeader2.source = "image://wall/" + number + "11"
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
                                                   "url": "image://wall/" + number + "1" + i
                                               })
                }
            }
        } else if (length % 3 == 1) {
            imgHeader1.sourceSize = Qt.size(320, 216)
            imgHeader1.source = "image://wall/" + number + "10"
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
                                                   "url": "image://wall/" + number + "1" + i
                                               })
                }
            }
        }
        console.log("photo wall initialization is of success")
        cont1.count = length
    }

    //关闭处理
    onClosing: {
        console.log("friendsInfoWin onClosing")
        hide()
        mapInfo.delete(number + "1") //先移除map
        destroy() //毁灭窗口
    }
    //显示所有非空资料
    function showNotEmptyInfo() {
        separInfo.visible = true
        whereInfo.visible = !whereInfo.isEmpty
        phoneInfo.visible = !phoneInfo.isEmpty
        qAgeInfo.visible = !qAgeInfo.isEmpty
        bloodGroupInfo.visible = !bloodGroupInfo.isEmpty
        homeInfo.visible = !homeInfo.isEmpty
        professInfo.visible = !professInfo.isEmpty
        corporationInfo.visible = !corporationInfo.isEmpty
        educationInfo.visible = !educationInfo.isEmpty
        statementInfo.visible = !statementInfo.isEmpty
    }

    //移动鼠标
    MouseCustomForWindow {
        onSendPos: {
            friendsInfoWin.x += movedCoordinate.x
            friendsInfoWin.y += movedCoordinate.y
        }
    }
    //躯干
    Rectangle {
        id: body
        x: 1
        y: 1
        width: friendsInfoWin.width - 2
        height: friendsInfoWin.height - 2
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
        }
        Rectangle {
            id: lfetBRec
            y: 358
            width: leftRec.width
            height: friendsInfoWin.height - 2 - leftRec.height
            color: "#5a898a"
            //头像区
            Rectangle {
                id: headRec
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
                        onClicked: {
                            editTag.focus = false
                        }
                        onHoveredChanged: {
                            if (containsMouse) {
                                cursorShape = Qt.PointingHandCursor
                                recHead.border.width = 2
                                headTip.show("点击查看头像")
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
                            source: headImg
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
                    height: 30
                    width: 170
                    x: recHead.x + recHead.width + 10
                    y: recHead.y
                    font.pointSize: 20
                    font.family: song.name
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    maximumLineCount: 1
                    text: friendsInfoWin.tag.length == 0 ? friendsInfoWin.name : friendsInfoWin.tag
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
                    y: headRec.height - height - 3
                    padding: 0
                    hoverEnabled: true
                    readOnly: true //只读
                    text: sigMetrics.elidedText
                    color: "white"
                    font.pointSize: 10
                    font.family: song.name
                    verticalAlignment: Text.AlignVCenter
                    background: Rectangle {
                        id: sigBgd
                        implicitWidth: headRec.width - signatureTF.x - weatherRec.width - 3
                        implicitHeight: 16
                        color: "transparent"
                    }

                    MouseArea {
                        width: parent.width
                        height: parent.height
                        hoverEnabled: true
                        onClicked: {
                            editTag.focus = false
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
                        text: friendsInfoWin.signature
                        elideWidth: signatureTF.width
                        elide: Text.ElideRight
                        onTextChanged: {
                            signatureTF.text = elidedText
                        }
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
                        property string str: "个性签名更新:" + friendsInfoWin.signature
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
            Button {
                id: sendBtn
                x: headRec.x
                y: headRec.y + headRec.height + 10
                width: headRec.width
                height: 32
                hoverEnabled: true
                text: friendsInfoWin.fgIndex!=-1?"发消息":"添加好友"//如果为-1 ze陌生人是添加好友 不是"发消息"
                onClicked: {
                    if(text!="添加好友"){
                    qqMainWin.sentFriendInfo(friendsInfoWin.number) //打开对话界面
                    }else{//传信息到辅助进程打开添加好友进程
                        console.log("添加好友")
                       funcc.writeStandartInputToProcess(friendsInfoWin.number)
                    }
                }

                background: Rectangle {
                    width: headRec.width
                    height: 32
                    color: sendBtn.hovered ? sendBtn.pressed ? "#b7b6b6" : "#ffffff" : "#dbdbdb"
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
                    friendsInfoWin.showMinimized()
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
                    friendsInfoWin.close()
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
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        editTag.focus = false
                    }
                }
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
                            text: friendsInfoWin.number
                            font.pointSize: 10
                            //text: friendsInfoWin.number
                        }
                    }
                    //name
                    Item {
                        width: 324
                        height: 18
                        Image {
                            source: "qrc:/images/mainInterface/userInfoName.png"
                        }
                        Label {
                            text: "昵称"
                            x: 30
                            font.pointSize: 10
                        }
                        TextInput {
                            x: 100
                            readOnly: true
                            text: friendsInfoWin.name
                            font.pointSize: 11
                        }
                    }
                    //tag
                    Item {
                        width: 324
                        height: 18
                        visible: friendsInfoWin.fgIndex!=-1//如果为-1 则没有备注
                        Image {
                            source: "qrc:/images/mainInterface/userInfoTag.png"
                        }
                        Label {
                            text: "备注"
                            x: 30
                            font.pointSize: 10
                        }
                        RowLayout {
                            x: 100
                            TextField {
                                property string bcolor: "transparent"
                                id: editTag
                                readOnly: true
                                padding: 0
                                clip: true
                                width: tagMet.width + 6
                                height: 18
                                font.pointSize: 11
                                placeholderText: "添加"
                                text: friendsInfoWin.tag
                                onEditingFinished: {
                                    var b = (friendsInfoWin.tag != editTag.text)
                                    friendsInfoWin.tag = editTag.text
                                    readOnly = true
                                    editTag.bcolor = "transparent"
                                    editTag.width = tagMet.width + 6
                                    penBtn.visible = true
                                    //如果不等于就更新
                                    if (b) {
                                        let obj = {}
                                        let length = qqMainWin.friendsModel.length
                                        let i, index = -1
                                        for (i = 0; i < length; ++i) {
                                            let item = qqMainWin.friendsModel[i]
                                            if (item.rowOf(
                                                        friendsInfoWin.number) != -1) {
                                                index = i
                                                break
                                            }
                                        }
                                        if (index == -1) {
                                            console.log("warning:not found the friend group including a number ",
                                                        friendsInfoWin.number)
                                            return
                                        }
                                        obj.index = index
                                        obj.tag = friendsInfoWin.tag
                                        obj.number = friendsInfoWin.number
                                        qqMainWin.updateTag(obj) //更新签名
                                    }
                                }

                                background: Rectangle {
                                    implicitHeight: 18
                                    implicitWidth: parent.width
                                    border.color: parent.bcolor
                                }
                                TextMetrics {
                                    id: tagMet
                                    font: editTag.font
                                    text: friendsInfoWin.tag == "" ? "添加" : friendsInfoWin.tag
                                }
                            }
                            Button {
                                id: penBtn
                                width: 18
                                height: 18
                                hoverEnabled: true
                                onClicked: {
                                    visible = false
                                    editTag.readOnly = false
                                    editTag.width = 150
                                    if (editTag.text == "") {
                                        editTag.text = friendsInfoWin.name
                                    }
                                    editTag.bcolor = "lightgray"
                                    editTag.focus = true
                                }

                                background: Rectangle {
                                    implicitWidth: 18
                                    implicitHeight: 18
                                    Image {
                                        id: imgPen
                                        sourceSize: Qt.size(18, 18)
                                        source: "qrc:/images/mainInterface/userInfoPen.png"
                                    }
                                    ColorOverlay {
                                        anchors.fill: parent
                                        source: imgPen
                                        color: penBtn.hovered ? Qt.darker(
                                                                    "#7f8393",
                                                                    1.15) : "#7f8393"
                                    }
                                }
                            }
                        }
                    }
                    //friend groups
                    Item {
                        width: 324
                        height: 18
                        visible:friendsInfoWin.fgIndex!=-1//如果为-1 则没有好友组
                        Image {
                            source: "qrc:/images/mainInterface/userInfoFG.png"
                        }
                        Label {
                            id: labTag
                            text: "分组"
                            x: 30
                            font.pointSize: 10
                        }
                        //分组框
                        ComboBox {
                            property int last: -1
                            property int w: fgLab.contentWidth + 20
                            id: fgCBox
                            x: 100
                            font.family: "新宋体"
                            font.pixelSize: 14
                            currentIndex: -1
                            editable: false
                            textRole: "r_group"

                            onActivated: {
                                //更新分组
                                if (currentIndex != last) {
                                    try {
                                        var m = friendsModel[last]
                                        var index = m.rowOf(
                                                    friendsInfoWin.number)
                                        if (index == -1) {
                                            console.log("friend index is -1")
                                            last = currentIndex
                                            return
                                        }
                                    } catch (e) {
                                        console.log("warning:", e.message)
                                    }

                                    moveFriend(last, currentIndex, index)
                                }
                                last = currentIndex
                            }
                            contentItem: Label {
                                id: fgLab
                                padding: 2
                                text: fgCBox.displayText
                                font: fgCBox.font
                                color: "black"
                            }

                            model: qqMainWin.friendGroupModel

                            delegate: ItemDelegate {
                                id: fgDet
                                width: 142
                                height: 30

                                background: Rectangle {
                                    color: fgDet.highlighted ? "lightgray" : "#ffffff"
                                    border.color: "white"
                                    Text {
                                        y: (parent.height - height) / 2
                                        x: 8
                                        text: r_group
                                        font: fgCBox.font
                                        elide: Text.ElideRight
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                highlighted: fgCBox.highlightedIndex === index
                                Component.onCompleted: {
                                    if (r_group == "我的设备") {
                                        //过滤
                                        fgDet.visible = false
                                    }
                                }
                            }

                            indicator: Canvas {
                                id: fgCanvas
                                x: fgCBox.width - width - fgCBox.rightPadding
                                y: fgCBox.topPadding + (fgCBox.availableHeight - height) / 2
                                width: 8
                                height: 8
                                contextType: "2d"

                                Connections {
                                    target: fgPop
                                    onVisibleChanged: {
                                        fgCanvas.requestPaint()
                                    }
                                }

                                onPaint: {
                                    var context = getContext("2d")
                                    context.reset()
                                    if (!fgPop.visible) {
                                        context.moveTo(0, height / 4)
                                        context.lineTo(width, height / 4)
                                        context.lineTo(width / 2,
                                                       height / 4 * 3)
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
                                implicitWidth: parent.w
                                implicitHeight: 18
                                border.color: fgCBox.hovered ? "lightgray" : "transparent"
                            }

                            popup: Popup {
                                id: fgPop
                                y: fgCBox.height + 2
                                width: 146
                                implicitHeight: contentHeight - 30 + 4
                                padding: 2
                                clip: true
                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: fgCBox.delegateModel
                                    currentIndex: fgCBox.highlightedIndex
                                    delegate: fgCBox.delegate
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
                    //person bar
                    RowLayout {
                        spacing: 10
                        Image {
                            source: "qrc:/images/mainInterface/userInfoPerson.png"
                        }
                        Label {
                            id: labSex
                            text: friendsInfoWin.sex
                            font.pointSize: 10
                        }

                        Label {
                            id: labAge
                            font.pointSize: 10
                            text: friendsInfoWin.age
                        }
                        Label {
                            id: labBirthday
                            font.pointSize: 10
                            text: friendsInfoWin.birthday
                        }
                        Label {
                            id: labConstellation
                            font.pointSize: 10
                            text: friendsInfoWin.constellation
                        }
                        Label {
                            id: labZodiac
                            text: friendsInfoWin.zodiac
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
                                model: gradeModel
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
                                onClicked: {
                                    editTag.focus = false
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
                            aftLabel.text: friendsInfoWin.phone
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
                            aftLabel.text: friendsInfoWin.qage
                            aftLabel.onTextChanged: {
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: bloodGroupInfo
                            preLabel.text: "血型"
                            aftLabel.text: friendsInfoWin.bloodGroup
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
                            aftLabel.text: friendsInfoWin.profession
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
                            aftLabel.text: friendsInfoWin.corporation
                            aftLabel.onTextChanged: {
                                if (aftLabel.text != "") {
                                    isEmpty = false
                                } else
                                    isEmpty = true
                            }
                        }
                        InfoSmallLabel {
                            id: educationInfo
                            height: eduModel.count * 40
                            preLabel.text: "教育经历"
                            aftLabel.visible: false
                            ListView {
                                x: 70
                                width: 224
                                height: educationInfo.height
                                model: eduModel
                                delegate: Item {
                                    implicitWidth: 224
                                    implicitHeight: 40
                                    Label {
                                        width: 220
                                        height: 20
                                        text: r_school
                                        font.pointSize: 10
                                        font.family: "新宋体"
                                        elide: Text.ElideRight
                                    } //ebebeb
                                    Label {
                                        id: label
                                        y: 20
                                        width: 220
                                        height: 20
                                        text: r_detail
                                        color: "gray"
                                        font.pointSize: 9
                                        font.family: "新宋体"
                                        elide: Text.ElideRight
                                    }
                                }
                            }
                        }
                        InfoSmallLabel {
                            id: statementInfo
                            preLabel.text: "个人说明"
                            aftLabel.text: friendsInfoWin.personalStatement
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
                                cont1.visible = true
                            } else {
                                cont1.visible = false
                            }
                        }

                        contentItem: Item {}
                        //有照片时状态 这里限制只设置最多8个图片 采用手工布局
                        Item {
                            property int count: 0 //图片个数
                            id: cont1
                            width: 324
                            height: 216
                            visible: true
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

    //教育信息模型
    ListModel {
        id: eduModel
    }
    //等级图标模型
    ListModel {
        id: gradeModel
    }
}
