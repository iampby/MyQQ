import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.11
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.2
import Model 1.0
import "qrc:/"

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
    property alias loaderForVerify: loaderForVerify //打开验证页面
    property alias loaderForOffline: loaderForOffline //弹出下线框
    property alias loaderForAlterTag: loaderForAlterTag //修改好友备注
    property alias loaderForChat: loaderForChat //聊天界面

    property alias cityWeatherModel: cityWeatherModel //保存3天天气信息
    property int weatherCanShow: 0 //2表示准备好
    property bool isinit: true //用来避免第一次用户信息变化是发送更新信号
    property alias friendGroupModel: friendGroupModel //好友组模型
    property alias histroyImgModel: histroyImgModel //历史头像model
    property alias gradeModel: gradeModel //用户等级模型
    property alias viewFriend: viewFriend //好友视图
    property var mapInfo: new Map() //好友资料列表 map
    property var mapChat: new Map() //聊天界面列表 map 不模仿为统合界面

    property var initVerifyInfo: null //初始化时一个临时验证消息载体
    property var initInfoList: null //初始化时一个临时聊天信息map载体

    property var friendsModel: [] //保存好友模型，用来过渡且便于使用
    //好友代理数，由于代理动态加载，在friendGroupModel添加数据过程中一直处于线程忙碌，无法直接构造，所以标记是否构造完用来加载好友模型
    property int friendDelegateCount: 0 //好友组实例化数，用于有效添加好友数据
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
    property int sum: 0 //好友数
    property bool hasMyself: false
    property int fgIndex: -1 //自己处于的好友组位置

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

    property bool isOffline: false //是否已离线

    signal move(point movedCoordinate)
    signal sendLeaveForMenuBtnItems
    signal sendCoordinate(point pos)
    //移动坐标处理信号，参数为鼠标位置偏差
    signal runGetUserInfo(var obj)
    //信息： name（昵称） sex（性别） signature（个性签名） days（活跃天数） grade（等级) status(状态） 所在地 故乡
    //获得一个好友信息,其位于第pos个数组中，0 开始
    signal runGetFriendInfoFunction(var obj, int pos)
    //获取设置信息
    signal runGetSetInfoFunction(var obj)
    //添加好友组到末尾
    signal addFGroup(string value)
    //当接收辅助进程打开陌生人资料
    //接收辅助进程信号，打开陌生人资料界面
    signal openFWin(var obj)

    //退出程序信号
    signal aboutToQuit
    //打开信息对话框
    signal clickedInfo(var id)

    //更新好友备注
    signal updateTag(var obj)
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
    //如果包含自己为好友 则本地好友昵称更新
    onNameChanged: {
        if (hasMyself) {
            for (var i = 0; i < friendsModel.length; ++i) {
                var m = friendsModel[i]
                var row = m.rowOf(myqq)
                if (row !== -1) {
                    console.log("find myself,the number is ", myqq)
                    m.setData(row, name, 1)
                    console.log("updated show: ", name)
                    console.log("end update")
                    break
                }
            }
        }
    }
    //个性签名更新
    onSignatureChanged: {
        console.log("onSignatureChanged:")
        if (isinit) {
            isinit = false
            return
        }
        //5 更新个性签名
        var ins = "5 updateSianature  " + myqq + "  headerSize"
        funcc.updateSignature(qqMainWin.signature, ins)
        //本地更新
        if (hasMyself) {
            for (var i = 0; i < friendsModel.length; ++i) {
                var m = friendsModel[i]
                var row = m.rowOf(myqq)
                if (row !== -1) {
                    console.log("find myself,the number is ", myqq)
                    m.setData(row, signature, 2)
                    console.log("updated show: ", signature)
                    console.log("end update")
                    break
                }
            }
        }
        signatureTF.text = signature //更新显示签名
    }

    //利用元对象信号槽通信机制，把参数从Qtc++->qml 获取user信息
    onRunGetUserInfo: {
        console.log("onRunGetUserInfo")
        //信息： name（昵称） sex（性别） signature（个性签名） days（活跃天数） grade（等级) status(状态） 所在地 故乡
        name = obj["name"]
        sex = obj["sex"]
        signature = obj["signature"]
        if (signature == "")
            isinit = false //如果不触发第一次变化则设置为false
        activeDays = obj["days"]
        grade = obj["grade"]
        //grade = 300
        status = "1" //obj["status"]
        where = obj["所在地"]
        townmans = obj["故乡"]
        var tag = qqMainWin.myqq + "101"
        images.setPixmap(tag, obj["headUrl"]) //好友号码+1
        histroyImgModel.append({
                                   "url": "image://history/" + tag,
                                   "index": 0
                               })
        gradeTip.days = func.computeToUpgrade(grade, activeDays)
        func.resetGradeModel(gradeModel, grade)
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
        var status = obj["状态"]
        var grade = obj["等级"]
        grade = qqMainWin.grade
        if (myqq == mq) {
            //自己总是在线
            status = "1"
            hasMyself = true
        }
        images.setPixmap2(mq + "1", imgPath, status)
        //添加数据到模型
        //qvariantmap转过来的只能[string]获取属性
        console.log("obj[\"个性签名\"]=", obj["个性签名"])
        var url = "image://friends/" + mq + "1"

        m.append(mq, obj["昵称"], obj["个性签名"], url, obj["备注"], grade, status,
                 obj["Message - Settin"], obj["Status_Settin"])
        sum += 1 //好友数加一
    }
    //利用元对象信号槽通信机制，把参数从Qtc++->qml,获取设置信息
    onRunGetSetInfoFunction: {
        console.log("obj")
        setInfo = obj //保存设置信息
    }

    //窗口关闭处理
    onClosing: {
        console.log("closing")
        realse() //释放资源
        //强制下线不推送远程
        if (!isOffline)
            funcc.exitMyQQ(qqMainWin) //发送退出到远程
        else {
            aboutToQuit()
        }
    }
    //程序退出处理
    onAboutToQuit: {
        //退出
        console.log("exit app")
        Qt.quit()
    }
    //窗口激活变化隐藏 2个拖拽便条
    onActiveChanged: {
        if (!active) {
            if (loaderForFG.status == Loader.Ready) {
                loaderForFG.item.close()
            }
            if (loaderForFriend.status == Loader.Ready) {
                loaderForFriend.item.close()
            }
        }
    }

    //2个拖拽便条随窗口位置变化而不可视
    onXChanged: {
        if (loaderForFG.status != Loader.Null) {
            loaderForFG.item.close()
        }
        if (loaderForFriend.status != Loader.Null) {
            loaderForFriend.item.close()
        }
    }
    onYChanged: {
        if (loaderForFG.status != Loader.Null) {
            loaderForFG.item.close()
        }
        if (loaderForFriend.status != Loader.Null) {
            loaderForFriend.item.close()
        }
    }

    //2个拖拽便条随窗口可视变化而不可视
    onVisibleChanged: {
        if (loaderForFG.status != Loader.Null) {
            loaderForFG.item.close()
        }
        if (loaderForFriend.status != Loader.Null) {
            loaderForFriend.item.close()
        }
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
        var length = friendGroupModel.count()
        //添加模型
        if (friendDelegateCount === length) {
            var listM = viewFriend.contentItem.children
            for (var i = 0; i < length; i++) {
                friendsModel[i].sort()
                listM[i].delegateModel = friendsModel[i]
                listM[i].pos = i
            }
            if (fgMenu.addIndex != -1) {
                scrFriend.closeAllHovered()
                if (loaderForFG.status == Loader.Ready)
                    loaderForFG.item.close()
                var item = listM[fgMenu.addIndex]
                scrFriend.closeAllHovered()
                item.edit.text = item.listGroup.name.text
                item.edit.visible = true
                item.muter = true
                item.edit.forceActiveFocus()
            }
            friendGroupModel.update(0, 1000)
        }
    }
    //添加好友组
    onAddFGroup: {
        if (friendGroupModel.count() > 15) {
            console.log("the number of friend group is not more than fifteen")
            return
        }
        loaderForFG.item.hide()
        friendGroupModel.insert(friendGroupModel.rowCount() - 1, value, "",
                                "", "none")
        console.log("added a friends group is successful", value)
    }
    //点击信息
    onClickedInfo: {
        tray.reset()
        //打开验证信息界面
        if (id === "-1") {
            actions.openVerifyAct.trigger(qqMainWin)
            //打开好友聊天界面
        } else {
            let number = id.substring(0, id.length - 1)
            let end = id.substring(id.length - 1, id.length)
            try {
                if (end == "1") {
                    //打开好友窗口
                    //console.log("发送即时消息")
                    sentFriendInfo(number)
                }
            } catch (err) {
                console.log(err.message)
                return
            }
        }
    }

    //更新好友备注
    onUpdateTag: {
        funcc.alterFTag(obj) //远程更新
        var index = obj.index
        var tag = obj.tag
        var number = obj.number
        var m = friendsModel[index]
        m.setData(m.rowOf(number), tag, 4)
    }
    //打开陌生人资料界面
    onOpenFWin: {
        console.log("onOpenFWin:")
        try {
            obj.fgIndex = -1
            obj.tag = "" //陌生人没有备注
        } catch (e) {
            console.log("warning:", e.message)
        }
        openFriendDataWin(obj)
    }


    //target func
    Connections {
        target: func

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
            console.log(name)
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
                    console.log("created a FriendModel")
                    break
                } else if (compFriend.status === Component.Error) {
                    console.log("compFriend created had failed")
                    break
                }
            }
        }
        //好友界面信息更改处理
        onUpdateFriendsModel: {
            var length = friendsModel.length
            for (var i = 0; i < length; ++i) {
                console.log("onUpdateFriendsModel:i=", i)
                var m = friendsModel[i]
                var row = m.rowOf(number)
                if (row !== -1) {
                    console.log("start update:MyQQ is", number)
                    //if (role === 3)
                    //  m.setData(row, "", role) //图片刷新
                    //不刷新 让后面状态处理整个模型换掉刷新才能正常显示灰度图
                    m.setData(row, value, role)
                    if (role == 6) {
                        images.flushPixmap2(number + "1", value)
                    }
                    console.log("end update")
                }
            }
        }
        //更新全部好友模型
        onUpdateTotalFModel: {
            console.log("update total friend models")
            var length = friendsModel.length
            for (var i = 0; i < length; ++i) {
                let m = friendsModel[i]
                //有内容换掉模型
                if (m.rowCount() > 0) {
                    m.sort() //排序
                    flushFImage(i) //换掉整个模型
                }
            }
            //把好友小便条刷新一下
            if (loaderForFriend.status == Loader.Ready) {
                var item = loaderForFriend.item
                var img = item.img
                item.img = ""
                item.img = img
            }
        }

        //初始化好友消息列表
        onGetReceiveFMessage: {
            console.log("init the information list and verify message object ")
            //先保存对象信息，等界面初始化完成就处理对象
            initInfoList = iobj
            initVerifyInfo = vobj
        }
        //处理消息列表和验证消息
        onEmitHandleVerifyAndInfoList: {
            console.log("handling the verify message and friend message list")
            let hasInfo = false
            //不为空添加
            if (!objIsEmpty(initVerifyInfo)) {
                console.log("verify information not empty")
                hasInfo = true
                let time = initVerifyInfo["time"]
                let tip = initVerifyInfo["name"] + "添加你为好友"
                infoModel.insert(0, {
                                     "r_imgPath": "qrc:/images/bellinfo.png",
                                     "r_title": "验证消息",
                                     "r_tip": tip,
                                     "r_time": time,
                                     "r_number": "-1"
                                 })
            }
            // number,info
            if (!objIsEmpty(initInfoList)) {
                console.log("information list not empty")
                try {
                    for (let i in initInfoList) {
                        let t = initInfoList[i]
                        console.log("???", t)
                        let time = t.substring(t.lastIndexOf(";;") + 2,
                                               t.length)
                        t = t.substring(0, t.lastIndexOf(";;"))
                        let name
                        for (let index in friendsModel) {
                            let m = friendsModel[index]
                            index = m.rowOf(i)
                            if (index != -1) {
                                name = m.data(index, 1)
                                let tag = m.data(index, 4)
                                if (tag != "") {
                                    name = tag
                                }
                                break
                            }
                        }
                        if (name == undefined)
                            name = ""
                        infoModel.insert(0, {
                                             "r_imgPath"//id=好友id+1
                                             : "image://friends/" + i + "1",
                                             "r_title": name,
                                             "r_tip": t,
                                             "r_time": time,
                                             "r_number": i + "1" //+1代表好友号码
                                         })
                        hasInfo = true
                    }
                } catch (e) {
                    console.log("warning:", e.message)
                }
            }
            infoModel.sort() //时间大到小排序
            if (hasInfo) {
                try{
                    //头像闪烁
                    let himg=infoModel.get(0).r_imgPath
                    tray.iconSource = himg
                    tray.midSource = himg
                    tray.tooltip = "有新消息到来"
                    timer1s.restart()
                    tray.hasInfo = true //有信息标记
                    //funcc.emitFVeify()//信号提醒托盘有新消息到来
                }catch(e){
                    console.log("error "+e)
                }
            }
            //对所有好友排序
            funcc.updateTotalFModel()
        }

        //添加好友验证消息
        onEmitFVeify: {
            var length = infoModel.count
            var existis = false
            for (var i = 0; i < length; ++i) {
                //验证信息则 number标记为-1
                if (infoModel.get(i).r_number == -1) {
                    existis = true
                    infoModel.set(i, {
                                      "r_imgPath": "qrc:/images/bellinfo.png",
                                      "r_title": "验证消息",
                                      "r_tip": obj["name"] + "添加你为好友",
                                      "r_time": obj["time"],
                                      "r_number": "-1"
                                  })
                    infoModel.move(i, 0, 1)
                    //如果打开验证窗口 则刷新
                    if (loaderForVerify.item.visible) {
                        var item = infoView.contentItem.children[0]
                        item.clicked()
                    }
                }
            }
            if (!existis) {
                infoModel.insert(0, {
                                     "r_imgPath": "qrc:/images/bellinfo.png",
                                     "r_title": "验证消息",
                                     "r_tip": obj["name"] + "添加你为好友",
                                     "r_time": obj["time"],
                                     "r_number": "-1"
                                 })
            }
            infoTip.visible = false
        }
        //添加好友处理 obj
        onEmitFriend: {
            console.log("onEmitFriend")
            //obj 全部数据
            var grade = obj["grade"]
            var groupName = obj["groupName"]
            var infoSet = obj["infoSet"]
            var name = obj["name"]
            var number = obj["number"]
            var signature = obj["signature"]
            var status = obj["status"]
            var statusSet = obj["statusSet"]
            var tag = obj["tag"]
            var length = friendGroupModel.count()
            var imgPath = "image://friends/" + number + "1"
            for (var i = 0; i < length; ++i) {
                if (friendGroupModel.data(i) == groupName) {
                    var m = friendsModel[i]
                    console.log("find the group name:", groupName)
                    m.append(number, name, signature, imgPath, tag, grade,
                             status, infoSet, statusSet)
                    viewFriend.model = null
                    friendDelegateCount = 0
                    viewFriend.model = friendGroupModel
                    //如果好友辅助添加进程打开，则从界面中刷掉已添加的好友项
                    funcc.emitDeletionExternalItemOf(number)
                    //friendGroupModel.update(i) //更新好友列表
                    //如果有打开的陌生人资料界面 刷新打开的资料界面
                    let tw=qqMainWin.mapInfo.get(number+"1")
                    if(tw){
                        tw.fgIndex=i
                        tw.last=i
                    }
                    break
                }
            }
        }
        //下线通知
        onEmitOffline: {
            loaderForOffline.obj = obj //赋值ip host time信息
            if (!qqMainWin.visible) {
                qqMainWin.show()
                qqMainWin.raise()
                qqMainWin.requestActivate()
            }
            qqMainWin.isOffline = true
            actions.openOfflineAct.trigger() //打开下线框
        }
        //好友消息接收
        onFriendMessage: {
            console.log("friend mesage receiving,number is ", number)
            try {
                time = time.split(";;") //0为时间 1为提示
                if (time.length < 2)
                    throw "message is lacked"
                var name
                for (var i in friendsModel) {
                    let m = friendsModel[i]
                    i = m.rowOf(number)
                    if (i != -1) {
                        name = m.data(i, 4) //备注
                        if (name != "") {
                            break
                        }
                        name = m.data(i, 1) //昵称
                        break
                    }
                }
                var existis = false
                var length=infoModel.count
                var himg=  "image://friends/" + number + "1notgray"
                for ( i = 0; i < length; ++i) {
                    //验证信息则 number标记为-1
                    if (infoModel.get(i).r_number == number+"1") {
                        existis = true
                        infoModel.set(i, {
                                          "r_imgPath": himg,
                                          "r_title"//彩色标记
                                          : name,
                                          "r_tip": time[1],
                                          "r_time": time[0],
                                          "r_number": number + "1"
                                      })
                        infoModel.move(i, 0, 1)
                    }
                }
                if(!existis)
                    infoModel.insert(0, {
                                         "r_imgPath": himg,
                                         "r_title"//彩色标记
                                         : name,
                                         "r_tip": time[1],
                                         "r_time": time[0],
                                         "r_number": number + "1"
                                     })
                var fw = mapChat.get(number + "1")
                if (fw == undefined){
                    //托盘闪烁
                    //头像闪烁
                    let himg=infoModel.get(0).r_imgPath
                    tray.iconSource = himg
                    tray.midSource = himg
                    tray.tooltip = "有新消息到来"
                    timer1s.restart()
                    tray.hasInfo = true //有信息标记
                    throw "friend win is close"
                }
                fw.addMessage(html, time[0])
            } catch (e) {
                if (typeof e == "string")
                    console.log("warning:", e)
                else console.log("receiveing message occured a error:", e.message)
            }
        }
        //自己更新等级及活跃度
        onUpdateMyGrade: {
            if (grade == -1) {
                console.log("my grade is not changed")
            } else {
                if (grade <= qqMainWin.grade) {
                    console.log("warning:now grade is changed to  smaller")
                } else {
                    qqMainWin.grade = grade
                }
            }
            qqMainWin.activeDays = ads
        }
    }
    //mainWin
    //托盘点击
    Connections {
        target: mainWin
        onTrayClicked: {
            console.log(" onTrayClicked")
            var items = infoView.contentItem.children
            if (items.length > 0) {
                var item = infoView.contentItem.children[0]
                item.clicked()
            }
        }
    }

    //images
    //历史头像添加时
    Connections {
        target: images
        //头像被更改时
        onHistoryImageAdded: {
            console.log("onHistoryImageAdded:url(", url, ") index(", index, ")")
            imgHead.source = "" //刷新
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
    //函数
    //qqmainwin释放时释放资源
    function realse() {
        console.log("qqMainWin realse resource")
        mainWin.hide() //先关闭上一级成为最后一个窗口
        //先释放server等长时操作
        funcc.realseServer()
        funcc.deleteNetAndUpdateTimer() //删除网络监测器
        funcc.emitCloseMyProcess() //关闭辅助进程
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
        //删除历史头像文件
        images.removeHistory()
        //删除 friends里头聊天记录文件
        funcc.removeDir("../user/" + myqq + "/friends") //移除所有聊天记录
        //释放更改头像资源
        if (loaderForAlterHImg.status == Loader.Ready) {
            loaderForAlterHImg.item.close()
        }
        //释放更改封面资源
        if (loaderForAlterInfo.status == Loader.Ready) {
            var lcover = loaderForAlterInfo.item.loaderForAlterCover
            if (lcover.status === Loader.Ready) {
                console.log("alter-cover interface could be closed")
                lcover.item.close() //关闭altercover窗口
            }
        }
        //释放wetherwin
        if (loaderForWeather.status == Loader.Ready) {
            console.log("realsed weather win")
            loaderForWeather.source = ""
        }
        //释放headInfwin
        if (loaderForHeadImg.status == Loader.Ready) {
            console.log("realsed head-image win")
            loaderForHeadImg.source = ""
        }
        //关于
        if (loaderForAbout.status == Loader.Ready) {
            loaderForAbout.item = ""
        }
        //好友组小标签
        if (loaderForFG.status == Loader.Ready) {
            loaderForFG.sourceComponent = undefined
        }
        //好友小标签
        if (loaderForFriend.status == Loader.Ready) {
            loaderForFriend.sourceComponent = undefined
        }
        //验证界面
        if (loaderForVerify.status == Loader.Ready) {
            loaderForVerify.source = ""
        }
        //释放资料列表
        //释放聊天界面
        try {
            mapChat.forEach(function (ele, i) {
                ele.close()
            })
            mapInfo.forEach(function (ele, i) {
                ele.close()
            })
        } catch (e) {
            console.log("warning:", e)
        }
        mapChat = null
        mapInfo = null
    }
    //移动位于index的好友from 分组 before to after
    function moveFriend(before, after, index) {
        //排除极端情况
        if (before == after || index === undefined || before === undefined
                || after === undefined)
            return false
        //不移动
        if (before < 0 || after < 0 || index < 0) {
            console.log("moveFriend(before,after,index) arguments including negative")
            return false
        }
        console.log("moved Friend with", index, " to group from ", before,
                    " to ", after, " position")
        try {
            var bm = friendsModel[before]
            var am = friendsModel[after]
            //这里不能从模型取出对象然后换模型 会随后一段时间发生数据对象自动析构
            var item = bm.getItem(index)
            var number = item.myqq()
            am.append(item.myqq(), item.name(), item.signature(),
                      item.imgPath(), item.tag(), item.grade(), item.status(),
                      item.infoSet(), item.statusSet())
            bm.remove(index) //删除
            //更新
            bm.update(before, 10000)
            am.sort() //重排序
            var items = viewFriend.contentItem.children
            //对于qwidget的嵌套模型 重赋值来刷新代理
            items[before].friend.model = null
            items[before].friend.model = friendsModel[before]
            items[after].friend.model = null
            items[after].friend.model = friendsModel[after]
            friendGroupModel.update(before, after) //更新外层代理项
            //更新远程
            funcc.moveFriend(before, after, number)
            //更新 组位置
            qqMainWin.fgIndex = after
        } catch (e) {
            console.log("error! " + e)
        }

        return number
    }
    //打开好友聊天界面 参数：号码
    function sentFriendInfo(number) {
        console.log("发送即时消息，号码：", number)
        var obj = {}
        obj.number = number + "1"
        try {
            var cw = mapChat.get(obj.number) //号码加1 表示好友
            if (cw == undefined) {
                console.log("chat win is undefined")
                cw = chatComp.createObject(null)
                if (!cw) {
                    console.log("created a object is of failure")
                    return
                }
                mapChat.set(obj.number, cw)
                funcc.getFIP(obj.number)
                let find = false
                let length = friendsModel.length
                for (var i = 0; i < length; ++i) {
                    let m = friendsModel[i]
                    let index = m.rowOf(number)
                    if (index != -1) {
                        find = true
                        // obj.number = number
                        obj.name = m.data(index, 1)
                        obj.headImg = m.data(index, 3)
                        obj.tag = m.data(index, 4)
                        obj.status = m.data(index, 6)
                        break
                    }
                }
                if (!find) {
                    throw "not find the number"
                }
                cw.obj = obj
                funcc.loadFChatLog(cw, number) //加载聊天记录
            }
        } catch (e) {
            if (typeof e == "string") {
                console.log("warning:", e)
            }
            console.log("warning:", e.message)
            return
        }
        actions.openChatWinAct.trigger(cw) //打开好友资料 传要创建的部件
    }
    //打开资料页面 参数：基本信息对象
    function openFriendDataWin(obj) {
        var number = obj.number
        if (number == myqq) {
            actions.openAlterUserInfoAct.trigger()
        } else {
            try {
                var map = qqMainWin.mapInfo
                var win = map.get(number + "1")
                if (win == undefined) {
                    win = friendInfoComp.createObject(null)
                    if (!win)
                        throw "created a object is null"
                    map.set(number + "1", win)
                    win.isFirst = true
                }
            } catch (e) {
                switch (typeof e) {
                case "string":
                    console.log("message:", e)
                    break
                default:
                    console.log("message:", e.message)
                }
                return
            }
            win.number = number
            win.name = obj.name
            win.signature = obj.signature
            win.headImg = obj.headImg
            win.tag = obj.tag
            win.fgIndex = obj.fgIndex
            win.last = obj.fgIndex
            actions.openFriendInfoAct.trigger(win) //打开好友资料 传要创建的部件
        }
    }
    //拖拽调整大小 回调函数
    function sizeChanged(w, h, delta, directX, directY) {
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
    //刷新好友图片状态 由于灰度图不能正常更新 所以换掉整个模型 即视图模型置零再赋值
    function flushFImage(i) {
        try {
            var view = viewFriend.contentItem.children[i].friend
            //刷新
            view.model = null
            view.model = friendsModel[i]
            view.forceLayout()
        } catch (e) {
            console.log(e)
        }
    }
    //检测一个对象是否为空 空true
    function objIsEmpty(obj) {
        if (obj == null || obj == undefined)
            return true
        for (let i in obj) {
            return false
        }
        return true
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
                            var obj = {}
                            obj.posY = qqMainWin.y + bodyRec.y + headRec.y + recHead.y //与头像对齐
                            obj.name = name
                            obj.tag = "" //无就更新tag为空
                            obj.grade = grade
                            obj.signature = signature
                            loaderForHeadImg.obj = obj
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
                            tipTimer500msForGrd.restart()
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
                property string days: "0"
                id: gradeTip
                x: gradeLab.x + gradeLab.posx
                y: gradeLab.y + gradeLab.posy + 20
                delay: 1000
                visible: false

                background: Label {
                    id: gradeBack
                    font.pointSize: 10
                    color: "gray"
                    text: "我的MQ等级:\n 等级:" + grade + "级\n 剩余升级天数:" + gradeTip.days + "天"
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
                //好友组
                ScrollView {
                    //起点
                    property int ix: qqMainWin.x + bodyRec.x + container.x
                    property int iy: qqMainWin.y + bodyRec.y + container.y + y
                    signal closeAllGroup
                    //关闭好友组按钮选中状态
                    signal closeAllFGChecked
                    signal closeFGChecked(int index)
                    //点击好友组便条
                    signal clickFG(int index, var mouse)
                    signal checkedFG(int index)
                    signal closeAllHovered
                    signal openAllHovered

                    //好友部分信号
                    signal closeAllFChecked
                    signal clickF(int fgIndex, int fIndex, var mouse)
                    signal doubleClickF(int fgIndex, int fIndex, var mouse)
                    signal checkedF(int fgIndex, int fIndex)
                    signal closeFChecked(int fgIndex, int fIndex)
                    id: scrFriend
                    visible: true
                    y: 40
                    padding: 0
                    width: headerRec.width
                    height: container.height - 40
                    hoverEnabled: true
                    contentWidth: headerRec.width
                    contentHeight: viewFriend.contentHeight
                    clip: true
                    onCloseAllGroup: {
                        var length = friendGroupModel.count()
                        var items = viewFriend.contentItem.children
                        for (var i = 0; i < length; ++i) {
                            var item = items[i]
                            item.listGroup.rotat = 0
                            friendGroupModel.setData(i, false, 4)

                            item.isChecked = false
                        }
                    }
                    onCloseFGChecked: {
                        var length = friendGroupModel.count()
                        var items = viewFriend.contentItem.children

                        var item = items[index]
                        item.isChecked = false
                    }
                    onCloseAllFGChecked: {
                        var length = friendGroupModel.count()
                        var items = viewFriend.contentItem.children
                        for (var i = 0; i < length; ++i) {
                            var item = items[i]
                            item.isChecked = false
                        }
                    }
                    onOpenAllHovered: {
                        var length = friendGroupModel.count()
                        var items = viewFriend.contentItem.children
                        for (var i = 0; i < length; ++i) {
                            var item = items[i]
                            item.he = true
                        }
                    }
                    onVisibleChanged: {
                        if (loaderForFG.status == Loader.Ready) {
                            loaderForFG.item.close()
                        }
                    }
                    onClickFG: {
                        var item = viewFriend.contentItem.children[index]
                        item.lgMouse.clicked(mouse)
                    }
                    onCheckedFG: {
                        var item = viewFriend.contentItem.children[index]
                        item.isChecked = true
                        fgScroll.visible = true
                    }
                    onCloseAllHovered: {
                        var length = friendGroupModel.count()
                        var items = viewFriend.contentItem.children
                        for (var i = 0; i < length; ++i) {
                            var item = items[i]
                            item.he = false
                        }
                    }

                    //滚动栏
                    ScrollBar.vertical: ScrollBar {
                        property bool isInint: true
                        id: fgScroll
                        padding: 0
                        parent: scrFriend //必须设置父对象才能启动hovered and pressed以及鼠标事件
                        anchors.right: parent.right //绑定右边
                        width: 6
                        height: scrFriend.height //必须指定高度滑块才能自由设置
                        orientation: Qt.Vertical
                        policy: (scrFriend.contentHeight > scrFriend.height) ? (hovered ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff) : ScrollBar.AlwaysOff
                        size: scrFriend.height / scrFriend.contentHeight

                        hoverEnabled: true
                        active: hovered || pressed
                        z: 1
                        onHoveredChanged: {
                            if (!hovered) {
                                var length = friendGroupModel.count()
                                var items = viewFriend.contentItem.children
                                for (var i = 0; i < length; ++i) {
                                    var item = items[i]
                                    item.he = true
                                }
                            }
                        }
                        onPositionChanged: {
                            if (isInint)
                                return
                            timer50msForFG.restart()
                            if (loaderForFG.status == loaderForFG.Ready)
                                loaderForFG.item.hide()
                        }
                        contentItem: Rectangle {
                            implicitWidth: 6
                            implicitHeight: parent.size * scrFriend.height
                            radius: width / 2
                            color: parent.hovered ? Qt.darker("#d7d7d7",
                                                              1.3) : "#d7d7d7"
                        }
                        Component.onCompleted: {
                            isInint = false
                        }
                    }
                    contentItem: ListView {
                        id: viewFriend
                        model: friendGroupModel
                        clip: true

                        bottomMargin: 0
                        topMargin: 0
                        delegate: Rectangle {
                            property alias delegateModel: listFriend.model
                            property alias friend: listFriend
                            property alias listGroup: listGroup
                            property alias muter: listGroup.muter //新项需要
                            property alias lgMouse: lgMouse
                            property alias isChecked: listGroup.isChecked //新添加项需要直接定义的属性
                            property alias he: listGroup.hoverEnabled
                            property alias edit: listGroup.edit
                            property string group: r_group
                            property bool isClicked: false
                            property int pos: -1
                            id: fgitem
                            width: headerRec.width
                            height: listGroup.height

                            //组
                            List {
                                id: listGroup
                                w: headerRec.width
                                h: 35
                                ratio.visible: true
                                name.text: group
                                sumForFriends: r_count
                                activeLine: r_online
                                onClicked: {
                                    isChecked = false
                                    var index = viewFriend.indexAt(fgitem.x,
                                                                   fgitem.y)
                                    if (index == -1) {
                                        console.log("index can't is negative")
                                        return
                                    }
                                    if (rotat == 0) {
                                        rotat = 90
                                        friendGroupModel.setData(index, true, 4)
                                        if (loaderForFG.status != Loader.Null) {
                                            loaderForFG.item.btn.rotat = 90
                                            loaderForFG.item.btn.backColor = fgitem.color
                                        }
                                    } else {
                                        rotat = 0
                                        friendGroupModel.setData(index, false,
                                                                 4)
                                        if (loaderForFG.status != Loader.Null) {
                                            loaderForFG.item.btn.rotat = 0
                                            loaderForFG.item.btn.backColor = fgitem.color
                                        }
                                    }
                                }
                                onEditFinished: {
                                    var index = viewFriend.indexAt(fgitem.x,
                                                                   fgitem.y)
                                    if (index === -1) {
                                        console.log("index is -1,item index acquired to update remote friend group unsuccessfully")
                                        return
                                    }
                                    friendGroupModel.setData(index, t) //更新本地组名
                                    scrFriend.openAllHovered()
                                    //推送远程
                                    var obj = {}
                                    //重命名
                                    if (fgMenu.addIndex == -1) {
                                        console.log("远程重命名分组")
                                        obj.content = "dmrFGroup"
                                        obj.type = "rename"
                                        obj.index = index
                                        obj.newName = t
                                        funcc.updateFGroup(obj)
                                        //添加分组
                                    } else {
                                        console.log("远程添加分组")
                                        obj.index = index
                                        obj.name = t
                                        funcc.addRemoteFriendGroup(obj)
                                        fgMenu.addIndex = -1 //重置
                                    }
                                }

                                MouseArea {
                                    id: lgMouse
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                                    hoverEnabled: parent.hoverEnabled
                                    onClicked: {
                                        if (mouse.button == Qt.LeftButton)
                                            parent.clicked()
                                        else {
                                            var index = viewFriend.indexAt(
                                                        fgitem.x, fgitem.y)
                                            if (index == -1) {
                                                console.log("get -1 opened friend group munu ")
                                                return
                                            }
                                            fgMenu.x = mouseX + 5
                                            fgMenu.y = mouseY + 5 + fgitem.y - fgScroll.position
                                                    * scrFriend.contentHeight
                                            //不可动组
                                            if (index == 0)
                                                return
                                            if (fgMenu.y > scrFriend.height / 2 + 15) {
                                                fgMenu.y = fgMenu.y - fgMenu.height - 10
                                            }
                                            console.log("fgmenu index=", index)
                                            fgMenu.indexOfPressed = index
                                            fgMenu.open()
                                        }
                                    }
                                    onHoveredChanged: {
                                        if (containsMouse) {
                                            //编辑时不要弹出
                                            if (fgitem.edit.visible) {
                                                fgitem.listGroup.isChecked = true
                                                return
                                            }
                                            var obj = loaderForFG.item
                                            var items = viewFriend.contentItem.children
                                            var length = friendGroupModel.count(
                                                        )
                                            for (var i = 0; i < length; ++i) {
                                                var item = items[i]
                                                if (item.group == "我的好友") {

                                                    obj.by = item.y + scrFriend.iy
                                                            + listGroup.height - fgScroll.position
                                                            * scrFriend.contentHeight
                                                    if (obj.by < scrFriend.iy)
                                                        obj.by = scrFriend.iy
                                                } else if (item.group == "黑名单") {
                                                    obj.ey = item.y + scrFriend.iy
                                                            - fgScroll.position
                                                            * scrFriend.contentHeight
                                                    if (obj.ey > scrFriend.iy + scrFriend.height)
                                                        obj.ey = scrFriend.iy + scrFriend.height
                                                }
                                            }
                                            var index = viewFriend.indexAt(
                                                        fgitem.x, fgitem.y)
                                            if (index == -1) {
                                                console.log("index is -1")
                                                return
                                            }
                                            scrFriend.closeAllFGChecked()
                                            parent.isChecked = true
                                            fgScroll.visible = true //显示滚动栏？
                                            if (index > 1 && index < length - 1)
                                                obj.canDarg = true
                                            else
                                                obj.canDarg = false
                                            obj.hide()
                                            var tx = scrFriend.ix + fgitem.x
                                            var ty = (scrFriend.iy + fgitem.y) - fgScroll.position
                                                    * (scrFriend.contentHeight) //定位绝对位置
                                            if (ty > scrFriend.iy + scrFriend.height - 35) {
                                                var temp = ty - (scrFriend.iy
                                                                 + scrFriend.height - 35)
                                                fgScroll.setPosition(
                                                            fgScroll.position + temp
                                                            / scrFriend.contentHeight)
                                                obj.y = (scrFriend.iy + fgitem.y)
                                                        - fgScroll.position
                                                        * (scrFriend.contentHeight) //定位绝对位置
                                            } else if (ty < scrFriend.iy) {
                                                fgScroll.setPosition(
                                                            fgScroll.position - (scrFriend.iy - ty)
                                                            / scrFriend.contentHeight)
                                                obj.y = (scrFriend.iy + fgitem.y)
                                                        - fgScroll.position
                                                        * (scrFriend.contentHeight) //定位绝对位置
                                            } else
                                                obj.y = ty
                                            obj.x = tx
                                            obj.btn.w = listGroup.w
                                            obj.btn.h = listGroup.h
                                            obj.name = listGroup.name.text
                                            obj.btn.rotat = listGroup.rotat
                                            obj.btn.activeLine = listGroup.activeLine
                                            obj.btn.sumForFriends = listGroup.sumForFriends
                                            obj.btn.backColor = listGroup.backColor
                                            obj.btn.set = listGroup.set
                                            obj.index = index
                                            obj.isDarg = false
                                            obj.opacity = 0.8
                                            obj.show()
                                            obj.raise()
                                        }
                                    }
                                }
                            }
                            //好友表
                            ListView {
                                id: listFriend
                                anchors.top: listGroup.bottom
                                visible: r_visible //可视化随模型数据变化
                                onVisibleChanged: {
                                    console.log("listFriend visible= ", visible)
                                    if (visible)
                                        parent.height += height
                                    else
                                        parent.height -= height
                                }
                                onCountChanged: {
                                    console.log("listFriend count=", count)
                                    height = count * 60
                                    //当更新时 强制矫正大小
                                    if (visible)
                                        parent.height = height + listGroup.height
                                    else
                                        parent.height = listGroup.height
                                }

                                delegate: Friend {
                                    property alias fmouse: fmouse
                                    property string img: r_imgPath //用于刷新图片，保持角色一致绑定
                                    id: fitem
                                    w: headerRec.width
                                    h: 60
                                    hoverEnabled: true
                                    headImg: img
                                    name: r_name
                                    label: r_tag
                                    status: r_status
                                    bottomText: r_signature
                                    //好友表信号
                                    Connections {
                                        target: scrFriend
                                        //关闭所有的选择
                                        onCloseAllFChecked: {
                                            fitem.isChecked = false
                                        }
                                        onClickF: {
                                            if (mouse.button == Qt.RightButton) {
                                                var i1 = viewFriend.indexAt(
                                                            fgitem.x, fgitem.y)
                                                var i2 = listFriend.indexAt(
                                                            fitem.x, fitem.y)
                                                if (i1 == fgIndex
                                                        && i2 == fIndex) {
                                                    fmouse.clicked(mouse)
                                                }
                                            }
                                        }
                                        onDoubleClickF: {
                                            var i1 = viewFriend.indexAt(
                                                        fgitem.x, fgitem.y)
                                            var i2 = listFriend.indexAt(
                                                        fitem.x, fitem.y)
                                            if (i1 == fgIndex && i2 == fIndex) {
                                                fmouse.doubleClicked(mouse)
                                            }
                                        }
                                        onCheckedF: {
                                            var i1 = viewFriend.indexAt(
                                                        fgitem.x, fgitem.y)
                                            var i2 = listFriend.indexAt(
                                                        fitem.x, fitem.y)
                                            if (i1 == fgIndex && i2 == fIndex) {
                                                fitem.isChecked = true
                                                fgScroll.visible = true
                                            }
                                        }
                                        onCloseFChecked: {
                                            var i1 = viewFriend.indexAt(
                                                        fgitem.x, fgitem.y)
                                            var i2 = listFriend.indexAt(
                                                        fitem.x, fitem.y)
                                            if (i1 == fgIndex && i2 == fIndex) {
                                                fitem.isChecked = false
                                            }
                                        }
                                        //控制hover激活与否
                                        onCloseAllHovered: {
                                            fitem.hoverEnabled = false
                                        }
                                        onOpenAllHovered: {
                                            fitem.hoverEnabled = true
                                        }
                                    }

                                    MouseArea {
                                        id: fmouse
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                                        onDoubleClicked: {
                                            if (mouse.button == Qt.LeftButton) {
                                                try {
                                                    sentFriendInfo(
                                                                r_myqq) //打开聊天界面
                                                } catch (e) {
                                                    console.log("warning:",
                                                                e.message)
                                                }
                                            }
                                        }

                                        onClicked: {
                                            if (mouse.button == Qt.RightButton) {
                                                var fgindex = viewFriend.indexAt(
                                                            fgitem.x, fgitem.y)
                                                if (fgindex == -1) {
                                                    console.log("get -1 opened friend menu ")
                                                    return
                                                }
                                                var findex = listFriend.indexAt(
                                                            fitem.x, fitem.y)
                                                if (findex == -1) {
                                                    console.log("get -1 opened friend menu ")
                                                    return
                                                }

                                                fMenu.fgIndex = fgindex
                                                fMenu.fIndex = findex
                                                //过滤自己， 自己不可删除
                                                if (r_myqq == qqMainWin.myqq) {
                                                    dfmenu.visible = false
                                                } else {
                                                    dfmenu.visible = true
                                                }

                                                fMenu.popup()
                                            }
                                        }
                                        onHoveredChanged: {
                                            if (containsMouse) {
                                                var obj = loaderForFriend.item
                                                var items = viewFriend.contentItem.children
                                                var length = friendGroupModel.count()
                                                var item = items[1] //wode好友组
                                                obj.by = item.y + scrFriend.iy - fgScroll.position
                                                        * scrFriend.contentHeight
                                                if (obj.by < scrFriend.iy)
                                                    obj.by = scrFriend.iy

                                                item = items[length - 1] //黑名单组
                                                obj.ey = item.y + scrFriend.iy - fgScroll.position
                                                        * scrFriend.contentHeight
                                                if (obj.ey > scrFriend.iy + scrFriend.height)
                                                    obj.ey = scrFriend.iy + scrFriend.height

                                                var fgindex = viewFriend.indexAt(
                                                            fgitem.x, fgitem.y)
                                                if (fgindex == -1) {
                                                    console.log("fgIndex is -1")
                                                    return
                                                }
                                                var fIndex = listFriend.indexAt(
                                                            fitem.x, fitem.y)
                                                if (fIndex == -1) {
                                                    console.log("fIndex is -1")
                                                    return
                                                }
                                                //关闭所有的标签
                                                scrFriend.closeAllFChecked()
                                                parent.isChecked = true
                                                fgScroll.visible = true //显示滚动栏？
                                                obj.hide()
                                                var tx = scrFriend.ix + fgitem.x
                                                var ty = (scrFriend.iy + fgitem.y + listGroup.height) - fgScroll.position * (scrFriend.contentHeight) + fitem.y //定位绝对位置
                                                if (ty > scrFriend.iy + scrFriend.height - 60) {
                                                    var temp = ty - (scrFriend.iy
                                                                     + scrFriend.height - 60)
                                                    fgScroll.setPosition(
                                                                fgScroll.position + temp / scrFriend.contentHeight) + listGroup.height
                                                    obj.y = (scrFriend.iy + fgitem.y) + fitem.y
                                                            - fgScroll.position
                                                            * (scrFriend.contentHeight) //定位绝对位置
                                                } else if (ty < scrFriend.iy) {
                                                    fgScroll.setPosition(
                                                                fgScroll.position - (scrFriend.iy - ty) / scrFriend.contentHeight)
                                                    obj.y = (scrFriend.iy + fgitem.y) + fitem.y
                                                            + listGroup.height - fgScroll.position
                                                            * (scrFriend.contentHeight) //定位绝对位置
                                                } else
                                                    obj.y = ty
                                                obj.x = tx
                                                obj.btn.w = fitem.w
                                                obj.btn.h = fitem.h
                                                obj.btn.backColor = fitem.backColor
                                                obj.img = fitem.headImg
                                                obj.name = fitem.name
                                                obj.label = fitem.label
                                                obj.sig = fitem.bottomText
                                                obj.grade = r_grade
                                                obj.isDarg = false
                                                obj.fIndex = fIndex
                                                obj.fgIndex = fgindex
                                                obj.show()
                                                obj.raise()
                                            }
                                        }
                                    }
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
                    ScrollBar.vertical.policy: (contentHeight
                                                > height) ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
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
                width: headerRec.width
                height: container.height
                contentWidth: headerRec.width
                contentHeight: infoTip.visible ? infoTip.height : infoView.height
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                //滚动栏
                ScrollBar.vertical: ScrollBar {
                    parent: scrInformation //必须设置父对象才能启动hovered and pressed以及鼠标事件

                    anchors.right: parent.right //绑定右边
                    orientation: Qt.Vertical
                    width: 6
                    height: scrInformation.height
                    policy: (scrInformation.contentHeight > scrInformation.height) ? (hovered ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff) : ScrollBar.AlwaysOff
                    size: scrInformation.height / scrInformation.contentHeight
                    hoverEnabled: true
                    active: hovered || pressed
                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: parent.size * scrInformation.height
                        radius: width / 2
                        color: parent.hovered ? Qt.darker("#d7d7d7",
                                                          1.3) : "#d7d7d7"
                    }
                }
                Label {
                    id: infoTip
                    visible: true
                    x: scrInformation.width / 2 - width / 2
                    y: 60
                    font.pointSize: 9
                    font.family: song.name
                    text: qsTr("暂时没有会话")
                    color: "gray"
                }
                ListView {
                    id: infoView
                    visible: !infoTip.visible
                    width: parent.width
                    model: ListModel {
                        id: infoModel
                        onCountChanged: {
                            if (count === 0) {
                                infoTip.visible = true
                            } else {
                                infoTip.visible = false
                            }
                        }
                        //函数
                        //对infomodel按r_time 大到小排序
                        function sort() {
                            let length = infoModel.count
                            for (var i = 1; i <= length - 1; ++i) {
                                if (infoModel.get(i).r_time > infoModel.get(
                                            i - 1).r_time) {
                                    infoModel.move(i, i - 1, 1)
                                }
                            }
                        }
                    }
                    delegate: Friend {
                        id: infoItem
                        w: headerRec.width
                        h: 60
                        headImg: r_imgPath
                        name: r_title
                        bottomText: r_tip
                        onClicked: {
                            clickedInfo(r_number)
                        }
                        topLab.width: w - timeItem.width - 3
                        bottomLab.width: w - 10
                        Rectangle {
                            id: timeItem
                            height: 30
                            x: parent.width - width - 3
                            y: 3
                            width: 85
                            clip: true

                            color: "transparent"
                            Text {
                                property int length: r_time.length
                                anchors.centerIn: parent
                                padding: 0
                                text: r_time.substring(
                                          0,
                                          length - 5) + "\n    " + r_time.substring(
                                          length - 5, length)
                                font.pointSize: 10
                                color: "gray"
                            }
                        }
                        Component.onCompleted: {
                            parent.parent.height = parent.parent.count * 60
                        }
                    }
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
                        menuItemTip.item.visible = true
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
                    qqMainWin.close() //呼叫关闭槽释放资源
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
        property var obj: null
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
        focus: true
        visible: Loader.Ready === loaderForAlterInfo.status
    }
    //打开验证页面
    Loader {
        id: loaderForVerify
        focus: true
        visible: Loader.Ready === loaderForVerify.status
    }
    //弹出下线框
    Loader {
        property var obj: null
        id: loaderForOffline
        focus: true
        visible: Loader.Ready === loaderForOffline.status
    }
    //修改好友备注
    Loader {
        property var obj: null
        id: loaderForAlterTag
        focus: true
        visible: Loader.Ready === loaderForAlterTag.status
    }
    //好友资料
    Component {
        id: friendInfoComp
        FriendWin {}
    }
    //聊天界面 这里使用单独窗口作为各个聊天界面 不模仿qq的统合界面
    Component {
        id: chatComp
        ChatWin {}
    }

    Loader {
        id: loaderForChat
        focus: true
        visible: Loader.Ready === loaderForChat.status
    }

    //移动好友组
    Loader {
        id: loaderForFG
        sourceComponent: fgComp
        visible: Loader.Ready === loaderForFG.status
    }
    Component {
        id: fgComp
        Window {
            property alias btn: btn
            property int index: -1
            property bool firstAllClose: false
            property bool isDarg: false
            property bool canDarg: false
            property int bx: scrFriend.ix
            property int ex: scrFriend.ix + scrFriend.width
            property int by: 0
            property int ey: 0
            property string name: ""

            id: fgwin
            width: btn.width
            height: btn.height
            flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
            opacity: 0.8
            onClosing: {
                hide()
            }
            onVisibleChanged: {
                if (visible) {
                    var item = loaderForFriend.item
                    scrFriend.closeFChecked(item.fgIndex, item.fIndex)
                    item.close()
                }
            }
            onXChanged: {
                if (x == m.tx && m.l) {
                    close()
                    m.l = false
                }
            }

            onYChanged: {
                if (y == m.ty && m.l) {
                    m.l = false
                    close()
                }
            }

            List {
                id: btn
                name.text: fgwin.name
                MouseArea {
                    property bool l: false
                    property bool isLeft: false
                    property int tx: 0
                    property int ty: 0
                    //开始的窗口位置
                    property int swx: 0
                    property int swy: 0
                    //按下位置
                    property point s: Qt.point(fgwin.x + startPoint.x,
                                               fgwin.y + startPoint.y)
                    property point startPoint: Qt.point(0, 0)
                    id: m

                    width: parent.width
                    height: parent.height
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    //防误移动
                    onSChanged: {
                        if (fgwin.firstAllClose) {
                            var offx = Math.abs(fgwin.x - swx)
                            var offy = Math.abs(fgwin.y - swy)
                            console.log(fgwin.y, swy)
                            if (offx * offx + offy * offy > 4) {
                                fgwin.firstAllClose = false
                                fgwin.opacity = 0.5
                                scrFriend.closeAllGroup() //关闭所有号好友表
                                var item = viewFriend.contentItem.children[friendGroupModel.count(
                                                                               ) - 1]
                                var ty = item.y + scrFriend.iy - fgScroll.position
                                        * scrFriend.contentHeight
                                var temp = scrFriend.iy + scrFriend.height
                                if (ty > temp)
                                    ty = temp
                                fgwin.ey = ty
                            }
                        }
                    }
                    onPressed: {
                        //弹窗关闭
                        if (fgMenu.visible)
                            fgMenu.close() //关闭菜单
                        if (menuPop.visible)
                            menuPop.close()
                        if (fMenu.visible)
                            fMenu.close()
                        if (mouse.button == Qt.LeftButton) {
                            isLeft = true
                            l = true
                            startPoint.x = mouseX
                            startPoint.y = mouseY
                            swx = fgwin.x
                            swy = fgwin.y
                            fgwin.firstAllClose = true
                            console.log(swy, fgwin.y)
                        } else
                            isLeft = false
                    }

                    onReleased: {
                        if (mouse.button == Qt.LeftButton) {
                            var ix = s.x - scrFriend.ix
                            var iy = s.y - scrFriend.iy + fgScroll.position
                                    * viewFriend.contentHeight
                            var index = viewFriend.indexAt(ix, iy)
                            if (index == -1) {
                                console.log("warning:index=-1")
                                cursorShape = Qt.ArrowCursor
                                return
                            }
                            var offx = Math.abs(swx - fgwin.x)
                            var offy = Math.abs(swy - fgwin.y)
                            var b = (!(Math.sqrt(
                                           offx * offx + offy * offy) < 3))
                            //忽略小弧度位移
                            if (fgwin.isDarg && b) {
                                //处理移动事件
                                if (cursorShape != Qt.ForbiddenCursor) {
                                    if (index !== fgwin.index
                                            && index != friendGroupModel.count(
                                                ) - 1 && index > 1) {
                                        var _1 = friendsModel[index]
                                        friendsModel[index] = friendsModel[fgwin.index]
                                        friendsModel[fgwin.index] = _1
                                        var items = viewFriend.contentItem.children
                                        items[index].friend.model = friendsModel[index]
                                        items[fgwin.index].friend.model = friendsModel[fgwin.index]

                                        friendsModel[index].update(
                                                    0,
                                                    friendsModel[index].rowCount(
                                                        ) - 1)
                                        friendsModel[fgwin.index].update(
                                                    0,
                                                    friendsModel[fgwin.index].rowCount(
                                                        ) - 1)
                                        friendGroupModel.swap(fgwin.index,
                                                              index)
                                        console.log("swap(", fgwin.index,
                                                    index, ")")
                                        //推送远程
                                        var obj = {}
                                        obj.content = "dmrFGroup"
                                        obj.type = "move"
                                        obj.index1 = fgwin.index
                                        obj.index2 = index
                                        funcc.updateFGroup(obj)
                                    }
                                }
                                cursorShape = Qt.ArrowCursor
                                fgwin.close()
                                return
                            }
                            cursorShape = Qt.ArrowCursor
                        }
                        fgwin.x = swx
                        fgwin.y = swy
                        fgwin.firstAllClose = false
                        btn.w = scrFriend.width - fgScroll.width
                        fgwin.opacity = 0.8
                        //不能移动则不打开
                        if ((!fgwin.canDarg) && b)
                            return
                        if (mouse.button == Qt.RightButton)
                            close()
                        scrFriend.clickFG(fgwin.index, mouse)
                    }
                    onContainsMouseChanged: {
                        if (!containsMouse) {
                            l = false
                            if (!pressed) {
                                scrFriend.closeFGChecked(fgwin.index)
                                fgScroll.visible = false
                                console.log("fgscroll close")
                                close()
                            }
                        } else {
                            scrFriend.checkedFG(fgwin.index)
                        }
                    }
                    //公式：窗口位置+=鼠标位置-起始位置；实现移动效果
                    onPositionChanged: {
                        if (pressed) {
                            if (!fgwin.canDarg || !isLeft)
                                return
                            btn.rotat = 0
                            fgwin.isDarg = true
                            tx = mouseX - startPoint.x
                            ty = mouseY - startPoint.y
                            fgwin.x += tx
                            fgwin.y += ty
                            if (s.x < bx || s.x > ex || (s.y < by
                                                         || s.y > ey)) {
                                cursorShape = Qt.ForbiddenCursor
                            } else
                                cursorShape = Qt.ArrowCursor
                        } else {
                            if (fgScroll.visible) {
                                if (mouseX > fgwin.width - fgScroll.width) {
                                    scrFriend.closeAllHovered()
                                    close()
                                    fgScroll.forceActiveFocus()
                                }
                            }
                        }
                    }
                    //滚动
                    onWheel: {
                        scrFriend.closeAllHovered()
                        close()
                        fgScroll.visible = true
                    }
                }
            }
        }
    }
    //移动好友标签
    Loader {
        id: loaderForFriend
        sourceComponent: friendComp
        visible: Loader.Ready === loaderForFriend.status
    }
    Component {
        id: friendComp
        Window {
            property alias btn: btn
            property int fgIndex: -1
            property int fIndex: -1
            property bool firstAllClose: false
            property bool isDarg: false
            property int bx: scrFriend.ix
            property int ex: scrFriend.ix + scrFriend.width
            property int by: 0
            property int ey: 0
            property string img: ""
            property string name: ""
            property string label: ""
            property string sig: ""
            property int grade: -1
            id: fwin
            width: btn.width
            height: btn.height
            flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
            opacity: 1.0
            onClosing: {
                hide()
            }
            onVisibleChanged: {
                if (visible) {
                    var item = loaderForFG.item
                    scrFriend.closeFGChecked(item.index)
                    item.close()
                }
            }
            onXChanged: {
                if (x == m.tx && m.l) {
                    close()
                    m.l = false
                }
            }

            onYChanged: {
                if (y == m.ty && m.l) {
                    m.l = false
                    close()
                }
            }
            Friend {
                id: btn
                headImg: fwin.img
                name: fwin.name
                label: fwin.label
                bottomText: fwin.sig
                MouseArea {
                    property bool l: false
                    property bool isLeft: false
                    property int tx: 0
                    property int ty: 0
                    //开始的窗口位置
                    property int swx: 0
                    property int swy: 0
                    //按下位置
                    property point s: Qt.point(fwin.x + startPoint.x,
                                               fwin.y + startPoint.y)
                    property point startPoint: Qt.point(0, 0)
                    id: m

                    width: parent.width
                    height: parent.height
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    //防误移动
                    onSChanged: {
                        if (fwin.firstAllClose) {
                            var offx = Math.abs(fwin.x - swx)
                            var offy = Math.abs(fwin.y - swy)
                            if (offx * offx + offy * offy > 4) {
                                fwin.firstAllClose = false
                                fwin.opacity = 0.5
                                fwin.label = ""
                                fwin.sig = ""
                                scrFriend.closeAllGroup() //关闭所有号好友表
                                //更新移动坐标范围 这里缺陷 会有延迟
                                var length = friendGroupModel.count()
                                var items = viewFriend.contentItem.children
                                var item = items[1] //wode好友组
                                var by = item.y + scrFriend.iy - fgScroll.position
                                        * scrFriend.contentHeight
                                if (by < scrFriend.iy)
                                    by = scrFriend.iy
                                item = items[length - 1] //黑名单组
                                var ey = item.y + scrFriend.iy - fgScroll.position
                                        * scrFriend.contentHeight
                                if (ey > scrFriend.iy + scrFriend.height)
                                    ey = scrFriend.iy + scrFriend.height
                                fwin.by = by
                                fwin.ey = ey
                            }
                        }
                    }
                    onPressed: {
                        //弹窗关闭
                        if (fgMenu.visible)
                            fgMenu.close() //关闭菜单
                        if (menuPop.visible)
                            menuPop.close()
                        if (fMenu.visible)
                            fMenu.close()
                        if (mouse.button == Qt.LeftButton) {
                            isLeft = true
                            l = true
                            startPoint.x = mouseX
                            startPoint.y = mouseY
                            swx = fwin.x
                            swy = fwin.y
                            fwin.firstAllClose = true
                        } else
                            isLeft = false
                    }
                    onDoubleClicked: {
                        fwin.firstAllClose = false //重置下状态
                        scrFriend.doubleClickF(fwin.fgIndex, fwin.fIndex, mouse)
                    }
                    onReleased: {
                        if (mouse.button == Qt.LeftButton) {
                            var ix = s.x - scrFriend.ix
                            var iy = s.y - scrFriend.iy + fgScroll.position
                                    * viewFriend.contentHeight //相对于viewfriend 内容项起点位置
                            var index = viewFriend.indexAt(ix, iy)
                            if (index == -1) {
                                console.log("warning:index=-1")
                                cursorShape = Qt.ArrowCursor
                                return
                            }
                            console.log("moveing friend to group with index ",
                                        index)
                            var offx = Math.abs(swx - fwin.x)
                            var offy = Math.abs(swy - fwin.y)
                            var b = (!(Math.sqrt(
                                           offx * offx + offy * offy) < 3))
                            //忽略小弧度位移
                            if (fwin.isDarg && b) {

                                //处理移动事件
                                if (cursorShape != Qt.ForbiddenCursor) {
                                    if (index !== fwin.index && index > 0) {
                                        let b = moveFriend(fwin.fgIndex, index,
                                                           fwin.fIndex)
                                        //更新资料分组
                                        if (typeof b == "string") {
                                            try {
                                                let item = qqMainWin.mapInfo.get(
                                                        b)
                                                if (item != undefined) {
                                                    item.fgIndex = index //更新资料索引
                                                    item.last = index
                                                }
                                            } catch (e) {
                                                console.log("warning:",
                                                            e.message)
                                            }
                                        }
                                    }
                                }
                                cursorShape = Qt.ArrowCursor
                                fwin.close()
                                return
                            }
                            cursorShape = Qt.ArrowCursor
                        }
                        fwin.x = swx
                        fwin.y = swy
                        fwin.firstAllClose = false
                        fwin.opacity = 1.0
                        btn.w = scrFriend.width - fgScroll.width
                        if (mouse.button == Qt.RightButton) {
                            scrFriend.clickF(fwin.fgIndex, fwin.fIndex, mouse)
                            close()
                        }
                    }
                    onContainsMouseChanged: {
                        if (!containsMouse) {
                            l = false
                            if (!pressed) {
                                scrFriend.closeFChecked(fwin.fgIndex,
                                                        fwin.fIndex)
                                fgScroll.visible = false
                                console.log("fgscroll close")
                                close()
                            }
                        } else {
                            scrFriend.checkedF(fwin.fgIndex, fwin.fIndex)
                        }
                    }
                    //是否弹出头像窗口
                    onHoveredChanged: {
                        if (containsMouse) {
                            var obj = {}
                            obj.posY = fwin.y //与标签对齐
                            obj.name = fwin.name
                            obj.tag = fwin.label
                            obj.grade = fwin.grade
                            obj.signature = fwin.sig
                            loaderForHeadImg.obj = obj
                            actions.headImgAct.trigger() //调用相应动作
                        } else {
                            loaderForHeadImg.item.headImgHovered = false //标记为不悬浮
                        }
                    }

                    //公式：窗口位置+=鼠标位置-起始位置；实现移动效果
                    onPositionChanged: {
                        if (pressed) {
                            if (!isLeft)
                                return
                            fwin.isDarg = true
                            tx = mouseX - startPoint.x
                            ty = mouseY - startPoint.y
                            fwin.x += tx
                            fwin.y += ty
                            if (s.x < bx || s.x > ex || (s.y < by
                                                         || s.y > ey)) {
                                cursorShape = Qt.ForbiddenCursor
                            } else
                                cursorShape = Qt.ArrowCursor
                        } else {
                            if (fgScroll.visible) {
                                if (mouseX > fwin.width - fgScroll.width) {
                                    scrFriend.closeAllHovered()
                                    close()
                                    fgScroll.forceActiveFocus()
                                }
                            }
                        }
                    }
                    //滚动
                    onWheel: {
                        scrFriend.closeAllHovered()
                        close()
                        fgScroll.visible = true
                    }
                }
            }
        }
    }
    //菜单
    //好友分组菜单
    Menu {
        property int indexOfPressed: -1
        property int addIndex: -1 //标记添加索引
        id: fgMenu
        width: 154
        height: 210
        visible: false
        topPadding: 9
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        parent: scrFriend //视觉对象 开启相对对象坐标定位
        modal: true

        Overlay.modal: Rectangle {
            color: "#00000000"
        }
        //弹出时初始化添加索引
        onVisibleChanged: {
            if (visible) {
                fMenu.visible = false
                addIndex = -1
                if (loaderForFG.status == Loader.Ready) {
                    loaderForFG.item.visible = false
                }
                if (loaderForFriend.status == Loader.Ready) {
                    loaderForFriend.item.visible = false
                }
            }
        }
        //控制是否可以删除和重命名
        onIndexOfPressedChanged: {
            if (indexOfPressed < 2
                    || indexOfPressed == friendGroupModel.count() - 1) {
                rmenu.enabled = false
                dmenu.enabled = false
            } else {
                rmenu.enabled = true
                dmenu.enabled = true
            }
        }

        //刷新好友联系人
        MenuItem {
            x: 1
            width: 162
            height: 30
            text: qsTr("刷新好友联系人")
            onTriggered: {
                console.log("刷新好友联系人")
                if (loaderForFG.status != Loader.Null) {
                    loaderForFG.item.close()
                }
                if (loaderForFriend.status != Loader.Null) {
                    loaderForFriend.item.close()
                }
                funcc.update()
            }
            background: Rectangle {
                implicitHeight: 30
                implicitWidth: 162
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: mouse.accepted = false
                    onHoveredChanged: {
                        if (containsMouse) {
                            parent.color = Qt.lighter("lightgray", 1.15)
                        } else {
                            parent.color = "transparent"
                        }
                    }
                }
            }
        }
        //显示在线联系人
        MenuItem {
            x: 1
            width: 162
            height: 30
            text: qsTr("显示在线联系人")

            onTriggered: {
                console.log("显示在线联系人")
            }
            background: Rectangle {
                implicitHeight: 30
                implicitWidth: 162
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: mouse.accepted = false
                    onHoveredChanged: {
                        if (containsMouse) {
                            parent.color = Qt.lighter("lightgray", 1.15)
                        } else {
                            parent.color = "transparent"
                        }
                    }
                }
            }
        }
        //分割条
        MenuItem {
            x: 1
            width: 162
            height: 11
            background: Rectangle {
                implicitHeight: 11
                implicitWidth: 162
                color: "transparent"
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: 1
                    color: "lightgray"
                }
            }
        }
        //添加分组
        MenuItem {
            x: 1
            width: 162
            height: 30
            text: qsTr("添加分组")
            onTriggered: {
                console.log("添加分组")
                //最多添加15个分组
                if (friendGroupModel.count() > 15) {
                    return
                }

                fgMenu.addIndex = fgMenu.indexOfPressed + 1 //标记添加索引
                var index = fgMenu.addIndex
                var obj = compFriend.createObject(qqMainWin)
                if (!obj) {
                    console.log("create a null object")
                    return
                }

                var length = friendGroupModel.rowCount()
                for (var i = length; i > index; --i) {
                    console.log(i, "=i")
                    friendsModel[i] = friendsModel[i - 1]
                }
                friendsModel[index] = obj
                viewFriend.model = null
                qqMainWin.friendDelegateCount = 0
                //更新插入
                friendGroupModel.insert(index, "未命名", "0", "0", "none")
                viewFriend.model = friendGroupModel //重赋值模型保证刷新视图
            }
            background: Rectangle {
                implicitHeight: 30
                implicitWidth: 162
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: mouse.accepted = false
                    onHoveredChanged: {
                        if (containsMouse) {
                            parent.color = Qt.lighter("lightgray", 1.15)
                        } else {
                            parent.color = "transparent"
                        }
                    }
                }
            }
        }

        MenuItem {
            id: rmenu
            x: 1
            width: 162
            height: 30
            text: qsTr("重命名")
            onTriggered: {
                console.log("重命名")
                var items = viewFriend.contentItem.children
                var item = items[fgMenu.indexOfPressed]
                if (loaderForFG.status == Loader.Ready)
                    loaderForFG.item.close()
                scrFriend.closeAllHovered()
                item.edit.text = item.listGroup.name.text
                item.edit.visible = true
                item.edit.selectAll()
                item.edit.forceActiveFocus()
            }
            background: Rectangle {
                implicitHeight: 30
                implicitWidth: 162
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: mouse.accepted = false
                    onHoveredChanged: {
                        if (containsMouse) {
                            parent.color = Qt.lighter("lightgray", 1.15)
                        } else {
                            parent.color = "transparent"
                        }
                    }
                }
            }
        }
        MenuItem {
            id: dmenu
            x: 1
            width: 162
            height: 30
            text: qsTr("删除该组")
            onTriggered: {
                var index = fgMenu.indexOfPressed
                var obj = {}
                obj.content = "dmrFGroup"
                obj.type = "delete"
                obj.index = index
                var model = friendsModel[index]
                // var length = friendGroupModel.count()
                friendsModel.splice(index, 1) //删除一个
                friendGroupModel.remove(index)
                var length = model.rowCount()
                if (length > 0) {
                    var fm = friendsModel[1]
                    for (var i = 0; i < length; ++i) {
                        var number = model.data(i, 0)
                        var name = model.data(i, 1)
                        var signature = model.data(i, 2)
                        var imgPath = model.data(i, 3)
                        var tag = model.data(i, 4)
                        var grade = model.data(i, 5)
                        var status = model.data(i, 6)
                        var infoSet = model.data(i, 7)
                        var statusSet = model.data(i, 8)
                        fm.append(number, name, signature, imgPath, tag, grade,
                                  status, infoSet, statusSet)
                        fm.sort() //排序更新
                    }
                }
                model.deletion() //释放内存
                funcc.updateFGroup(obj) //更新远程
            }
            background: Rectangle {
                implicitHeight: 30
                implicitWidth: 162

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: mouse.accepted = false
                    onHoveredChanged: {
                        if (containsMouse) {
                            parent.color = Qt.lighter("lightgray", 1.15)
                        } else {
                            parent.color = "transparent"
                        }
                    }
                }
            }
        }
        background: Rectangle {
            implicitHeight: 210
            implicitWidth: 164
            radius: 3
            border.color: "lightgray"
        }
    }
    //好友菜单
    Menu {
        property int fgIndex: -1
        property int fIndex: -1
        property int h: 154
        id: fMenu
        width: 156
        height: h
        visible: false
        topPadding: 6
        bottomPadding: 6
        leftPadding: 1
        rightPadding: 1
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        parent: scrFriend //视觉对象 开启相对对象坐标定位
        modal: true
        Overlay.modal: Rectangle {
            color: "#00000000"
        }
        //弹出时初始化添加索引
        onVisibleChanged: {
            if (visible) {
                fgMenu.visible = false
                if (loaderForFG.status == Loader.Ready) {
                    loaderForFG.item.visible = false
                }
                if (loaderForFriend.status == Loader.Ready) {
                    loaderForFriend.item.visible = false
                }
            }
        }
        //发送即时消息
        MenuItem {
            height: 30
            width: 154
            leftPadding: 12
            text: qsTr("发送即时消息")
            onTriggered: {
                try {
                    var number = friendsModel[fMenu.fgIndex].data(
                                fMenu.fIndex) //获取号码
                    sentFriendInfo(number)
                } catch (err) {
                    console.log(err.message)
                    return
                }
            }
        }
        //分割条
        MenuItem {
            width: 154
            height: 11
            background: Rectangle {
                implicitHeight: 11
                implicitWidth: 154
                color: "transparent"
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: 1
                    color: "lightgray"
                }
            }
        }
        //查看资料
        MenuItem {
            height: 30
            width: 154
            leftPadding: 12
            text: qsTr("查看资料")
            onTriggered: {
                console.log("查看资料")
                try {
                    var number = friendsModel[fMenu.fgIndex].data(
                                fMenu.fIndex) //获取号码
                    var name = friendsModel[fMenu.fgIndex].data(fMenu.fIndex,
                                                                1) //获取昵称
                    var sig = friendsModel[fMenu.fgIndex].data(fMenu.fIndex,
                                                               2) //获取签名
                    var img = friendsModel[fMenu.fgIndex].data(fMenu.fIndex,
                                                               3) //获取头像
                    var tag = friendsModel[fMenu.fgIndex].data(fMenu.fIndex,
                                                               4) //获取头像
                } catch (err) {
                    console.log(err.message)
                    return
                }
                var obj = {}
                obj.number = number
                obj.name = name
                obj.signature = sig
                obj.headImg = img
                obj.tag = tag
                obj.fgIndex = fMenu.fgIndex
                obj.last = fMenu.fgIndex
                openFriendDataWin(obj)
            }
        }
        //分割条
        MenuItem {
            width: 154
            height: 11
            background: Rectangle {
                implicitHeight: 11
                implicitWidth: 154
                color: "transparent"
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: 1
                    color: "lightgray"
                }
            }
        }
        //修改好友备注
        MenuItem {
            height: 30
            width: 154
            leftPadding: 12
            text: qsTr("修改好友备注")
            onTriggered: {
                var obj = {}
                var number = friendsModel[fMenu.fgIndex].data(fMenu.fIndex)
                obj.type = "alias"
                obj.number = number
                obj.index = fMenu.fgIndex
                loaderForAlterTag.obj = obj
                actions.openAlterTagAct.trigger() //执行对应行为
            }
        }

        //移动联系人至
        Menu {
            property string color: "transparent"
            id: mmenu
            width: 140
            height: 100
            title: " 移动联系人至"
            cascade: true
            topPadding: 6
            bottomPadding: 6
            leftPadding: 1
            rightPadding: 1
            delegate: MenuItem {
                id: cmenuItem
                implicitWidth: 154
                implicitHeight: 30

                onTriggered: {
                    console.log("移动联系人")
                    var length = mmenu.count
                    var index = -1
                    for (var i = 0; i < length; ++i) {
                        var item = mmenu.itemAt(i)
                        if (item == cmenuItem) {
                            index = i + 1
                        }
                    }

                    var b = moveFriend(fMenu.fgIndex, index,
                                       fMenu.fIndex) //移动好友
                    //更新资料分组
                    if (typeof b == "string") {
                        console.log("moveFriend(fMenu.fgIndex, index, fMenu.fIndex) return a string:",
                                    b)
                        try {
                            item = qqMainWin.mapInfo.get(b)
                            if (item != undefined) {
                                item.fgIndex = index //更新资料索引
                                item.last = index
                            }
                        } catch (e) {
                            console.log("warning:", e.message)
                        }
                    }
                }

                arrow: null
                indicator: null
                contentItem: Text {
                    leftPadding: 0
                    text: cmenuItem.text
                    font: cmenuItem.font
                    horizontalAlignment: Text.AlignHCenter
                    opacity: enabled ? 1.0 : 0.3
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    implicitWidth: 138
                    implicitHeight: 30
                    opacity: enabled ? 1 : 0.3
                    color: cmenuItem.highlighted ? Qt.lighter(
                                                       "lightgray",
                                                       1.15) : "transparent"
                }
            }
            background: Rectangle {
                implicitHeight: mmenu.height
                implicitWidth: 140
                radius: 3
                border.color: "lightgray"
            }
        }
        //删除好友
        MenuItem {
            id: dfmenu
            height: 30
            width: 154
            leftPadding: 12
            text: qsTr("删除好友")
            onTriggered: {
                console.log("删除好友")
                var index = fMenu.fgIndex
                var cindex = fMenu.fIndex
                var number = friendsModel[index].data(cindex)
                var item = viewFriend.contentItem.children[index]
                item.friend.model.remove(cindex)
                item.friend.model = null
                item.friend.model = friendsModel[index]
                friendGroupModel.update(index, 10000)
                funcc.deleteFriend(index, number) //更新远程
                //更新ismyself
                if (number == myqq) {
                    hasMyself = false
                }
            }
            onVisibleChanged: {
                if (visible) {
                    fMenu.h = 184
                } else {
                    fMenu.h = 154
                }
            }
        }
        delegate: MenuItem {
            id: menuItem
            implicitWidth: 154
            implicitHeight: 30
            arrow: Rectangle {
                x: parent.width - width
                implicitWidth: 30
                implicitHeight: 30
                color: "transparent"
                visible: menuItem.subMenu
                Image {
                    anchors.centerIn: parent
                    asynchronous: true
                    source: "qrc:images/mainInterface/arrowLeft.png"
                }
            }
            indicator: null
            contentItem: Text {
                leftPadding: 0
                text: menuItem.text
                font: menuItem.font
                opacity: enabled ? 1.0 : 0.3
                elide: Text.ElideRight
            }
            background: Rectangle {
                implicitWidth: 154
                implicitHeight: 30
                opacity: enabled ? 1 : 0.3
                color: menuItem.highlighted ? Qt.lighter("lightgray",
                                                         1.15) : "transparent"
            }
        }
        background: Rectangle {
            implicitHeight: fMenu.h
            implicitWidth: 156
            radius: 3
            border.color: "lightgray"
        }
    }
    //好友组菜单项
    Component {
        id: fgMItem
        Action {}
    }

    //非界面
    //城市天气模型 保存最多4个城市天气
    ListModel {
        id: cityWeatherModel
    }
    //好友组模型
    FriendGroupModel {
        id: friendGroupModel
        onGroupListChanged: {
            fgMItemTimer.restart() //延迟初始化菜单项
        }
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
            for (var i = 0; i < histroyImgModel.count; ++i) {
                console.log(histroyImgModel.get(i).url,
                            histroyImgModel.get(i).index)
            }
        }
    }

    //等级图标模型
    ListModel {
        id: gradeModel
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
        id: tipTimer500msForGrd
        interval: 500
        onTriggered: {
            gradeTip.close()
        }
    }
    //延迟开启分组hover
    Timer {
        id: timer50msForFG
        interval: 50
        onTriggered: {
            scrFriend.openAllHovered()
        }
    }
    //延迟处理添加好友处理
    Timer {
        id: timer300msDelayForAddFG
        interval: 300
        onTriggered: {
            var index = fgMenu.addIndex
            console.log("added a friend group located at ", index)
            var item = viewFriend.contentItem.children[index]
            scrFriend.closeAllHovered()
            item.edit.text = "未命名"
            item.edit.visible = true
            item.edit.selectAll()
            item.edit.forceActiveFocus()
        }
    }
    //当好友组变化时延迟初始化菜单
    Timer {
        id: fgMItemTimer
        interval: 2000
        onTriggered: {
            var length = mmenu.count
            //重置为零
            for (var i = 0; i < length; ++i) {
                var obj = mmenu.itemAt(i)
                if (obj != null) {
                    mmenu.removeItem(obj)
                }
            }
            length = friendGroupModel.count()
            //第一项不需要
            for (i = 1; i < length; ++i) {
                var name = friendGroupModel.data(i)
                obj = fgMItem.createObject(mmenu, {
                                               "text": name
                                           })
                obj = mmenu.addAction(obj)
            }
            mmenu.height = (i - 1) * 30 + 12
            funcc.updateAuxiliaryProcessFGoup(friendGroupModel.getGroups(
                                                  )) //更新辅助进程好友分组
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
            onPositionChanged: func.positionChange(mouse, -1, -1, qqMainWin,
                                                   sizeChanged)
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
                                                 qqMainWin, sizeChanged)
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
            onPositionChanged: func.positionChange(mouse, 1, -1, qqMainWin,
                                                   sizeChanged)
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
                                                 1, qqMainWin, sizeChanged)
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
                                                 qqMainWin, sizeChanged)
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

            onPositionChanged: func.positionChange(mouse, -1, 1, qqMainWin,
                                                   sizeChanged)
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
                                                 qqMainWin, sizeChanged)
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

            onPositionChanged: func.positionChange(mouse, 1, 1, qqMainWin,
                                                   sizeChanged)
        }
    }

    Component.onCompleted: {
        //初始化
        mainWin.width = qqMainWin.width
        mainWin.height = qqMainWin.height
        myqq = mainWin.myqq
        //获取对应文件 如头像 消息
        funcc.connectGetFile("2 headImg " + myqq)
        funcc.initWh()//爬取天气 因为网址原因 不能自动获取城市 故赋值了北京市
        funcc.mkDir(("../user/" + mainWin.myqq + "/weather"))//制造对应目录
    }
}
