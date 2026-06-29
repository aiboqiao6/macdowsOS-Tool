// ============================================================
// AboutPage.qml — 关于页面（macOS 27 风格）
// 仿 macOS "关于本机" 面板
// ============================================================
import QtQuick
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

            // macOS "关于本机" 风格顶部
            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12
                topPadding: 24

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 64
                    height: 64
                    radius: 14
                    color: "#007AFF"
                    visible: false
                }

                Image {
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/res/WindowsToMac.png"
                    width: 72
                    height: 72
                    fillMode: Image.PreserveAspectFit
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "macdowsOS Tool"
                    font { pixelSize: 22; weight: Font.Bold }
                    color: "#1D1D1F"
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "版本 1.0"
                    font.pixelSize: 14
                    color: "#86868B"
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 260
                    height: 1
                    color: "#D2D2D7"
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "将 Windows 界面转换为 macOS 风格"
                    font.pixelSize: 13
                    color: "#6E6E73"
                }
            }

            // 免责声明
            Rectangle {
                width: parent.width
                height: 140

                Column {
                    anchors {
                        left: parent.left; leftMargin: 18
                        top: parent.top; topMargin: 18
                        right: parent.right; rightMargin: 18
                    }
                    spacing: 8

                    Label { text: "免责声明"; font.pixelSize: 15; font.weight: Font.Bold; color: "#C62828" }
                    Label { text: "用户应自行确保所使用的所有组件/工具/软件均符合版权法规。本程序仅用于学习研究Windows程序控制原理及C++基础知识，禁止用于其他任何用途。使用macdowsOS Tool即表示用户同意以上内容且同意自行承担相关风险。"; font.pixelSize: 12; color: "#6E6E73"; wrapMode: Text.WordWrap; width: parent.width - 36; lineHeight: 1.6 }
                }
            }

            // 反诈中心
            Rectangle {
                width: parent.width
                height: 68

                Column {
                    anchors {
                        left: parent.left; leftMargin: 18
                        right: parent.right; rightMargin: 18
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 4
                    Label { text: "反诈中心"; font.pixelSize: 15; font.weight: Font.Bold; color: "#F57C00" }
                    Label { text: "此工具完全免费，如果你是付费获得，说明你被骗了。"; font.pixelSize: 13; color: "#E65100"; wrapMode: Text.WordWrap; width: parent.width - 36 }
                }
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "此程序使用 Qt Quick (QML) 构建"
                font.pixelSize: 12
                color: "#A1A1A6"
                bottomPadding: 24
            }
        }
    }
}
