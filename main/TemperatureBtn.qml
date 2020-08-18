import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
//天气按钮 包含信息：如今天温度、天气图标
Button {
    property alias whichDay: whichday.text
    property alias temperature: tem.text
    property alias img: img.source
    property alias back: back
    width: 78
    height: 106
    background: Rectangle {
        id: back
        implicitHeight: 106
        implicitWidth: 78
        color: "transparent"
        border.color: "lightgray"
        border.width: 0
    }
    ColumnLayout {
        width: 78
        Label {
            id: whichday
            Layout.alignment: Qt.AlignHCenter
            font.pointSize: 13
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("今天")
        }
        Label {
            id: tem
            Layout.alignment: Qt.AlignHCenter
            font.pointSize: 13
            text: "25℃"
            horizontalAlignment: Text.AlignHCenter
        }
        Image {
            id: img
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            fillMode: Image.Stretch
            source: "qrc:/images/mainInterface/weather/多云.png"
        }
    }
}
