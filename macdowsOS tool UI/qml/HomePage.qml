// ============================================================
// HomePage.qml — macOS 27 风格首页/欢迎页
// 居中布局，仿 macOS "关于本机" 风格
// ============================================================
import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    color: "transparent"

    Column {
        anchors.centerIn: parent
        spacing: 10

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "macdowsOS Tool"
            font { pixelSize: 40; weight: Font.Bold }
            color: "#1D1D1F"
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "自动化的Windows仿macOS美化工具"
            font.pixelSize: 15
            color: "#6E6E73"
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1
        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 220
            height: 1
            color: "#D2D2D7"
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "版本 1.22"
            font.pixelSize: 14
            color: "#86868B"
        }

        

        

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 14
            topPadding: 12

           
           
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "使用左侧导航栏选择要执行的操作"
            font.pixelSize: 13
            color: "#A1A1A6"
            topPadding: 12
        }
    }
}
