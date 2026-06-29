// ============================================================
// BeautificationHub.qml — 系统美化入口页
//   列表选择：系统美化 / 选择安装
// ============================================================
import QtQuick
import QtQuick.Controls

Item {
    id: root

    Column {
        anchors {
            top: parent.top; topMargin: 0
            left: parent.left; leftMargin: 0
            right: parent.right; rightMargin: 0
        }
        spacing: 0

        Label {
            text: "系统美化"
            font { pixelSize: 20; weight: Font.Bold }
            color: "#1D1D1F"
            leftPadding: 30
            bottomPadding: 0
            height: 34
            verticalAlignment: Text.AlignVCenter
        }

        Item { height: 4; width: 1 }

        // ——— 选项 1：系统美化 ———
        Item {
            width: parent.width
            height: 34

            Rectangle {
                anchors {
                    left: parent.left; leftMargin: 24
                    right: parent.right; rightMargin: 24
                }
                height: parent.height
                y: 0
                radius: 8
                color: ma1.containsMouse ? "#D8D8E0" : "transparent"
            }

            Row {
                anchors {
                    left: parent.left; leftMargin: 40
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10

                Image {
                    source: "qrc:/res/WindowsToMac.png"
                    width: 18; height: 18
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    text: "系统美化"
                    font { pixelSize: 13 }
                    color: "#252525"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            MouseArea {
                id: ma1
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: appWindow.openThemeWizard()
            }
        }

        // ——— 选项 2：选择安装 ———
        Item {
            width: parent.width
            height: 34

            Rectangle {
                anchors {
                    left: parent.left; leftMargin: 24
                    right: parent.right; rightMargin: 24
                }
                height: parent.height
                y: 0
                radius: 8
                color: ma2.containsMouse ? "#D8D8E0" : "transparent"
            }

            Row {
                anchors {
                    left: parent.left; leftMargin: 40
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10

                Image {
                    source: "qrc:/res/box.png"
                    width: 18; height: 18
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    text: "选择安装"
                    font { pixelSize: 13 }
                    color: "#252525"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            MouseArea {
                id: ma2
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: contentStack.currentIndex = 2
            }
        }
    }
}
