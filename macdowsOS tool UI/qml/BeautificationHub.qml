pragma ComponentBehavior: Bound

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

            Label { text: "系统美化"; color: "#1D1D1F"; font { pixelSize: 28; weight: Font.DemiBold } }
            Label { text: "选择完整美化方案，或单独安装某个组件。"; color: "#6E6E73"; font.pixelSize: 13 }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.maximumWidth: 620
            Layout.preferredHeight: 226
            radius: 16
            color: "#FFFFFF"
            border.color: "#E5E5EA"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                ItemDelegate {
                    id: win10Item
                    Layout.fillWidth: true
                    Layout.preferredHeight: 74
                    hoverEnabled: true
                    background: Rectangle { color: win10Item.hovered ? "#F0F0F5" : "transparent"; radius: 14 }
                    contentItem: RowLayout {
                        spacing: 14
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 3

                            Label {
                                text: "Win10 系统美化"
                                color: "#1D1D1F"
                                font { pixelSize: 14; weight: Font.DemiBold }
                            }

                            Label {
                                text: "为 Windows 10 安装完整 macOS 外观。"
                                color: "#86868B"
                                font.pixelSize: 12
                            }
                        }
                        Label { text: "›"; color: "#A1A1A6"; font.pixelSize: 24 }
                    }
                    onClicked: appWindow.openThemeWizard(10)
                }

                Rectangle { Layout.fillWidth: true; Layout.leftMargin: 56; Layout.preferredHeight: 1; color: "#E5E5EA" }

                ItemDelegate {
                    id: win11Item
                    Layout.fillWidth: true
                    Layout.preferredHeight: 74
                    hoverEnabled: true
                    background: Rectangle { color: win11Item.hovered ? "#F0F0F5" : "transparent"; radius: 14 }
                    contentItem: RowLayout {
                        spacing: 14
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 3

                            Label {
                                text: "Win11 系统美化"
                                color: "#1D1D1F"
                                font { pixelSize: 14; weight: Font.DemiBold }
                            }

                            Label {
                                text: "为 Windows 11 安装完整 macOS 外观。"
                                color: "#86868B"
                                font.pixelSize: 12
                            }
                        }
                        Label { text: "›"; color: "#A1A1A6"; font.pixelSize: 24 }
                    }
                    onClicked: appWindow.openThemeWizard(11)
                }

                Rectangle { Layout.fillWidth: true; Layout.leftMargin: 56; Layout.preferredHeight: 1; color: "#E5E5EA" }

                ItemDelegate {
                    id: customItem
                    Layout.fillWidth: true
                    Layout.preferredHeight: 76
                    hoverEnabled: true
                    background: Rectangle { color: customItem.hovered ? "#F0F0F5" : "transparent"; radius: 14 }
                    contentItem: RowLayout {
                        spacing: 14
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 3

                            Label {
                                text: "选择安装"
                                color: "#1D1D1F"
                                font { pixelSize: 14; weight: Font.DemiBold }
                            }

                            Label {
                                text: "单独安装 Dock、图标、主题等组件。"
                                color: "#86868B"
                                font.pixelSize: 12
                            }
                        }
                        Label { text: "›"; color: "#A1A1A6"; font.pixelSize: 24 }
                    }
                    onClicked: contentStack.currentIndex = 2
                }
            }
        }

        Label {
            text: "卸载组件"
            color: "#1D1D1F"
            font { pixelSize: 18; weight: Font.DemiBold }
            Layout.topMargin: 8
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.maximumWidth: 620
            Layout.preferredHeight: 8 * 58
            radius: 16
            color: "#FFFFFF"
            border.color: "#E5E5EA"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Repeater {
                    model: [
                        { name: "卸载 Windows 10 组件", desc: "卸载 Dock、主题、补丁、图标和 OldNewExplorer", action: function() { Backend.uninstallWindows10(); } },
                        { name: "卸载 Windows 11 组件", desc: "卸载 Dock、主题、补丁、图标和 StartAllBack", action: function() { Backend.uninstallWindows11(); } },
                        { name: "删除 MyDockFinder", desc: "删除 Dock、启动项和锁屏壁纸设置", action: function() { Backend.deleteMyDockFinder(); } },
                        { name: "删除主题补丁", desc: "卸载 UltraUXThemePatcher", action: function() { Backend.uninstallThemePatch(); } },
                        { name: "删除已安装主题", desc: "恢复 Windows 默认主题文件", action: function() { Backend.deleteTheme(); } },
                        { name: "卸载 StartAllBack", desc: "卸载 Win11 开始菜单增强", action: function() { Backend.uninstallStartAllBack(); } },
                        { name: "卸载 OldNewExplorer", desc: "恢复资源管理器默认设置", action: function() { Backend.uninstallOldNewExplorer(); } },
                        { name: "显示桌面图标", desc: "恢复 Windows 桌面图标显示", action: function() { Backend.setDesktopIcons(true); } }
                    ]

                    delegate: ItemDelegate {
                        required property var modelData
                        Layout.fillWidth: true
                        Layout.preferredHeight: 58
                        hoverEnabled: true
                        background: Rectangle {
                            color: parent.hovered ? "#F5F5F7" : "transparent"
                        }
                        contentItem: ColumnLayout {
                            spacing: 2
                            Label {
                                text: modelData.name
                                color: "#1D1D1F"
                                font { pixelSize: 14; weight: Font.DemiBold }
                            }
                            Label {
                                text: modelData.desc
                                color: "#86868B"
                                font.pixelSize: 12
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                        }
                        onClicked: modelData.action()
                    }
                }
            }
        }

            Item { Layout.preferredHeight: 24 }
        }
    }
}
