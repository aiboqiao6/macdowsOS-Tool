// ============================================================
// WindowsFixPage.qml — 系统修复页面（macOS 27 风格）
// 功能：通过命令行工具（如 SFC、DISM）检查和修复 Windows 系统文件
// ============================================================

// QtQuick 模块：提供 QML 核心类型（Rectangle、Item、Label 等基础组件）
import QtQuick
// Backend 模块：C++ 后端接口，提供系统修复功能的实际调用
import Backend 1.0
// QtQuick.Controls 模块：提供标准界面控件（Button、ScrollView 等）
import QtQuick.Controls

// === 根元素：透明的矩形容器，作为页面背景 ===
Rectangle {
    id: root
    color: "transparent"  // 背景透明，由父容器控制背景色

    // === 滚动视图：内容超出可视区域时支持滚动 ===
    ScrollView {
        width: parent.width
        anchors.fill: parent
        clip: true  // 裁剪超出部分，防止内容溢出

        // 隐藏垂直滚动条（保持 macOS 简洁风格）
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOff
        }

        // === 垂直列布局：排列页面所有内容 ===
        Column {
            spacing: 14           // 子元素垂直间距 14px
            width: parent.width

            // === 页面标题："系统修复工具" ===
            Label {
                text: "系统修复工具"
                font { pixelSize: 20; weight: Font.Bold }  // 20px 加粗字体
                color: "#1D1D1F"         // 深灰色文字
                bottomPadding: 6
            }

            // === 页面说明文字：描述本页功能 ===
            Label {
                text: "使用命令行检查和修复 Windows 系统文件"  // 提示用户功能用途
                font.pixelSize: 13
                color: "#86868B"         // 浅灰色辅助文字
                width: parent.width
                bottomPadding: 10
            }

            // === 警告提示区域：红色文字提醒用户注意事项 ===
            Rectangle {
                width: parent.width
                height: 68

                Column {
                    anchors {
                        left: parent.left; leftMargin: 18
                        right: parent.right; rightMargin: 18
                        verticalCenter: parent.verticalCenter
                    }
                    // 警告标题：加粗红色
                    Label { 
                        text: "重要提示"; 
                        font.pixelSize: 14; 
                        font.weight: 
                        Font.Bold; 
                        color: "#C62828"      // 深红色，表示警示
                    }
                    // 警告内容：提醒用户可能的后果和建议
                    Label { 
                        text: "系统修复可能需要重启计算机。建议在操作前创建系统还原点。"; 
                        font.pixelSize: 12; 
                        color: "#C62828"; 
                        width: parent.width 
                    }
                }
            }

            // 额外空白间隔（保持布局整洁）
            

            // === 运行系统修复按钮区域 ===
            Rectangle {
                width: parent.width
                height: 76
                color: "#FFFFFF"     // 白色背景卡片
                radius: 14           // 圆角 14px，卡片风格

                // === "运行系统修复" 按钮 ===
                Button {
                    id: fixBtn
                    text: "运行系统修复"
                    implicitWidth: 150; implicitHeight: 30
                    // 按钮背景：蓝色圆角矩形，按下时颜色加深
                    background: Rectangle { 
                        radius: 10; 
                        color: fixBtn.down ? "#0066CC" : "#007AFF" 
                    }
                    // 按钮文字：白色居中
                    contentItem: Label { 
                        text: "运行系统修复"; 
                        color: "#FFFFFF"; 
                        font.pixelSize: 12; 
                        horizontalAlignment: Text.AlignHCenter; 
                        verticalAlignment: Text.AlignVCenter 
                    }
                    // 点击后调用 C++ 后端的 runWindowsFix() 函数执行系统修复
                    onClicked: Backend.runWindowsFix()
                }
                        
                // 此处预留空白，为后续可能增加更多修复选项做准备
                
                
            }

            // === 底部留白 ===
            
        }
    }
}
