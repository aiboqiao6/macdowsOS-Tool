// ============================================================
// WindowsFixPage.qml — 系统修复页面（macOS 27 风格）
// ============================================================
import QtQuick
import Backend 1.0
import QtQuick.Controls

Rectangle {
    id: root
    color: "transparent"

    ScrollView {
        width: parent.width
        anchors.fill: parent
        clip: true

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOff
        }

        Column {
            spacing: 14
            width: parent.width

            Label {
                text: "系统修复工具"
                font { pixelSize: 20; weight: Font.Bold }
                color: "#1D1D1F"
                bottomPadding: 6
            }

            Label {
                text: "使用命令行检查和修复 Windows 系统文件"
                font.pixelSize: 13
                color: "#86868B"
                width: parent.width
                bottomPadding: 10
            }

            // 警告提示
            Rectangle {
                width: parent.width
                height: 68

                Column {
                    anchors {
                        left: parent.left; leftMargin: 18
                        right: parent.right; rightMargin: 18
                        verticalCenter: parent.verticalCenter
                    }
                    Label { 
                        text: "重要提示"; 
                        font.pixelSize: 14; 
                        font.weight: 
                        Font.Bold; 
                        color: "#C62828" 
                    }
                    Label { 
                        text: "系统修复可能需要重启计算机。建议在操作前创建系统还原点。"; 
                        font.pixelSize: 12; 
                        color: "#C62828"; 
                        width: parent.width 
                    }
                }
            }

            

            // 运行系统修复
            Rectangle {
                width: parent.width
                height: 76
                color: "#FFFFFF"
                radius: 14
 
                Button {
                    text: "运行系统修复"
                    implicitWidth: 150; implicitHeight: 30
                    background: Rectangle { 
                        radius: 10; 
                        color: parent.down ? "#0066CC" : "#007AFF" 
                    }
                    contentItem: Label { 
                        text: "运行系统修复"; 
                        color: "#FFFFFF"; 
                        font.pixelSize: 12; 
                        horizontalAlignment: Text.AlignHCenter; 
                        verticalAlignment: Text.AlignVCenter 
                    }
                    onClicked: Backend.runWindowsFix()
                }
                        
                
                
            }

            
        }
    }
}
