pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
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
            radius: 10
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
                    background: Rectangle { color: win10Item.hovered ? "#F5F5F7" : "transparent"; radius: 10 }
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
                    background: Rectangle { color: win11Item.hovered ? "#F5F5F7" : "transparent"; radius: 0 }
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
                    background: Rectangle { color: customItem.hovered ? "#F5F5F7" : "transparent"; radius: 10 }
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

        Item { Layout.fillHeight: true }
    }
}
