import QtQuick 2.11
//窗口对象的移动
MouseArea {
    width: parent.width
    height: parent.height
    property point startPoint: Qt.point(0, 0)
    signal sendPos(point movedCoordinate)

    onPressed: {
        startPoint.x = mouseX
        startPoint.y = mouseY
    }
    //公式：窗口位置+=鼠标位置-起始位置；实现移动效果
    onPositionChanged: {
        if (pressed) {
            sendPos(Qt.point(mouseX - startPoint.x, mouseY - startPoint.y))
        }
    }
}
