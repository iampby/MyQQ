import QtQuick 2.0
import QtQuick.Controls 2.5
import "../"

//资料界面的更多信息小部件 ，如公司 xxx 标签
Item {
    property alias preLabel: preLabel
    property alias aftLabel: aftLabel
    width: aftLabel.width + 70
    height: aftLabel.height
    Label {
        id: preLabel
        leftPadding: 0
        font.pointSize: 10
        font.family: "宋体"
        color: "gray"
    }
    Label {
        id: aftLabel
        x: 70
        leftPadding: 0
        font.family: "宋体"
        font.pointSize: 10
        MouseForShape {
            shapeInside: Qt.SizeVerCursor
            shapeOutside: Qt.ArrowCursor
        }
    }
}
