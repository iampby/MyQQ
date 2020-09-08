import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Window 2.11
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.0
import FuncC 1.0
import QtQuick.Particles 2.0
import QtWinExtras 1.0
import Model 1.0

import "qrc:/login"
import "qrc:/register"
import "qrc:/main"

//本应用通过Qml开发界面，Qt C++开发底层逻辑
ApplicationWindow {
    property var list: []
    //公式：px=dp*(ppi/160) -> px/dp=ppi/160=Screen.devicePixelRatio(不固定),这个程序只是用来勘测分辨率变化，dp是用于安卓适配屏幕的
    property real dp: Screen.pixelDensity * 25.4 / 16 * 1.43 //初始化可变数值时会自动发送一次信号
    property real preDp: 0.0
    property int inf: Math.pow(2, 0)
    //初始化为屏幕大小，便于实时获取屏幕可利用大小
    property int desktopAvailableHeight: Screen.desktopAvailableHeight
    property int desktopAvailableWidth: Screen.desktopAvailableWidth
    property string myqq: funcc.myQQ
    property string passwd: funcc.passwd

    // signal dpChanged 无需定义，属性自动生成槽函数与信号
    //打开消息框信号
    signal trayClicked
    id: mainWin
    // x: (mainWin.desktopAvailableWidth / 2 - width / 2) * dp
    // y: (mainWin.desktopAvailableHeight / 2 - height / 2) * dp //实时更新属性赋值后会无效,需重新绑定
    visible: true
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint //取消窗口边缘框架 topmost（任务栏之上）
    //property alias actions: actions
    width: 495
    height: 470 //透明主界面大小
    color: "#00000000"

    onClosing: {
        console.log("main close")
        //释放资源
        if (mainWin.inf == Math.pow(2, 2)) {
            try {
                inCenterLoader.item.close() //释放qqMainWin
            } catch (err) {
                console.log(err.message) //打印编译器错误提示
            }
        }

        // funcc.closeWidget()
    }
    title: qsTr("MyQQ")
    onDpChanged: func.mainWinDpChanged()
    Func {
        id: func
    }
    FuncC {
        id: funcc
    }

    Actions {
        id: actions
    }
    FontLoader {
        id: song
        name: "宋体"
    }
    //托盘
    SystemTrayIcon {
        property bool hasInfo: false
        property string midSource: "./images/QQTray.png"
        property string initSource: "./images/QQTray.png"
        signal reset
        id: tray
        visible: true
        iconSource: initSource


        onReset: {
            console.log("A dialog box will be opened")
            hasInfo = false
            timer1s.stop()
            tooltip = ""
            iconSource = initSource
        }
        onActivated: {
            //打开对话框
            if (hasInfo) {
                reset()
                trayClicked()
                return
            }
            if (mainWin.inf != Math.pow(2, 2)) {
                mainWin.show()
                mainWin.raise()
                mainWin.requestActivate()
            } else {
                inCenterLoader.item.show()
                inCenterLoader.item.raise()
                inCenterLoader.item.requestActivate()
            }
        }
    }
    Loader {
        id: inCenterLoader
        anchors.centerIn: parent
        asynchronous: true //异步加载更快，防止加载慢导致 mainWinReSize 函数效果不理想
        sourceComponent: comptLogin
        // source: "./register/RegisterInterface.qml"
        visible: status == Loader.Ready
        onLoaded: {
            //对Component和Qml文件组件都起作用,当组件加载完成时，才能调用dpChanged启动对组件调整
            dpChanged() //调整位置
            funcc.initImageSet(images) //初始化图片提供器images给funcc,便于调用
        }
    }
    Component {
        id: comptLogin
        LoginTotalInterface {}
    }
    //funcc连接信号器
    Connections {
        target: funcc
        //验证信息到来
        onEmitFVeify: {
            console.log("the qml get a friend verify:")
            tray.iconSource = "qrc:/images/bellinfo.png"
            tray.midSource = "qrc:/images/bellinfo.png"
            tray.tooltip = "有新消息到来"
            timer1s.start()
            tray.hasInfo = true //有信息标记
        }
    }
    //tray定时器 闪烁效果
    Timer {
        id: timer1s
        repeat: true
        onTriggered: {
            if (tray.iconSource != "") {
                tray.iconSource = ""
            } else
                tray.iconSource = tray.midSource
        }
    }
    Component.onCompleted: {
        //声明时间格式化函数
        //原型方法
        Date.prototype.format = function (fmt) {

            var o = {
                "M+": this.getMonth() + 1,
                "d+"//月份
                : this.getDate(),
                "h+"//日
                : this.getHours(),
                "m+"//小时
                : this.getMinutes(),
                "s+"//分
                : this.getSeconds(),
                "q+"//秒
                : Math.floor((this.getMonth() + 3) / 3),
                "S"//季度
                : this.getMilliseconds() //毫秒
            }
            if (/(y+)/.test(fmt))
                fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(
                                      4 - RegExp.$1.length))
            for (var k in o)
                if (new RegExp("(" + k + ")").test(fmt))
                    fmt = fmt.replace(
                                RegExp.$1,
                                (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(
                                                                        ("" + o[k]).length)))
            return fmt
        }
    }

    //不能在窗口里创建函数，会导致其x,y等属性只读


    /* function x() {
        mainWin.inf = Math.pow(2, 2) //标记为main界面
        inCenterLoader.sourceComponent = undefined
        inCenterLoader.source = "./main/mainInterface.qml"
        mainWin.hide()
    }*/
}
