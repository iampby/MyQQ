import QtQuick 2.11

//loader部件的移动
MouseArea {
    anchors.fill: parent
    property point startPoint: Qt.point(0, 0)
    property bool isLoginInf: false //是否是登入界面
    propagateComposedEvents: true
    onPressed: {
        //当pressed和clicked、released同时存在时，注意相同属性的赋值，pressed响应在前，click后响应，released在中间
        if (isLoginInf) {
            //登入界面
            if ((mouseX > 338 & mouseY < 32) | //头像范围->
                    (Math.pow(mouseX - 214) + Math.pow(
                         mouseY - 124) <= Math.pow(33)) | //头像状态图标范围->
                    (Math.pow(mouseX - 231) + Math.pow(
                         mouseY - 137) <= 64) | //编辑区域->
                    (mouseX > 118 & mouseX < 340
                     & ((mouseY > 163 & mouseY < 189)
                        | (mouseY > 202 & mouseY < 228))) | //选中框区域与找回密码区域->
                    (mouseY > 250 & mouseY < 262 & ((mouseX > 81 & mouseX < 97) | (mouseX > 178 & mouseX < 194) | (mouseX > 276 & mouseX < 328))) | //注册区域
                    (mouseX > 9 & mouseX < 59 & mouseY > 303 & mouseY < 314) | //二维码区域->
                    (mouseX > 399 & mouseX < 422 & mouseY > 296 & mouseY < 322) | //登入区域->
                    (mouseX > 89 & mouseX < 339 & mouseY > 279 & mouseY < 311)) {
                mouse.accepted = false // 鼠标不继续接受pressed;pressed继续为false,且鼠标不接受released信号;所以需要限定区域执行
            } else {
                mouse.accepted = true
                startPoint.x = mouseX
                startPoint.y = mouseY
                console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
                parent.account.loseFocus() //调用函数不可用省略括号
                parent.passwd.loseFocus() //发送丢死焦点信号
            }
        } else {
            startPoint.x = mouseX
            startPoint.y = mouseY
            console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
        }
    }
    //公式：窗口位置+=鼠标位置-起始位置；实现移动效果
    onPositionChanged: {
        if (pressed) {
            mainWin.x += mouseX - startPoint.x
            mainWin.y += mouseY - startPoint.y
        }
    }
}
