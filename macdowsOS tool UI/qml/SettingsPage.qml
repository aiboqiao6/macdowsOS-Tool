// ============================================================
// SettingsPage.qml — 设置页面（macOS 27 风格）
// macOS "系统设置" 面板风格
// ============================================================
import QtQuick
import Backend 1.0
import QtQuick.Controls

Rectangle {
    id: root
    color: "transparent"

    ScrollView {
        anchors.fill: parent
        clip: true

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOff
        }

        Column {
            spacing: 14
            width: parent.width

            Label {
                text: "设置"
                font { pixelSize: 20; weight: Font.Bold }
                color: "#1D1D1F"
                bottomPadding: 10
            }

            // 桌面图标设置
            Rectangle {
                width: parent.width
                height: 56

                Row {
                    anchors {
                        left: parent.left; leftMargin: 18
                        right: parent.right; rightMargin: 18
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 10

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2
                        Label {
                            text: "桌面图标"
                            font { pixelSize: 14; weight: Font.Medium }
                            color: "#1D1D1F"
                        }
                        Label {
                            text: "显示或隐藏桌面图标"
                            font.pixelSize: 12
                            color: "#86868B"
                        }
                    }

                    Item { height: 1; width: 1 }

                    Button {
                        text: "隐藏"
                        implicitWidth: 64; implicitHeight: 30
                        anchors.verticalCenter: parent.verticalCenter
                        background: Rectangle { radius: 10; color: parent.down ? "#D0D0D5" : parent.hovered ? "#DCDCE0" : "#E8E8ED" }
                        contentItem: Label { text: "隐藏"; color: "#1D1D1F"; font.pixelSize: 12; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                        onClicked: Backend.setDesktopIcons(false)
                    }

                    Button {
                        text: "显示"
                        implicitWidth: 64; implicitHeight: 30
                        anchors.verticalCenter: parent.verticalCenter
                        background: Rectangle { radius: 10; color: parent.down ? "#D0D0D5" : parent.hovered ? "#DCDCE0" : "#E8E8ED" }
                        contentItem: Label { text: "显示"; color: "#1D1D1F"; font.pixelSize: 12; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                        onClicked: Backend.setDesktopIcons(true)
                    }
                }
            }

            
            Item { width: 1; height: 24 }
        }
    }
}
