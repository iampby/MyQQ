import QtQuick 2.11


/*
  设置鼠标形状的组件
 */
MouseArea {
    property int shapeInside: 0
    property int shapeOutside: 0
    width: parent.width
    height: parent.height
    hoverEnabled: true
    propagateComposedEvents: true
    //必须定义pressed事件才能被底层控件接收且mouse.accepted = false不可省略,accepted默认为false
    onPressed: mouse.accepted = false
    onContainsMouseChanged: {
        if (containsMouse) {
            cursorShape = shapeInside
        } else
            cursorShape = shapeOutside
    }
}
