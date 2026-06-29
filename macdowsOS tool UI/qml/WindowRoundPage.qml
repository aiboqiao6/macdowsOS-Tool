// ============================================================
// WindowRoundPage.qml — 窗口圆角配置页（macOS 27 风格）
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
                text: "窗口圆角工具"
                font { pixelSize: 20; weight: Font.Bold }
                color: "#1D1D1F"
                bottomPadding: 6
            }

            Label {
                text: "提示：此功能目前处于实验性阶段，请谨慎使用。"
                font.pixelSize: 13
                color: "#E65100"
                wrapMode: Text.WordWrap
                width: parent.width
                bottomPadding: 10
            }

            // 使用说明
            Rectangle {
                width: parent.width
                height: 160

                Column {
                    anchors {
                        left: parent.left; leftMargin: 18
                        top: parent.top; topMargin: 18
                        right: parent.right; rightMargin: 18
                    }
                    spacing: 10

                    Label {
                        text: "使用说明"
                        font { pixelSize: 15; weight: Font.Medium }
                        color: "#1D1D1F"
                    }

                    Label { text: "1. 在 config 目录下创建配置文件，指定要修改的窗口名称"; font.pixelSize: 13; color: "#6E6E73"; wrapMode: Text.WordWrap; width: parent.width - 36 }
                    Label { text: "2. 配置完成后，点击下方按钮开始修改"; font.pixelSize: 13; color: "#6E6E73"; wrapMode: Text.WordWrap; width: parent.width - 36 }
                    Label { text: "3. 修改完成后需要重启资源管理器生效"; font.pixelSize: 13; color: "#6E6E73"; wrapMode: Text.WordWrap; width: parent.width - 36 }
                }
            }

            // 打开配置目录
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
                        Label { text: "打开配置文件目录"; font.pixelSize: 14; font.weight: Font.Medium; color: "#1D1D1F" }
                        Label { text: "在 config 目录中创建或编辑配置文件"; font.pixelSize: 12; color: "#86868B" }
                    }

                    Item { height: 1; width: 1 }

                    Button {
                        text: "打开目录"
                        implicitWidth: 84; implicitHeight: 30
                        anchors.verticalCenter: parent.verticalCenter
                        background: Rectangle { radius: 10; color: parent.down ? "#0066CC" : parent.hovered ? "#0066CC" : "#007AFF" }
                        contentItem: Label { text: "打开目录"; color: "#FFFFFF"; font.pixelSize: 12; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                        onClicked: Qt.openUrlExternally("./config")
                    }
                }
            }

            // 重启资源管理器
            Rectangle {
                width: parent.width
                height: 48

                Row {
                    anchors {
                        left: parent.left; leftMargin: 18
                        right: parent.right; rightMargin: 18
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 10

                    Label {
                        text: "修改完成后需要重启资源管理器"
                        font.pixelSize: 13
                        color: "#F57C00"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Item { height: 1; width: 1 }

                    Button {
                        text: "重启资源管理器"
                        implicitWidth: 140; implicitHeight: 30
                        anchors.verticalCenter: parent.verticalCenter
                        background: Rectangle { radius: 10; color: parent.down ? "#0066CC" : parent.hovered ? "#0066CC" : "#007AFF" }
                        contentItem: Label { text: "重启资源管理器"; color: "#FFFFFF"; font.pixelSize: 12; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                        onClicked: Backend.restartExplorer()
                    }
                }
            }

            Item { width: 1; height: 24 }
        }
    }
}
