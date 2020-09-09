

/*
    必须设置属性
    property var target
    property var lengthvalue  //记录外面大的区域的大小 和target不同 Point(x,y) x:width, y:height
*/
import QtQuick 2.9

Rectangle {
    id: scrollBar
    property var target
    property var lengthvalue

    //记录外面大的区域的大小 和target不同 Point(x,y) x:width, y:height
    property var orientation: Qt.Vertical //竖直
    property var ispress: false
    property var m_step: 1 //变化比例

    signal poschange(var mouseX, var mouseY)

    width: orientation == Qt.Vertical ? 6 : target.width
    height: orientation == Qt.Vertical ? target.height : 6
    anchors.right: orientation == Qt.Vertical ? target.right : undefined
    anchors.left: orientation == Qt.Vertical ? undefined : target.left

    radius: 3
    z: target.z + 1
    color: "#ff80ffff"
    opacity: 0.01

    Rectangle {
        id: crect
        color: "lightgray"
        radius: 5
        z: scrollBar.z + 1
        property bool mishow: false

        //位置
        width: orientation == Qt.Vertical ? 6 : 50
        height: orientation == Qt.Vertical ? 50 : 6

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true

            property var p_old

            drag.filterChildren: true
            drag.target: crect

            drag.axis: orientation == Qt.Vertical ? Drag.YAxis : Drag.XAxis

            drag.minimumX: orientation == Qt.Vertical ? 0 : 0
            drag.maximumX: orientation == Qt.Vertical ? 0 : Math.ceil(
                                                            scrollBar.width - crect.width)

            drag.minimumY: orientation == Qt.Vertical ? 0 : 0
            drag.maximumY: orientation == Qt.Vertical ? Math.ceil(
                                                            scrollBar.height - crect.height) : 0

            onMouseXChanged: {
                if (!scrollBar.ispress)
                    return
                if (scrollBar.orientation == Qt.Vertical)
                    return

                var offset_x = crect.x
                scrollBar.poschange(-offset_x * scrollBar.m_step, 0)
            }
            onMouseYChanged: {

                if (!scrollBar.ispress)
                    return
                if (scrollBar.orientation != Qt.Vertical)
                    return

                var offset_y = Math.ceil(crect.y)
                scrollBar.poschange(0, -offset_y * scrollBar.m_step)
            }

            //鼠标按住
            onPressed: {
                if (crect.mishow) {
                    scrollBar.ispress = true
                } else
                    scrollBar.ispress = false
                p_old = Qt.point(crect.x, crect.y)
            }
            onReleased: {
                scrollBar.ispress = false
            }
        }
    }

    Component.onCompleted: {
        //加载完成是设置长度
        if (orientation == Qt.Vertical) {
            var hs = scrollBar.height * scrollBar.height / lengthvalue.y
            crect.height = Math.floor(hs)
            m_step = (lengthvalue.y) / scrollBar.height
            if (scrollBar.height < lengthvalue.y)
                crect.mishow = true
        } else {
            var hs = scrollBar.width * scrollBar.width / lengthvalue.x
            crect.width = Math.floor(hs)
            m_step = (lengthvalue.x) / scrollBar.width
            if (scrollBar.width < lengthvalue.x)
                crect.mishow = true
        }
    }
}
