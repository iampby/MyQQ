import QtQuick 2.11

MouseArea {
    width: parent.width;height: parent.height
    property point startPoint: Qt.point(0, 0)
    signal sendPos(point movedCoordinate)

    onPressed: {
        startPoint.x = mouseX
        startPoint.y = mouseY
        console.log("mouseX=", mouseX, "mouseY=", mouseY) //测试位置
    }
    //公式：窗口位置+=鼠标位置-起始位置；实现移动效果
    onPositionChanged: {
        if (pressed) {
            //parent.x += mouseX - startPoint.x //利用parent设置坐标的方法不适用loader加载的窗口类 可能是加载方式不一样导致
           // parent.y += mouseY - startPoint.y
            sendPos(Qt.point(mouseX - startPoint.x, mouseY - startPoint.y))
        }
    }
}
