import QtQuick
import Backend 1.0
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    ScrollView {
        anchors.fill: parent
        clip: true
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        ColumnLayout {
            width: root.width
            spacing: 18

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Label { text: "组件操作"; color: "#1D1D1F"; font { pixelSize: 28; weight: Font.DemiBold } }
                Label { text: "安装、卸载或维护美化组件。"; color: "#6E6E73"; font.pixelSize: 13 }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.maximumWidth: 660
                Layout.preferredHeight: installColumn.implicitHeight
                radius: 16
                color: "#FFFFFF"
                border.color: "#E5E5EA"
                border.width: 1

                ColumnLayout {
                    id: installColumn
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 0

                    Repeater {
                        model: [
                            { name: "安装 myDockFinder", desc: "macOS 风格 Dock 栏", action: function() { Backend.installMyDockFinder(); } },
                            { name: "安装系统图标", desc: "替换为 macOS 风格图标", action: function() { Backend.installIcons(); } },
                            { name: "恢复系统图标", desc: "从备份恢复 Windows 原始图标", action: function() { Backend.restoreIcons(); } },
                            { name: "安装 Win11 主题", desc: "安装 macOS 风格主题（Win11）", action: function() { Backend.installThemeWin11(); } },
                            { name: "安装 Win10 主题", desc: "安装 macOS 风格主题（Win10）", action: function() { Backend.installThemeWin10(); } },
                            { name: "安装主题补丁", desc: "安装 UltraUXThemePatcher 以支持第三方主题", action: function() { Backend.patchThemePath(); } },
                            { name: "安装 StartAllBack", desc: "Win11 开始菜单增强", action: function() { Backend.installStartAllBack(); } },
                            { name: "安装 OldNewExplorer", desc: "资源管理器增强", action: function() { Backend.installOldNewExplorer(); } },
                            { name: "隐藏桌面图标", desc: "隐藏 Windows 桌面图标", action: function() { Backend.setDesktopIcons(false); } },
                            { name: "重启资源管理器", desc: "使更改生效", action: function() { Backend.restartExplorer(); } }
                        ]

                        delegate: ColumnLayout {
                            id: rowRoot
                            required property var modelData
                            Layout.fillWidth: true
                            spacing: 0

                            RowLayout {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 66
                                Layout.leftMargin: 18
                                Layout.rightMargin: 18
                                spacing: 14

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 3
                                    Label {
                                        text: rowRoot.modelData.name
                                        color: "#1D1D1F"
                                        font { pixelSize: 14; weight: Font.DemiBold }
                                    }

                                    Label {
                                        text: rowRoot.modelData.desc
                                        color: "#86868B"
                                        font.pixelSize: 12
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }
                                }

                                Button {
                                    id: actionBtn
                                    text: "执行"
                                    implicitWidth: 70
                                    implicitHeight: 30
                                    background: Rectangle {
                                        radius: 11
                                        color: actionBtn.enabled ? (actionBtn.down ? "#0066CC" : actionBtn.hovered ? "#2F7AF8" : "#0A84FF") : "#C7C7CC"
                                    }
                                    contentItem: Label {
                                        text: actionBtn.text
                                        color: "#FFFFFF"
                                        font { pixelSize: 12; weight: Font.DemiBold }
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    onClicked: {
                                        actionBtn.enabled = false
                                        actionBtn.text = "处理中"
                                        Qt.callLater(function() {
                                            rowRoot.modelData.action()
                                            actionBtn.text = "完成"
                                            actionBtn.enabled = true
                                        })
                                    }
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.leftMargin: 64
                                Layout.rightMargin: 18
                                Layout.preferredHeight: 1
                                color: "#E5E5EA"
                                visible: index < 15
                            }
                        }
                    }
                }
            }

            Item { Layout.preferredHeight: 24 }
        }
    }
}
