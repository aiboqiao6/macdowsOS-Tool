// ============================================================
// CeleCraftPage.qml — 选择性安装页面（macOS 27 风格）
// macOS 偏好设置面板风格
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

            // 页面标题
            Label {
                text: "选择安装"
                font { pixelSize: 20; weight: Font.Bold }
                color: "#1D1D1F"
                bottomPadding: 6
            }

            Label {
                text: "单独安装某项美化组件"
                font.pixelSize: 13
                color: "#86868B"
                wrapMode: Text.WordWrap
                width: parent.width
                bottomPadding: 10
            }

            // 安装项目列表 — macOS 偏好设置风格
            Repeater {
                model: [
                    { name: "安装 myDockFinder", desc: "macOS 风格 Dock 栏", action: function() { Backend.installMyDockFinder(); } },
                    { name: "安装系统图标", desc: "替换为 macOS 风格图标", action: function() { Backend.installIcons(); } },
                    { name: "安装 Win11 主题", desc: "安装 macOS 风格主题（Win11）", action: function() { Backend.installThemeWin11(); } },
                    { name: "安装 Win10 主题", desc: "安装 macOS 风格主题（Win10）", action: function() { Backend.installThemeWin10(); } },
                    { name: "安装主题路径补丁", desc: "确保主题文件正确加载", action: function() { Backend.patchThemePath(); } },
                    { name: "删除已安装主题", desc: "恢复 Windows 默认主题", action: function() { Backend.deleteTheme(); } },
                    { name: "安装 StartAllBack", desc: "Win11 开始菜单增强", action: function() { Backend.installStartAllBack(); } },
                    { name: "安装 OldNewExplorer", desc: "资源管理器增强", action: function() { Backend.installOldNewExplorer(); } },
                    { name: "执行最终设置", desc: "完成安装后的收尾配置", action: function() { Backend.finalSetting(); } },
                    { name: "重启资源管理器", desc: "使更改生效", action: function() { Backend.restartExplorer(); } }
                ]

                delegate: Rectangle {
                    width: parent.width
                    height: 60

                    Row {
                        anchors {
                            left: parent.left; leftMargin: 18
                            right: parent.right; rightMargin: 18
                            verticalCenter: parent.verticalCenter
                        }
                        spacing: 10

                        Column {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 3
                            Label {
                                text: modelData.name
                                font { pixelSize: 14; weight: Font.Medium }
                                color: "#1D1D1F"
                            }
                            Label {
                                text: modelData.desc
                                font.pixelSize: 12
                                color: "#86868B"
                            }
                        }

                        Item { height: 1; width: 1 }

                        Button {
                            id: actionBtn
                            text: "安装"
                            implicitWidth: 72
                            implicitHeight: 30
                            anchors.verticalCenter: parent.verticalCenter

                            background: Rectangle {
                                radius: 10
                                color: actionBtn.down ? "#0066CC" : (actionBtn.hovered ? "#0066CC" : "#007AFF")
                            }
                            contentItem: Label {
                                text: actionBtn.text
                                color: "#FFFFFF"
                                font { pixelSize: 12; weight: Font.Medium }
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            onClicked: {
                                actionBtn.enabled = false
                                actionBtn.text = "..."
                                Qt.callLater(function() {
                                    modelData.action()
                                    actionBtn.text = "完成"
                                    actionBtn.enabled = true
                                })
                            }
                        }
                    }
                }
            }

            Item { width: 1; height: 24 }
        }
    }
}
