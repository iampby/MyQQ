import QtQuick 2.11

Item {
    //注意，不知道是loader原因还是什么的  含有函数的部件属性容易异常,即最好不要设置任何属性
    signal makeRequestFinished
    signal sizeChanged(int w, int h, point delta, int directX, int directY)
    function mainWinReSize(x, y, width, height) {
        //调整main窗口大小
        mainWin.width = width
        mainWin.height = height
        mainWin.x = x
        mainWin.y = y
    }
    function timeForTipVisibleHandler() {
        xTip = xTemp
        yTip = yTemp
        isVisible = true
        console.log("timer->", xTip, yTip, isVisible)
    }
    function mainWinDpChanged() {
        if (mainWin.inf === Math.pow(2, 0)) {
            //调整登录界面,因为小于800*600,只调整位置
            console.log("inf =0")
            if (mainWin.preDp === 0.0) {
                mainWin.preDp = mainWin.dp
                return
            }
            if (mainWin.preDp < mainWin.dp) {
                mainWin.preDp = mainWin.dp
                return
            } else if (mainWin.preDp > mainWin.dp) {
                mainWin.preDp = mainWin.dp
                var nx = (mainWin.desktopAvailableWidth - mainWin.width) / 2
                if (mainWin.x - (mainWin.desktopAvailableWidth - 425) > 425 / 2)
                    mainWin.x = nx
                else
                    mainWin.x = mainWin.x * mainWin.dp
                mainWin.y = (mainWin.desktopAvailableHeight - mainWin.height) / 2
            } else {
                mainWin.x = (mainWin.desktopAvailableWidth - mainWin.width) / 2
                mainWin.y = (mainWin.desktopAvailableHeight - mainWin.height) / 2
            }
        } else if (mainWin.inf === Math.pow(2, 1)) {
            //调整注册界面,由于界面大于800*600，对此作出调整
            if (mainWin.desktopAvailableWidth === 800) {
                inCenterLoader.item.lengthR = 500
                inCenterLoader.item.dpJust = mainWin.dp
                mainWin.width = 800
                mainWin.x = (mainWin.desktopAvailableWidth - mainWin.width) / 2
                mainWin.y = (mainWin.desktopAvailableHeight - mainWin.height) / 2
            } else {
                mainWin.width = 850
                inCenterLoader.item.lengthR = 550
                inCenterLoader.item.dpJust = 1.0
                mainWin.x = (mainWin.desktopAvailableWidth - mainWin.width) / 2
                mainWin.y = (mainWin.desktopAvailableHeight - mainWin.height) / 2
            }
            mainWin.preDp = mainWin.dp
        }
    }
    //初始化qqmainwin坐标为右贴任务栏
    //qqmainwin的初始化x坐标
    function qqMainX() {
        var x
        if (funcc.taskDirection() !== 3) {
            x = mainWin.desktopAvailableWidth - inCenterLoader.item.width - 1
        } else {
            x = mainWin.desktopAvailableWidth - inCenterLoader.item.width + funcc.taskBarWidth(
                        ) - 1
        }
        console.log("qqMainX->", x)
        return x
    }
    //qqmainwin的初始化y坐标
    function qqMainY() {
        var y
        if (funcc.taskDirection() !== 1) {
            y = 10
        } else {
            y = mainWin.desktopAvailableHeight - inCenterLoader.item.height + funcc.taskBarWidth(
                        ) + 10
        }
        console.log("qqMainY->", y)
        return y
    }
    //js爬虫与json练习，获取当天pm,温度，风向 参数string
    function makeRequest(url, array) {
        var req = new XMLHttpRequest()
        req.onreadystatechange = function () {
            if (req.readyState == 4) {
                var a = req.responseText
                a = a.match(/{\"od\":.+\"}]}}/)

                if (a === null)
                    return
                var _24hData = JSON.parse(a)
                array.windGrade = _24hData.od.od2[24].od25
                array.temperature = _24hData.od.od2[0].od21
                array.wind = _24hData.od.od2[0].od24
                array.pm = _24hData.od.od2[24].od28
                console.log(" array.pm", array.pm)
                makeRequestFinished()
            }
        }
        req.open("GET", url)
        req.send()
    }
    //返回空气质量等级，参数为pm，返回值等级字符串，如优
    function aql(pm) {
        switch (true) {
        case (0 <= pm && pm <= 50):
            console.log("优")
            return "优"
        case (51 <= pm && pm <= 100):
            console.log("良")
            return "良"
        case (101 <= pm && pm <= 150):
            console.log("轻度污染")
            return "轻度污染"
        case (151 <= pm && pm <= 200):
            return "中度污染"
        case (201 <= pm && pm <= 300):
            return "重度污染"
        default:
            return "严重污染"
        }
    }
    //返回图片路径:qrc:/... 参数：天气字符串如晴
    //因为数据有限，只能简单判别几种天气，加载对应图片
    function getImgPathForWeather(weather) {
        var partPath = "qrc:/images/mainInterface/weather/"
        if (weather.indexOf("晴") != -1)
            return partPath + "晴.png"
        else if (weather.indexOf("多云") != -1)
            return partPath + "多云.png"
        else if (weather.indexOf("阴") != -1)
            return partPath + "阴.png"
        else if (weather.indexOf("雷阵雨") != -1)
            return partPath + "雷阵雨.png"
        else if (weather.indexOf("雨") != -1)
            return partPath + "小雨.png"
        else if (weather.indexOf("雪") != -1)
            return partPath + "小雪.png"
        else if (weather.indexOf("雾") != -1)
            return partPath + "雾.png"
        else
            return partPath + "多云.png" //如出意外天气 返回默认多云
    }
    //进入鼠标区域时改变鼠标形状
    function enter(direct, win) {
        funcc.setMyCursor(direct, win)
    }
    //按下时做按标识
    function press(mouse, win) {
        win.isPressed = true
        win.customPoint = Qt.point(mouse.x, mouse.y)
    }
    //离开鼠标区域 重置按标识
    function release(win) {
        win.isPressed = false
    }
    //拖拽移动处理
    function positionChange(newPosition, directX, directY, win/*x轴方向*/ /*y轴方向*/ ) {
        if (!win.isPressed)
            return
        var delta = Qt.point(newPosition.x - win.customPoint.x,
                             newPosition.y - win.customPoint.y)
        var tmpW, tmpH

        if (directX >= 0) {
            tmpW = win.width + delta.x
        } else {
            tmpW = win.width - delta.x
        }
        if (directY >= 0) {
            tmpH = win.height + delta.y
        } else {
            tmpH = win.height - delta.y
        }
        sizeChanged(tmpW, tmpH, delta, directX, directY) //发送变化后的大小，鼠标偏差，鼠标所在区域
    }

    //用于化帆布的边界虚线
    function dottedLinTo(context, sx, sy, ex, ey, length) {
        //获取线条的水平宽度
        var w = ex - sx
        //获取线条的垂直高度
        var h = ey - sy
        //高的线条个数
        var verLength = Math.floor(length / 2 * (h / (w + h)))
        //宽的线条个数
        var horLength = Math.floor(length / 2 * (w / (w + h)))
        //偶数个使资源不浪费
        if (verLength % 2 != 0)
            verLength += 1
        if (horLength % 2 != 0)
            horLength += 1
        var twoMove = {
            "x": 0.0,
            "y": 0.0
        }
        for (var i = 0; i < verLength; i++) {
            if (i % 2 == 0) {
                context.moveTo(sx, sy + h / verLength * i)
                twoMove.x = ex
                twoMove.y = sy + h / verLength * i
            } else {
                context.lineTo(sx, sy + h / verLength * i)
                context.moveTo(twoMove.x, twoMove.y)
                context.lineTo(ex, sy + h / verLength * i)
            }
        }
        for (i = 0; i < horLength; i++) {
            if (i % 2 == 0) {
                context.moveTo(sx + w / horLength * i, sy)
                twoMove.x = sx + w / horLength * i
                twoMove.y = ey
            } else {
                context.lineTo(sx + w / horLength * i, sy)
                context.moveTo(twoMove.x, twoMove.y)
                context.lineTo(sx + w / horLength * i, ey)
            }
        }
        context.stroke() //画轮廓
    }
    //获取字符串字节数
    function getCharByteLength(str) {
        var bytesCount = 0
        for (var i = 0; i < str.length; i++) {
            var c = str.charAt(i)
            if (/^[\u0000-\u00ff]$/.test(c)) //匹配双字节
            {
                bytesCount += 1
            } else {
                bytesCount += 2
            }
        }
        return bytesCount
    }
    function addHeadUrl() {
        console.log("addhandlehead")
        images.readHistory()
    }
    //返回升级天数
    function computeToUpgrade(grade, activeDays) {
        grade += 1
        var sum = (grade + 4) * grade
        var remain = sum - parseInt(activeDays)
        console.log("remain=", remain)
        return remain
    }
    //数组 年 月 日 返回年龄
    function getAge(date) {
        var cur = new Date().toJSON().substring(0, 10)
        var curarr = cur.split("-")
        var age = 0
        age = parseInt(curarr[0]) - parseInt(date[0])
        if (parseInt(curarr[1]) < parseInt(date[1])) {
            age -= 1
        } else if (parseInt(curarr[1]) === parseInt(date[1])) {
            if (parseInt(curarr[2]) < parseInt(date[2]))
                age -= 1
        }
        console.log("age=", age)
        return age
    }
    //数组 年 月 日 返回Q龄
    function getQAge(date) {
        var cur = new Date().toJSON().substring(0, 10)
        var curarr = cur.split("-")
        var age = 0
        age = parseInt(curarr[0]) - parseInt(date[0])
        if (parseInt(curarr[1]) < parseInt(date[1])) {
            age -= 1
        } else if (parseInt(curarr[1]) === parseInt(date[1])) {
            if (parseInt(curarr[2]) < parseInt(date[2]))
                age -= 1
        }
        console.log("Q age=", age)
        return age
    }
    //birarr包含日期的数组  xxxx xx xx 返回星座
    function getConstellation(birArr) {
        var m = parseInt(birArr[1])
        var d = parseInt(birArr[2])
        var constellation
        switch (m) {
        case 1:
            if (d > 19)
                constellation = "水瓶座"
            else
                constellation = "摩羯座"
            break
        case 2:
            if (d > 18)
                constellation = "双鱼座"
            else
                constellation = "水瓶座"
            break
        case 3:
            if (d > 20)
                constellation = "白羊座"
            else
                constellation = "双鱼座"
            break
        case 4:
            if (d > 19)
                constellation = "金牛座"
            else
                constellation = "白羊座"
            break
        case 5:
            if (d > 20)
                constellation = "双子座"
            else
                constellation = "金牛座"
            break
        case 6:
            if (d > 20)
                constellation = "巨蟹座"
            else
                constellation = "双子座"
            break
        case 7:
            if (d > 22)
                constellation = "狮子座"
            else
                constellation = "巨蟹座"
            break
        case 8:
            if (d > 22)
                constellation = "处女座"
            else
                constellation = "狮子座"
            break
        case 9:
            if (d > 22)
                constellation = "天秤座"
            else
                constellation = "处女座"
            break
        case 10:
            if (d > 22)
                constellation = "天蝎座"
            else
                constellation = "天秤座"
            break
        case 11:
            if (d > 22)
                constellation = "射手座"
            else
                constellation = "天蝎座"
            break
        case 12:
            if (d > 21)
                constellation = "摩羯座"
            else
                constellation = "射手座"
            break
        }
        return constellation
    }
}
