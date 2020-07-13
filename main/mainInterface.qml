import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.11
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.2
import Model 1.0
import "../"

//注意，qqMainWin是通过加载器从Component清零后用文件的方式加载的，这会导致，在初始化时通过加载器访问的属性异常
//即在刚刚初始化期间，通过对象直接访问属性才是安全的，所以这里所有的初始化的涉及属性相关的处理过程都在qqmainwin内进行
ApplicationWindow {
    property alias menuPop: menuPop //左下角菜单弹出窗
    property alias loaderForAbout: loaderForAbout //关于小窗口
    property alias loaderForMesWin: loaderForMesWin //切换小提示窗口
    property alias loaderForWeather: loaderForWeather //天气界面
    property alias loaderForHeadImg: loaderForHeadImg //头像悬浮界面
    property alias loaderForAlterHImg: loaderForAlterHImg //更改头像界面
    property alias loaderForAlterInfo: loaderForAlterInfo //修改用户资料界面
    property alias cityWeatherModel: cityWeatherModel //保存3天天气信息
    property int weatherCanShow: 0 //2表示准备好
    property bool isinit: true //用来避免第一次用户信息变化是发送更新信号
    property alias friendGroupModel: friendGroupModel
    property alias histroyImgModel: histroyImgModel //历史头像model
    property alias viewFriend: viewFriend

    property var friendsModel: [] //保存好友模型，用来过渡且便于使用
    //好友代理数，由于代理动态加载，在friendGroupModel添加数据过程中一直处于线程忙碌，无法直接构造，所以标记是否构造完用来加载好友模型
    property int friendDelegateCount: 0
    property var setInfo: ({}) //初始化空对象，用来保存设置信息
    property var arrayWea: {
        "temperature"//天气的主要数据
        //温度
        : "",
        "wind"//风向
        : "",
        "windGrade"//风力
        : "",
        "pm"//PM 2.5
        : ""
    }
    //用户属性
    property string myqq: ""
    property string name: "我的昵称"
    property alias headSource: imgHead.source //头像
    property string sex: ""
    property string activeDays: "" //数据库是int64 这里转为安全数据
    property int grade: -1
    property string signature: ""
    property string status: "1"
    property string where: ""
    property string townmans: ""

    //用于鼠标拉伸
    property int enableSize: 6
    property bool isPressed: false
    property point customPoint: Qt.point(0, 0)

    property int headImgY: qqMainWin.y + bodyRec.y + headRec.y + recHead.y //头像窗口的Y坐标
    signal sendLeaveForMenuBtnItems
    signal sendCoordinate(point pos)
    //移动坐标处理信号，参数为鼠标位置偏差
    signal runGetUserInfo(var obj)
    //信息： name（昵称） sex（性别） signature（个性签名） days（活跃天数） grade（等级) status(状态） 所在地 故乡
    //获得一个好友信息,其位于第pos个数组中，0 开始 过渡信号,代码具体执行在Func.qml
    signal runGetFriendInfoFunction(var obj, int pos)
    //获取设置信息
    signal runGetSetInfoFunction(var obj)

    id: qqMainWin
    visible: true
    x: func.qqMainX() //初始化窗口位置于贴右，或者右贴任务栏
    y: func.qqMainY()
    width: 282
    height: mainWin.desktopAvailableHeight - 13
    maximumHeight: mainWin.desktopAvailableHeight
    maximumWidth: 620
    minimumHeight: 527
    minimumWidth: 282
    color: "#00000000"
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint //取消窗口边缘框架
    MouseCustomForWindow {
        onPressed: {
            startPoint.x = mouseX
            startPoint.y = mouseY
            console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
            if (signatureTF.focus) {
                signatureTF.focus = !signatureTF.focus
            }
        }
        onSendPos: {
            qqMainWin.x += movedCoordinate.x
            qqMainWin.y += movedCoordinate.y
        }
    }
    //传myqq到image
    onMyqqChanged: {
        console.log("dispatched myqq to images")
        images.setMyQQ(myqq)
    }

    //利用元对象信号槽通信机制，把参数从Qtc++->qml 获取user信息
    onRunGetUserInfo: {
        console.log("onRunGetUserInfo")
        //信息： name（昵称） sex（性别） signature（个性签名） days（活跃天数） grade（等级) status(状态） 所在地 故乡
        name = obj["name"]
        console.log("qMainWin.myqq + \"1\", obj[\"headUrl\"]",
                    qqMainWin.myqq + "1" + obj["headUrl"])
        sex = obj["sex"]
        signature = obj["signature"]
        activeDays = obj["days"]
        grade = obj["grade"]
        status = obj["status"]
        where = obj["所在地"]
        townmans = obj["故乡"]
        var tag = qqMainWin.myqq + "101"
        images.setPixmap(tag, obj["headUrl"]) //好友号码+1
        histroyImgModel.append({
                                   "url": "image://history/" + tag,
                                   "index": 0
                               })
    }
    //利用元对象信号槽通信机制，把参数从Qtc++->qml 获取好友信息
    onRunGetFriendInfoFunction: {
        console.log("runGetFriendInforFuncion()")
        var m = friendsModel[pos] //获取以保存的空好友模型
        while (m === null) {
            console.log("m=null")
        } //如果为空则等待其加载
        var mq = obj["myqq"]
        var imgPath = "../user/" + mainWin.myqq + "/friends/" + mq + "1.png" //换为List.qml本地路径
        images.setPixmap2(mq + "1", imgPath)
        //添加数据到模型
        //qvariantmap转过来的只能[string]获取属性
        console.log("obj[\"个性签名\"]=", obj["个性签名"])
        var url = "image://friends/" + mq + "1"
        console.log("mq:", mq)
        m.append(mq, obj["昵称"], obj["个性签名"], url, obj["备注"], obj["等级"],
                 obj["状态"], obj["Message - Settin"], obj["Status_Settin"])
    }
    //利用元对象信号槽通信机制，把参数从Qtc++->qml,获取设置信息
    onRunGetSetInfoFunction: {
        console.log("obj")
        setInfo = obj //保存设置信息
    }
    //窗口关闭处理
    onClosing: {
        console.log("closing")
        for (var i = 0; i < 3; i++) {
            funcc.setCityData("", i, 0)
            funcc.setCityData("", i, 1)
        }

        for (i = 1; i < cityWeatherModel.count; i++) {
            funcc.setCityData(cityWeatherModel.get(i).city, i - 1, 0)
            funcc.setCityData(cityWeatherModel.get(i).url, i - 1, 1)
        }
        funcc.mkDir(("../user/" + mainWin.myqq + "/weather"))
        funcc.writeWeatherFile("../user/" + mainWin.myqq + "/weather/city")
        images.removeHistory()
        if (loaderForAlterHImg.status == Loader.Ready) {
            loaderForAlterHImg.item.close()
        }
        Qt.quit()
    }
    onWeatherCanShowChanged: {
        if (weatherCanShow === 2) {
            cityWeatherModel.append({
                                        "temperature": qqMainWin.arrayWea.temperature,
                                        "city": funcc.localCity + qsTr("(当地)"),
                                        "weather": funcc._3daysdata(0, 0, 0),
                                        "wind": qqMainWin.arrayWea.wind,
                                        "windGrade": qqMainWin.arrayWea.windGrade,
                                        "aql": func.aql(qqMainWin.arrayWea.pm),
                                        "pm": qqMainWin.arrayWea.pm,
                                        "today": funcc._3daysdata(0, 0, 1),
                                        "tomorrow": funcc._3daysdata(0, 1, 1),
                                        "wea2": funcc._3daysdata(0, 1, 0),
                                        "after2days": funcc._3daysdata(0, 2, 1),
                                        "wea3": funcc._3daysdata(0, 2, 0),
                                        "url": funcc.localUrl
                                    })
            imgWeather.source = func.getImgPathForWeather(cityWeatherModel.get(
                                                              0).weather)
        }
    }
    //当代理各项都实例化时，对代理项的代理初始化模型
    onFriendDelegateCountChanged: {
        //添加模型
        if (friendDelegateCount === friendGroupModel.count()) {
            var listM = viewFriend.contentItem.children
            for (var i = 0; i < listM.length; i++) {
                listM[i].delegateModel = friendsModel[i]
                listM[i].pos = i
                console.log("delegate pos:", i, listM[i] === undefined)
            }
        }
    }
    //个性签名更新
    onSignatureChanged: {
        console.log("onSignatureChanged:")
        //5 更新个性签名
        if (isinit) {
            isinit = false
            console.log("isinit=true")
            return
        }
        var ins = "5 updateSianature  " + myqq + "  headerSize"
        funcc.updateSignature(qqMainWin.signature, ins)
        for (var i = 0; i < friendsModel.length; ++i) {
            console.log("onSignatureChanged:i=", i)
            var m = friendsModel[i]
            var row = m.rowOf(myqq)
            if (row !== -1) {
                console.log("find myself,the number is ", myqq)
                m.setData(row, signature, 2)
                console.log("updated show: ", signature)
                console.log("end update")
            }
        }
    }
    //target func
    Connections {
        target: func
        //拖拽调整大小
        onSizeChanged: {
            console.log("onSizeChanged:", w, h)
            if (w < qqMainWin.minimumWidth) {
                if (directX < 0)
                    qqMainWin.x += (qqMainWin.width - qqMainWin.minimumWidth)
                qqMainWin.width = qqMainWin.minimumWidth
            } else if (w > qqMainWin.maximumWidth) {
                if (directX < 0)
                    qqMainWin.x += (qqMainWin.width - qqMainWin.maximumWidth)
                qqMainWin.width = qqMainWin.maximumWidth
            } else {
                qqMainWin.width = w
                if (directX < 0)
                    qqMainWin.x += delta.x
            }

            if (h < qqMainWin.minimumHeight) {
                if (directY < 0)
                    qqMainWin.y += (qqMainWin.height - qqMainWin.minimumHeight)
                qqMainWin.height = qqMainWin.minimumHeight
            } else if (h > qqMainWin.maximumHeight) {
                if (directY < 0)
                    qqMainWin.y += (qqMainWin.height - qqMainWin.maximumHeight)
                qqMainWin.height = qqMainWin.maximumHeight
            } else {
                qqMainWin.height = h
                if (directY < 0)
                    qqMainWin.y += delta.y
            }
        }
        onMakeRequestFinished: {
            if (weatherCanShow === 0)
                weatherCanShow = 1
            else if (weatherCanShow === 1)
                weatherCanShow = 2
        }
    }
    //target funcc
    Connections {
        target: funcc
        //得到网址时时爬取json部分的天气数据
        onFinished: {
            var url = funcc.localUrl
            funcc.crawWeatherUrl(url, 0)
            func.makeRequest(url, arrayWea)
        }
        //当本地天气已经准备好时，设置可以打开weather窗口
        onCrawWeatherUrlFinished: {
            if (weatherCanShow === 0)
                weatherCanShow = 1
            else if (weatherCanShow === 1)
                weatherCanShow = 2
        }
        //登录信息获取完成时传递qqmainwin到Qt，用来从qt里头调用qml方法
        onGetFileFinished: {
            funcc.win = qqMainWin //传递元对象到Qtc++
            funcc.initLoginInfo() //读取inxo.xml
            funcc.win = null
        }
        //接受读取的一个好友组信息到模型
        onGetFriendGroup: {
            friendGroupModel.append(name, "", "", set)
            //创建一个空好友模型，用于初始化viewFriend代理


            /*Qt.createQmlObject在有其它部件创建时可能回到导致null对象创建
            var friendObject = Qt.createQmlObject(('import Model 1.0;
    FriendModel{ }'), qqMainWin, "dynamic_friend_model")
            //以下和官方文档描述不符，不知道啥事阿
    if (friendObject === null) {
                //错误返回null
                var err = friendObject.qmlErrors
                //输出错误提示
                console.log("err.fileName", err.fileName, " in line:",
                            err.lineNumber, "column:", err.columnNumber, ",",
                            err.message)
            }
    */
            while (true) {
                if (compFriend.status === Component.Ready) {
                    var friendObject = compFriend.createObject(qqMainWin)
                    friendsModel[pos] = friendObject //保存模型用于后面代理初始化代理
                    console.log("created a FriendModel",
                                friendsModel[pos] === null)
                    break
                } else if (compFriend.status === Component.Error) {
                    console.log("compFriend created had failed")
                    break
                }
            }
        }
        //好友界面信息更改处理
        onUpdateFriendsModel: {
            console.log("value=", value, " number=", number)
            var length = friendsModel.length
            for (var i = 0; i < length; ++i) {
                console.log("onUpdateFriendsModel:i=", i)
                var m = friendsModel[i]
                var row = m.rowOf(number)
                if (row !== -1) {
                    console.log("start update:MyQQ is", number)
                    if (role === 3)
                        m.setData(row, "", role) //图片刷新
                    m.setData(row, value, role)
                    console.log("end update")
                }
            }
        }
    }

    //历史头像添加时
    Connections {
        target: images
        //头像被更改时
        onHistoryImageAdded: {
            console.log("onHistoryImageAdded:url(", url, ") index(")
            imgHead.source = ""
            // imgHead.source = "image://history/" + myqq + "101/" + Math.random()
            if (url != "") {
                histroyImgModel.append({
                                           "url": "image://history/" + url,
                                           "index": index
                                       })
            }
            imgHead.source = "image://history/" + myqq + "101" //设置头像 qimage源
        }
    }

    //实体
    Rectangle {
        id: bodyRec
        x: enableSize - border.width
        y: enableSize - border.width
        width: parent.width - 2 * enableSize + border.width * 2
        height: parent.height - 2 * enableSize + border.width * 2
        border.width: 2
        border.color: "lightgray"
        Rectangle {
            id: headerRec
            x: bodyRec.border.width
            y: bodyRec.border.width
            height: 32
            width: parent.width - 2 * bodyRec.border.width
            color: "#23cbfe" //35 203 254
            MouseCustomForWindow {
                onPressed: {
                    startPoint.x = mouseX
                    startPoint.y = mouseY
                    console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
                    if (signatureTF.focus) {
                        signatureTF.focus = !signatureTF.focus
                    }
                }
                onSendPos: {
                    qqMainWin.x += movedCoordinate.x
                    qqMainWin.y += movedCoordinate.y
                }
            }
            Image {
                x: 10
                y: (32 - height) / 2
                source: "qrc:images/mainInterface/qqWhite.ico"
            }
            RowLayout {
                id: rowHead
                x: headerRec.width - rowHead.width
                spacing: 0
                layoutDirection: Qt.RightToLeft
                Button {
                    property int xTip: 0
                    property int yTip: 0
                    id: closeBtn
                    width: 31
                    height: 32
                    hoverEnabled: true
                    flat: true
                    onClicked: {
                        close()
                    }
                    background: Image {
                        source: closeBtn.hovered ? "qrc:/images/mainInterface/qqCloseC.png" : "qrc:/images/mainInterface/qqClose.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onPressed: {
                            mouse.accepted = false

                            if (signatureTF.focus) {
                                signatureTF.focus = !signatureTF.focus
                            }
                        }
                        onPositionChanged: {
                            if (!closeTip.visible) {
                                closeBtn.xTip = mouseX
                                closeBtn.yTip = mouseY
                            }
                        }
                        onHoveredChanged: {
                            if (containsMouse)
                                closeTip.visible = true
                            else
                                closeTip.visible = false
                        }
                    }
                    ToolTip {
                        id: closeTip
                        x: closeBtn.xTip
                        y: closeBtn.yTip + 20
                        width: 35
                        height: 22
                        delay: 1000
                        Text {
                            text: "关闭"
                            color: "gray"
                            horizontalAlignment: Text.AlignVCenter
                            verticalAlignment: Text.AlignHCenter
                        }
                    }
                }
                Button {
                    property int xTip: 0
                    property int yTip: 0
                    id: minBtn
                    width: 31
                    height: 32
                    hoverEnabled: true
                    flat: true
                    onClicked: {
                        qqMainWin.hide()
                    }
                    background: Image {
                        id: imgMin
                        source: "qrc:images/mainInterface/qqMinimum.png"
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
                        onPositionChanged: {
                            if (!minTip.visible) {
                                minBtn.xTip = mouseX
                                minBtn.yTip = mouseY
                            }
                        }
                        onHoveredChanged: {
                            if (containsMouse)
                                minTip.visible = true
                            else
                                minTip.visible = false
                        }
                    }
                    BrightnessContrast {
                        id: brighrMin
                        anchors.fill: imgMin
                        source: imgMin
                        brightness: minBtn.pressed ? 0.5 : minBtn.hovered ? 0.3 : 0.0
                    }
                    ToolTip {
                        id: minTip
                        x: minBtn.xTip
                        y: minBtn.yTip + 20
                        width: 45
                        height: 22
                        delay: 1000
                        Text {
                            text: "最小化"
                            color: "gray"
                            horizontalAlignment: Text.AlignVCenter
                            verticalAlignment: Text.AlignHCenter
                        }
                    }
                }
                ToolButton {
                    property int xTip: 0
                    property int yTip: 0
                    id: infManagerBtn
                    width: 19
                    height: 32
                    hoverEnabled: true
                    background: Image {
                        id: imgManager
                        source: "qrc:/images/mainInterface/infManager.png"
                    }
                    MouseArea {
                        property int xTemp: 0
                        property int yTemp: 0
                        width: parent.width
                        height: parent.height
                        hoverEnabled: true
                        onPressed: {
                            mouse.accepted = false
                        }
                        onPositionChanged: {
                            if (!infManagerTip.visible) {
                                infManagerBtn.xTip = mouseX
                                infManagerBtn.yTip = mouseY
                            }
                        }
                        onHoveredChanged: {
                            if (containsMouse)
                                infManagerTip.visible = true
                            else
                                infManagerTip.visible = false
                        }
                    }
                    BrightnessContrast {
                        id: brighrManager
                        anchors.fill: imgManager
                        source: imgManager
                        brightness: infManagerBtn.pressed ? 0.5 : infManagerBtn.hovered ? 0.3 : 0.0
                    }
                    ToolTip {
                        id: infManagerTip
                        x: infManagerBtn.xTip
                        y: infManagerBtn.yTip + 20
                        width: 70
                        height: 22
                        delay: 1000
                        Text {
                            text: "界面管理器"
                            color: "gray"
                            horizontalAlignment: Text.AlignVCenter
                            verticalAlignment: Text.AlignHCenter
                        }
                    }
                }
                ToolButton {
                    property int xTip: 0
                    property int yTip: 0
                    id: moneyBtn
                    width: 31
                    height: 32
                    hoverEnabled: true
                    background: Image {
                        id: imgMoney
                        source: "qrc:/images/mainInterface/money.png"
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
                        onPositionChanged: {
                            if (!moneyTip.visible) {
                                moneyBtn.xTip = mouseX
                                moneyBtn.yTip = mouseY
                            }
                        }
                        onHoveredChanged: {
                            if (containsMouse)
                                moneyTip.visible = true
                            else
                                moneyTip.visible = false
                        }
                    }
                    BrightnessContrast {
                        id: brighrMoney
                        anchors.fill: imgMoney
                        source: imgMoney
                        brightness: moneyBtn.pressed ? 0.5 : moneyBtn.hovered ? 0.3 : 0.0
                    }
                    ToolTip {
                        id: moneyTip
                        x: moneyBtn.xTip
                        y: moneyBtn.yTip + 20
                        width: 60
                        height: 22
                        delay: 1000
                        Text {
                            text: "MyQQ钱包"
                            color: "gray"
                            horizontalAlignment: Text.AlignVCenter
                            verticalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }
        }
        //头像一栏
        Rectangle {
            id: headRec
            x: bodyRec.border.width
            y: headerRec.y + headerRec.height
            width: headerRec.width
            height: 80
            color: "#23cdfe"
            //头像
            Rectangle {
                id: recHead
                x: 14
                y: 8
                width: 52
                height: 52
                color: "#23cdfe"
                radius: 90
                clip: true
                MouseArea {
                    id: mouseHeadImg
                    width: parent.width
                    height: parent.height
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onPressed: {
                        if (signatureTF.focus) {
                            //如果可以 个性签名编辑就能完成
                            signatureTF.focus = !signatureTF.focus
                        }
                        //右键处理
                        if (pressedButtons == Qt.RightButton) {
                            //设置popup在窗口的坐标
                            headImgMenu.x = headImgMenu.posx + 5 + recHead.x
                            headImgMenu.y = headImgMenu.posy + 5 + recHead.y
                            headImgMenu.visible = true
                        } else {
                            actions.openAlterUserInfoAct.trigger() //打开个人资料界面
                        }
                    }
                    onHoveredChanged: {
                        if (containsMouse) {
                            actions.headImgAct.trigger() //调用相应动作
                        } else {
                            loaderForHeadImg.item.headImgHovered = false //标记为不悬浮
                        }
                    }
                    onPositionChanged: {
                        if (!headImgMenu.visible) {
                            headImgMenu.posx = mouseX
                            headImgMenu.posy = mouseY
                        }
                    }
                }

                Image {
                    id: imgHead
                    asynchronous: true
                    cache: false
                    fillMode: Image.PreserveAspectCrop
                    visible: false
                    sourceSize: Qt.size(parent.size, parent.size)
                }
                //遮罩
                OpacityMask {
                    width: parent.width
                    height: parent.height
                    source: imgHead
                    maskSource: recHead
                }
            }
            //头像右键弹出菜单
            Popup {
                property int posx: 0 //记录鼠标在头像里的x坐标
                property int posy: 0 //记录鼠标在头像里的y坐标
                id: headImgMenu
                width: 140
                height: 104
                visible: false
                background: Rectangle {
                    implicitWidth: 140
                    implicitHeight: 104
                    border.width: 1
                    border.color: "lightgray"
                    //修改个人资料
                    ToolButton {
                        id: alterInfoBtn
                        y: 6
                        width: headImgMenu.width
                        height: (headImgMenu.height - 12) / 3
                        text: "修改个人资料"
                        hoverEnabled: true
                        font.pointSize: 10
                        onClicked: {
                            headImgMenu.visible = false
                            actions.openAlterUserInfoAct.trigger() //打开个人资料界面
                        }
                        background: Rectangle {
                            width: headImgMenu.width
                            height: (headImgMenu.height - 12) / 3
                            color: alterInfoBtn.hovered ? "#eeeeee" : "transparent"
                        }
                    }
                    //更换头像
                    ToolButton {
                        id: alterHImgBtn
                        width: headImgMenu.width
                        height: (headImgMenu.height - 12) / 3
                        y: alterInfoBtn.y + alterInfoBtn.height
                        text: "更换头像"
                        hoverEnabled: true
                        font.pointSize: 10
                        onClicked: {
                            headImgMenu.visible = false
                            actions.openAlterHImgAct.trigger() //触发修改头像行为
                        }

                        background: Rectangle {
                            width: headImgMenu.width
                            height: (headImgMenu.height - 12) / 3
                            color: alterHImgBtn.hovered ? "#eeeeee" : "transparent"
                        }
                    }
                    ToolButton {
                        id: alterSSet
                        width: headImgMenu.width
                        height: (headImgMenu.height - 12) / 3
                        y: alterHImgBtn.y + alterHImgBtn.height
                        text: "修改系统设置"
                        hoverEnabled: true
                        font.pointSize: 10
                        onClicked: {
                            headImgMenu.visible = false
                        }
                        background: Rectangle {
                            width: headImgMenu.width
                            height: (headImgMenu.height - 12) / 3
                            color: alterSSet.hovered ? "#eeeeee" : "transparent"
                        }
                    }
                }
            }
            Label {
                property int maxWidth: signatureTF.width - gradeLab.width - 1
                property int w: maxWidth
                id: nameLab
                height: 24
                width: w
                x: recHead.x + recHead.width + 10
                y: recHead.y
                font.pointSize: 13
                font.family: song.name
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                maximumLineCount: 1
                text: name
                color: "#ffffff"
                onMaxWidthChanged: {
                    if (maxWidth > nameMetrics.advanceWidth) {
                        width = nameMetrics.advanceWidth
                    } else
                        width = maxWidth
                }
                TextMetrics {
                    id: nameMetrics
                    font: nameLab.font
                    text: name
                }
            }
            Label {
                property int posx: 0
                property int posy: 0
                id: gradeLab
                x: nameLab.x + nameLab.width + 1
                y: signatureTF.y - height - 1
                font.pointSize: 10
                font.family: song.name
                text: "lv" + grade
                color: "#b86030" //235 240 49
                background: Rectangle {
                    implicitHeight: 24
                    implicitWidth: 16
                    color: "transparent"
                    Label {
                        font.pointSize: 9
                        text: gradeLab.text
                        font.family: song.name
                        color: "#ebf031" //235 240 49
                    }
                }

                MouseArea {
                    width: parent.width
                    height: parent.height
                    hoverEnabled: true
                    onHoveredChanged: {
                        if (containsMouse) {
                            gradeTip.visible = true
                        } else {
                            tipTimer1sForGrd.restart()
                        }
                    }
                    onPressed: {
                        if (signatureTF.focus) {
                            signatureTF.focus = !signatureTF.focus
                        }
                    }
                    onPositionChanged: {
                        if (!gradeTip.visible) {
                            gradeLab.posx = mouseX
                            gradeLab.posy = mouseY
                        }
                        mouse.accepted = true
                    }
                }
            }
            //等级提示
            ToolTip {
                id: gradeTip
                x: gradeLab.x + gradeLab.posx
                y: gradeLab.y + gradeLab.posy + 20
                delay: 1000
                visible: false

                background: Label {
                    id: gradeBack
                    font.pointSize: 10
                    color: "gray"
                    text: "我的MQ等级:\n 等级:" + grade + "级\n 剩余升级天数:55天"
                    lineHeightMode: Text.FixedHeight //固定值模式
                    lineHeight: gradeMetrics.height + 5 //固定值为行高加5
                    verticalAlignment: Text.AlignVCenter
                    background: Rectangle {
                        implicitHeight: 52
                        implicitWidth: gradeMetrics.advanceWidth
                        color: Qt.rgba(230, 230, 230, 1.0)
                    }
                }
                TextMetrics {
                    id: gradeMetrics
                    text: " 剩余升级天数:55天"
                    font: gradeBack.font
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
                leftPadding: 0
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
                        signature = text //失去焦点 把文本赋值给个性签名
                        text = sigMetrics.elidedText
                        console.log(text)
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
                    text: signature
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
            Rectangle {
                id: weatherRec
                x: headerRec.width - width
                y: recHead.y
                width: 48
                height: 52
                color: "transparent"
                MouseArea {
                    width: 48
                    height: 52
                    hoverEnabled: true
                    onHoveredChanged: {
                        console.log(weatherCanShow)
                        if (containsMouse) {
                            actions.showWeatherAct.trigger()
                        } else {
                            actions.hideWeatherAct.trigger()
                        }
                    }
                    onPressed: {
                        if (signatureTF.focus) {
                            signatureTF.focus = !signatureTF.focus
                        }
                    }
                }
                Image {
                    id: imgWeather
                    sourceSize: Qt.size(48, 52)
                    source: "qrc:/images/mainInterface/wind.png"
                }
            }
        }
        //搜索部分
        Rectangle {
            id: seachRec
            x: bodyRec.border.width
            y: headRec.y + headRec.height
            width: headerRec.width
            height: 30
            color: Qt.lighter("#53cbfd", 1.2)
            RowLayout {
                x: 14
                Image {
                    id: imgSearch
                    sourceSize: Qt.size(13, 13)
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/images/mainInterface/iconSearch.png"
                }
                Rectangle {
                    width: headerRec.width - 38 - 29
                    height: 30
                    TextField {
                        width: parent.width
                        height: parent.height
                        color: "white"
                        placeholderText: "搜索"
                        font.pointSize: 10
                        font.family: song.name
                        background: Rectangle {
                            implicitWidth: parent.width
                            implicitHeight: parent.height
                            color: Qt.lighter("#53cbfd", 1.2)
                        }
                    }
                }
                Image {
                    id: imgFork
                    // sourceSize: Qt.size(16, 16)
                    source: "qrc:/images/mainInterface/frok.png"
                }
                ColorOverlay {
                    id: colorImgSearch
                    anchors.fill: imgSearch
                    source: imgSearch
                    color: "white"
                }
            }
        }
        //联系人和消息栏
        RowLayout {
            x: bodyRec.border.width
            y: seachRec.y + seachRec.height
            spacing: 0
            Button {
                id: contactBtn
                text: qsTr("联系人")
                checkable: true
                checked: true
                font.pointSize: 12
                font.family: song.name
                onPressed: {
                    checked = true
                    informationBtn.checked = false
                    container.currentIndex = 0
                }
                onReleased: {
                    console.log("rel")
                    container.currentIndex = 0
                    checked = true
                    informationBtn.checked = false
                }
                contentItem: Text {
                    text: contactBtn.text
                    font: contactBtn.font
                    color: contactBtn.checked ? "#23cdfe" : contactBtn.hovered ? "black" : "gray"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: headerRec.width / 2
                    implicitHeight: 40
                }
                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    visible: contactBtn.checked
                    width: parent.width - 20
                    height: 2
                    color: "#23cdfe"
                }
            }
            Button {
                id: informationBtn
                text: qsTr("消息")
                checkable: true
                font.pointSize: 12
                font.family: song.name
                onPressed: {
                    checked = true
                    contactBtn.checked = false
                    container.currentIndex = 1
                }
                onReleased: {
                    container.currentIndex = 1
                    checked = true
                    contactBtn.checked = false
                    console.log("viaed to release of mouse")
                }
                contentItem: Text {
                    text: informationBtn.text
                    font: informationBtn.font
                    color: informationBtn.checked ? "#23cdfe" : informationBtn.hovered ? "black" : "gray"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    implicitWidth: headerRec.width / 2
                    implicitHeight: 40
                }
                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    visible: informationBtn.checked
                    width: parent.width - 20
                    height: 2
                    color: "#23cdfe"
                }
            }
        }
        //分割线
        Rectangle {
            id: separator
            x: bodyRec.border.width
            y: seachRec.y + seachRec.height + informationBtn.height
            width: headerRec.width
            height: 1
            color: "lightgray"
        }
        //可滚动区
        Container {
            id: container
            x: bodyRec.border.width
            y: separator.y + separator.height
            width: headerRec.width
            height: bodyRec.height - y - footerRec.height - bodyRec.border.width
            currentIndex: 0
            onCurrentIndexChanged: {
                recContact.visible = !recContact.visible
                scrInformation.visible = !scrInformation.visible
            }
            contentItem: ListView {
                snapMode: ListView.SnapOneItem
                orientation: ListView.Vertical
                highlightMoveVelocity: 100
                currentIndex: container.currentIndex
                interactive: false
            }

            Rectangle {
                id: recContact
                visible: true
                width: headerRec.width
                height: headerRec.height - 140 - 40
                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents: true
                }
                RowLayout {
                    id: rowContactList
                    x: 10
                    height: 40
                    Button {
                        id: friendBtn
                        checkable: true
                        checked: true
                        text: qsTr("好友")
                        font.pointSize: 11
                        font.family: song.name
                        onPressed: {
                            groupChatBtn.checked = false
                            scrGroupChat.visible = false
                            checked = true
                            scrFriend.visible = true
                            if (signatureTF.focus) {
                                signatureTF.focus = !signatureTF.focus
                            }
                        }
                        onReleased: {
                            groupChatBtn.checked = false
                            checked = true
                        }
                        contentItem: Text {
                            text: friendBtn.text
                            font: friendBtn.font
                            color: friendBtn.checked ? "#23cdfe" : friendBtn.hovered ? "black" : "gray"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        background: Rectangle {
                            implicitWidth: 62
                            implicitHeight: 26
                            color: friendBtn.checked ? "#ecfaff" : "white"
                            Image {
                                anchors.right: parent.right
                                sourceSize: Qt.size(9, 9)
                                fillMode: Image.PreserveAspectCrop
                                visible: (friendBtn.checked & friendBtn.hovered)
                                source: "qrc:images/mainInterface/arrowBottom.png"
                            }
                        }
                    }
                    Button {
                        id: groupChatBtn
                        checkable: true
                        checked: false
                        text: qsTr("群聊")
                        font.pointSize: 11
                        font.family: song.name
                        onPressed: {
                            friendBtn.checked = false
                            scrFriend.visible = false
                            checked = true
                            scrGroupChat.visible = true

                            if (signatureTF.focus) {
                                signatureTF.focus = !signatureTF.focus
                            }
                        }
                        onReleased: {
                            friendBtn.checked = false
                            checked = true
                        }
                        contentItem: Text {
                            text: groupChatBtn.text
                            font: groupChatBtn.font
                            color: groupChatBtn.checked ? "#23cdfe" : groupChatBtn.hovered ? "black" : "gray"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        background: Rectangle {
                            implicitWidth: 62
                            implicitHeight: 26
                            color: groupChatBtn.checked ? "#ecfaff" : "white"
                            Image {
                                anchors.right: parent.right
                                sourceSize: Qt.size(9, 9)
                                fillMode: Image.PreserveAspectCrop
                                visible: (groupChatBtn.checked & groupChatBtn.hovered)
                                source: "qrc:/images/mainInterface/arrowBottom.png"
                            }
                        }
                    }
                }
                RowLayout {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    height: 40
                    Rectangle {
                        width: 1
                        height: 18
                        color: "lightgray"
                    }
                    Image {
                        id: imgCrosss
                        sourceSize: Qt.size(24, 24)
                        fillMode: Image.PreserveAspectCrop
                        source: "qrc:/images/mainInterface/cross.png"
                    }
                }
                ScrollView {
                    id: scrFriend
                    visible: true
                    y: 40
                    width: headerRec.width
                    height: container.height - 40
                    hoverEnabled: true
                    contentWidth: headerRec.width
                    contentHeight: viewFriend.height
                    //滚动栏
                    ScrollBar.vertical: ScrollBar {
                        parent: scrFriend //必须设置父对象才能启动hovered and pressed以及鼠标事件
                        anchors.right: parent.right //绑定右边
                        orientation: Qt.Vertical
                        policy: ScrollBar.AlwaysOn // (contentHeight > container.height
                        //- 40) ? (hovered ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff) : ScrollBar.AlwaysOff
                        size: scrFriend.height / scrFriend.contentHeight
                        hoverEnabled: true
                        active: true
                        contentItem: Rectangle {
                            implicitWidth: 6
                            implicitHeight: parent.size * scrFriend.height
                            radius: width / 2
                            color: parent.hovered ? Qt.darker("#d7d7d7",
                                                              1.3) : "#d7d7d7"
                        }
                    }

                    contentItem: ListView {
                        id: viewFriend
                        model: friendGroupModel

                        delegate: Rectangle {
                            property alias delegateModel: listFriend.model
                            property alias friend: listFriend
                            property int pos: -1
                            width: headerRec.width
                            height: listGroup.height

                            //组
                            List {
                                id: listGroup
                                w: headerRec.width
                                h: 35
                                ratio.visible: false
                                name.text: r_group
                                sumForFriends: r_count
                                activeLine: r_online
                                onClicked: {
                                    if (rotat === 0) {
                                        rotat = 90
                                    } else {
                                        rotat = 0
                                    }
                                    backColor = "transparent"
                                    if (parent.friend.count === 0)
                                        return
                                    parent.friend.visible = !parent.friend.visible
                                }
                            }
                            //好友表
                            ListView {
                                id: listFriend
                                anchors.top: listGroup.bottom
                                visible: false
                                onVisibleChanged: {
                                    if (visible)
                                        parent.height += height
                                    else
                                        parent.height -= height
                                }

                                delegate: Friend {
                                    Component.onCompleted: {
                                        parent.parent.height = parent.parent.count * 60
                                    }
                                    w: headerRec.width
                                    h: 60
                                    headImg: r_imgPath
                                    topText: setInfo["列表"] === "0" ? (r_tag === "" ? r_name : r_tag + "(" + r_name + ")") : r_name
                                    bottomText: r_signature
                                }
                            }

                            Component.onCompleted: {
                                friendDelegateCount += 1
                            }
                        }
                    }
                }
                ScrollView {
                    id: scrGroupChat
                    visible: false
                    y: 40
                    width: headerRec.width
                    height: container.height - 40
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: (contentHeight > container.height
                                                - 40) ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    ListView {
                        id: viewGroupChat
                        delegate: List {
                            width: headerRec.width
                            height: 35
                            name.text: qsTr("我的群聊")
                            name.font.weight: Font.Black
                        }
                    }
                }
            }
            ScrollView {
                id: scrInformation
                anchors.fill: parent
                visible: false
                y: 40
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: (contentHeight > container.height) ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                Label {
                    x: scrInformation.width / 2 - width / 2
                    y: 60
                    font.pointSize: 9
                    font.family: song.name
                    text: qsTr("暂时没有会话")
                    color: "gray"
                }
            }
        }
        Rectangle {
            id: footerRec
            x: bodyRec.border.width
            y: bodyRec.height - height - bodyRec.border.width
            height: 40
            width: headerRec.width
            color: "white"
            MouseCustomForWindow {
                onPressed: {
                    startPoint.x = mouseX
                    startPoint.y = mouseY
                    console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
                    if (signatureTF.focus) {
                        signatureTF.focus = !signatureTF.focus
                    }
                }
                onSendPos: {
                    qqMainWin.x += movedCoordinate.x
                    qqMainWin.y += movedCoordinate.y
                }
            }
            RowLayout {
                id: bottomLay
                x: 5
                spacing: 0
                ToolButton {
                    id: mainMenuBtn
                    width: 28
                    height: 40
                    background: Rectangle {
                        implicitWidth: parent.width
                        implicitHeight: parent.height
                        color: mainMenuBtn.hovered ? "#f2f2f2" : "white"
                        Image {
                            id: imgMainMenu
                            anchors.centerIn: parent
                            sourceSize: Qt.size(28, 40)
                            source: "qrc:/images/mainInterface/mainMenu.png"
                        }
                    }
                    onPressed: actions.mainMenuAct.trigger()
                }
                ToolButton {
                    id: addFriendBtn
                    width: 28
                    height: 40
                    action: actions.addFriendsAct
                    background: Rectangle {
                        implicitWidth: parent.width
                        implicitHeight: parent.height
                        color: addFriendBtn.hovered ? "#f2f2f2" : "white"
                        Image {
                            id: imgAddFriend
                            sourceSize: Qt.size(28, 40)
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/images/mainInterface/addFriend.png"
                        }
                    }
                }
            }
            Rectangle {
                implicitWidth: parent.width
                implicitHeight: 1
                color: "lightgray"
            }
        }
        //菜单栏
        Popup {
            id: menuPop
            x: bodyRec.border.width + 1
            y: bodyRec.height - height - footerRec.height - 1
            width: 142
            height: 217
            clip: true
            focus: true
            padding: 1
            onVisibleChanged: {
                if (!visible)
                    helpBtn.isShowSubMenu = false
            }

            background: Rectangle {
                border.width: 1
                border.color: "lightgray"
            }
            contentItem: Column {
                spacing: 3
            }
            Row {
                //我的收藏
                ToolButton {
                    id: collectBtn
                    hoverEnabled: true
                    onHoveredChanged: {
                        if (hovered)
                            sendLeaveForMenuBtnItems()
                    }
                    background: Rectangle {
                        implicitHeight: 80
                        implicitWidth: 70
                        color: collectBtn.hovered ? "#f2f2f2" : "white"
                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 3
                            Image {
                                Layout.alignment: Qt.AlignHCenter
                                source: "qrc:/images/mainInterface/colection.png"
                            }
                            Label {
                                Layout.alignment: Qt.AlignHCenter
                                text: qsTr("我的收藏")
                                font.family: song.name
                                font.pointSize: 9
                            }
                        }
                        Rectangle {
                            y: parent.height
                            width: parent.width * 2
                            height: 1
                            color: "lightgray"
                        }
                    }
                }
                //信息管理
                ToolButton {
                    id: inforManagerBtn
                    hoverEnabled: true
                    onHoveredChanged: {
                        if (hovered)
                            sendLeaveForMenuBtnItems()
                    }
                    background: Rectangle {
                        implicitHeight: 80
                        implicitWidth: 70
                        color: inforManagerBtn.hovered ? "#f2f2f2" : "white"
                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 3
                            Image {
                                Layout.alignment: Qt.AlignHCenter
                                source: "qrc:/images/mainInterface/inforManager.png"
                            }
                            Label {
                                Layout.alignment: Qt.AlignHCenter
                                text: qsTr("消息管理")
                                font.family: song.name
                                font.pointSize: 9
                            }
                        }
                    }
                }
            }
            //帮助
            ToolButton {
                property bool isShowSubMenu: false
                id: helpBtn
                hoverEnabled: true
                onIsShowSubMenuChanged: {
                    if (isShowSubMenu) {
                        if (menuItemTip.status === Loader.Null)
                            menuItemTip.source = "qrc:/main/HelpWin.qml"
                        time1s.restart()
                        var x = 147 + qqMainWin.x
                        var y = qqMainWin.y + qqMainWin.height - 180
                        qqMainWin.sendCoordinate(Qt.point(x, y))
                    } else {
                        time1s.stop()
                        menuItemTip.item.visible = false
                        //menuItemTip.source = ""
                    }
                }

                onHoveredChanged: {
                    if (hovered)
                        isShowSubMenu = true
                }

                background: Rectangle {
                    implicitWidth: 140
                    implicitHeight: 30
                    color: helpBtn.isShowSubMenu ? "#f2f2f2" : "white"
                    Label {
                        x: 25
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("帮助")
                    }
                    Image {
                        x: parent.width - width - 10
                        y: parent.height / 2 - height / 2
                        sourceSize: Qt.size(11, 11)
                        source: "qrc:/images/mainInterface/arrowLeft.png"
                    }
                }
                Timer {
                    id: time1s
                    interval: 1000
                    repeat: false
                    running: false
                    onTriggered: {
                        console.log(menuItemTip.item.visible, "->1")
                        menuItemTip.item.visible = true
                        console.log(menuItemTip.item.visible, "->2")
                    }
                }

                Connections {
                    target: qqMainWin
                    onSendLeaveForMenuBtnItems: {
                        if (!helpBtn.hovered)
                            helpBtn.isShowSubMenu = false
                    }
                }
            }
            //设置
            ToolButton {
                id: setBtn
                hoverEnabled: true
                onHoveredChanged: {
                    if (hovered) {
                        sendLeaveForMenuBtnItems()
                    }
                }
                background: Rectangle {
                    implicitWidth: 140
                    implicitHeight: 30
                    color: setBtn.hovered ? "#f2f2f2" : "white"
                    Label {
                        x: 25
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("设置")
                    }
                }
            }
            Rectangle {
                width: 140
                height: 1
                color: "lightgray"
            }
            //切换账号
            ToolButton {
                id: switchBtn
                hoverEnabled: true
                onHoveredChanged: {
                    if (hovered)
                        sendLeaveForMenuBtnItems()
                }
                action: actions.switchAct
                background: Rectangle {
                    implicitWidth: 140
                    implicitHeight: 30
                    color: switchBtn.hovered ? "#f2f2f2" : "white"
                    Label {
                        x: 25
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("切换账号")
                    }
                }
            }
            //退出
            ToolButton {
                id: exitBtn
                hoverEnabled: true
                onHoveredChanged: {
                    if (hovered)
                        sendLeaveForMenuBtnItems()
                }
                onClicked: {
                    Qt.quit()
                }
                background: Rectangle {
                    implicitWidth: 140
                    implicitHeight: 30
                    color: exitBtn.hovered ? "#f2f2f2" : "white"
                    Image {
                        y: parent.height / 2 - height / 2
                        sourceSize: Qt.size(25, 24)
                        source: "qrc:/images/mainInterface/exitIcon.png"
                    }
                    Label {
                        x: 25
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("退出")
                    }
                }
            }
        }
    }

    //界面
    //天气
    Loader {
        id: loaderForWeather
        visible: Loader.Ready === loaderForMesWin.status
    }
    //账号切换提示窗
    Loader {
        id: loaderForMesWin
        visible: Loader.Ready === loaderForMesWin.status
    }
    //菜单栏
    Loader {
        id: menuItemTip
    }
    //关于MyQQ
    Loader {
        id: loaderForAbout
        visible: Loader.Ready === loaderForAbout.status
    }
    //头像界面
    Loader {
        id: loaderForHeadImg
        visible: Loader.Ready === loaderForAbout.status
    }
    //更改头像界面
    Loader {
        id: loaderForAlterHImg
        visible: Loader.Ready === loaderForAlterHImg.status
    }
    //修改用户资料界面
    Loader {
        id: loaderForAlterInfo
        visible: Loader.Ready === loaderForAlterInfo.status
    }

    //非界面
    //城市天气模型 保存最多4个城市天气
    ListModel {
        id: cityWeatherModel
    }
    //好友组模型
    FriendGroupModel {
        id: friendGroupModel
    }
    //分组好友模型
    Component {
        id: compFriend
        FriendModel {}
    }
    //模型数据 历史头像
    ListModel {
        property bool isgot: false
        id: histroyImgModel
        onCountChanged: {
            console.log("historymodel count changed")
            imgHead.source = "image://history/" + myqq + "101" //设置头像 qimage源
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
    //等级
    Timer {
        id: tipTimer1sForGrd
        interval: 1000
        onTriggered: {
            gradeTip.close()
        }
    }

    //鼠标拉伸
    //左上角
    Item {
        id: leftTop
        width: enableSize
        height: enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, qqMainWin)
            }
            onEntered: func.enter(1, qqMainWin)
            onReleased: func.release(qqMainWin)
            onPositionChanged: func.positionChange(mouse, -1, -1, qqMainWin)
        }
    }

    //Top
    Item {
        id: top
        height: enableSize
        width: qqMainWin.width - enableSize * 2
        x: enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, qqMainWin)
            }
            onEntered: func.enter(2, qqMainWin)
            onReleased: func.release(qqMainWin)

            onMouseYChanged: func.positionChange(Qt.point(customPoint.x,
                                                          mouseY), 1, -1,
                                                 qqMainWin)
        }
    }

    //右上角
    Item {
        id: rightTop
        width: enableSize
        height: enableSize
        x: qqMainWin.width - enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, qqMainWin)
            }
            onEntered: func.enter(3, qqMainWin)
            onReleased: func.release(qqMainWin)
            onPositionChanged: func.positionChange(mouse, 1, -1, qqMainWin)
        }
    }

    //Left
    Item {
        id: left
        width: enableSize
        height: qqMainWin.height - 2 * enableSize
        y: enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, qqMainWin)
            }
            onEntered: func.enter(4, qqMainWin)
            onReleased: func.release(qqMainWin)

            onMouseXChanged: func.positionChange(Qt.point(mouseX,
                                                          customPoint.y), -1,
                                                 1, qqMainWin)
        }
    }

    //Right
    Item {
        id: right
        width: enableSize
        height: qqMainWin.height - 2 * enableSize
        x: qqMainWin.width - enableSize
        y: enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, qqMainWin)
            }
            onEntered: func.enter(6, qqMainWin)
            onReleased: func.release(qqMainWin)

            onMouseXChanged: func.positionChange(Qt.point(mouseX,
                                                          customPoint.y), 1, 1,
                                                 qqMainWin)
        }
    }

    //左下角
    Item {
        id: leftBottom
        width: enableSize
        height: enableSize
        y: qqMainWin.height - enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, qqMainWin)
            }
            onEntered: func.enter(7, qqMainWin)
            onReleased: func.release(qqMainWin)

            onPositionChanged: func.positionChange(mouse, -1, 1, qqMainWin)
        }
    }

    //bottom
    Item {
        id: bottom
        height: enableSize
        width: qqMainWin.width - 2 * enableSize
        x: enableSize
        y: qqMainWin.height - enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, qqMainWin)
            }
            onEntered: func.enter(8, qqMainWin)
            onReleased: func.release(qqMainWin)

            onMouseYChanged: func.positionChange(Qt.point(customPoint.x,
                                                          mouseY), 1, 1,
                                                 qqMainWin)
        }
    }

    //右下角
    Item {
        id: rightBottom
        width: enableSize
        height: enableSize
        x: qqMainWin.width - enableSize
        y: qqMainWin.height - enableSize
        z: 10
        MouseArea {
            width: parent.width
            height: parent.height
            hoverEnabled: true

            onPressed: {
                if (signatureTF.focus) {
                    signatureTF.focus = !signatureTF.focus
                }
                func.press(mouse, qqMainWin)
            }
            onEntered: func.enter(9, qqMainWin)
            onReleased: func.release(qqMainWin)

            onPositionChanged: func.positionChange(mouse, 1, 1, qqMainWin)
        }
    }
    Component.onCompleted: {
        //初始化
        console.log(headerRec.height)
        mainWin.width = qqMainWin.width
        mainWin.height = qqMainWin.height
        myqq = mainWin.myqq
        funcc.connectGetFile("2 headImg " + myqq)
        funcc.initWh()
        funcc.mkDir(("../user/" + mainWin.myqq + "/weather"))
    }
}
