import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Window 2.11
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.0
import "qrc:/login"
import "qrc:/register"
import "qrc:/main"
import FuncC 1.0
import QtQuick.Particles 2.0
import QtWinExtras 1.0

//本应用通过Qml开发界面，Qt C++开发底层逻辑
ApplicationWindow {
    id: mainWin
    visible: true
    // x: (mainWin.desktopAvailableWidth / 2 - width / 2) * dp
    // y: (mainWin.desktopAvailableHeight / 2 - height / 2) * dp //实时更新属性赋值后会无效,需重新绑定
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
    signal adjustCoordination
    //获得所有的设置信息  过渡信号
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint //取消窗口边缘框架 topmost（任务栏之上）
    //property alias actions: actions
    width: 495
    height: 470 //透明主界面大小
    color: "#00000000"

    onClosing: {
        console.log("main close")
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

    SystemTrayIcon {
        visible: true
        iconSource: "./images/QQTray.png"
        onActivated: {
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
            funcc.initImageSet(images);//初始化图片提供器images给funcc,便于调用
        }
    }
    Component {
        id: comptLogin
        LoginTotalInterface {}
    }

    //不能在窗口里创建函数，对导致其x,y等属性只读


    /* function x() {
        mainWin.inf = Math.pow(2, 2) //标记为main界面
        inCenterLoader.sourceComponent = undefined
        inCenterLoader.source = "./main/mainInterface.qml"
        mainWin.hide()
    }*/
}
