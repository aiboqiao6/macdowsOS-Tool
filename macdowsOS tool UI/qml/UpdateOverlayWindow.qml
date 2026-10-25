// ============================================================
// UpdateOverlayWindow.qml — 仿 macOS 系统更新全屏遮挡窗口
//
// 功能说明：
//   自动化安装/卸载流程执行期间显示的全屏置顶窗口，
//   样式对照 macOS Tahoe 26 系统更新安装界面：
//     纯黑背景 + 居中白色苹果标 + 细圆角进度条 + 底部"剩余约 X 分钟"
//   遮挡期间所有第三方安装器窗口被完全覆盖，鼠标点击被本窗口吞掉。
//   对外接口：
//     begin(totalSteps)                       — 开始显示并初始化进度
//     updateProgress(stepName, done, total)   — 更新进度（驱动剩余时间估算）
//     finish(success)                         — 成功则保持显示至重启；
//                                               失败则隐藏以展示错误弹窗
// ============================================================
import QtQuick
import QtQuick.Window

Window {
    id: overlayWindow
    title: "macOS 更新"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    color: "transparent"
    visible: false

    // 覆盖主显示器全屏
    x: Screen.virtualX
    y: Screen.virtualY
    width: Screen.width
    height: Screen.height

    // 内部状态
    property int totalSteps: 0
    property int completedSteps: 0
    property real emaStepSeconds: 0          // 单步耗时指数移动平均
    property bool firstStepPending: true     // 首步未完成前显示"正在准备更新…"
    property var lastStepTime: null
    property bool restarting: false
    property real displayRatio: 0            // 实际渲染的进度比例（平滑跟随真实进度）
    property bool fillAnimated: false        // 进度条动画开关：归零重置期间必须关闭

    // 真实步进之间的"蠕动"推进：进度条永不静止，但绝不越过真实进度太多
    Timer {
        id: creepTimer
        interval: 1800
        repeat: true
        running: overlayWindow.visible && !overlayWindow.restarting && overlayWindow.totalSteps > 0
        onTriggered: {
            const target = overlayWindow.totalSteps > 0
                ? Math.min(overlayWindow.completedSteps / overlayWindow.totalSteps, 1) : 0
            const cap = Math.min(target + 0.04, 0.95)
            if (overlayWindow.displayRatio < cap) {
                const inc = 0.002 + Math.random() * 0.006
                overlayWindow.displayRatio = Math.min(overlayWindow.displayRatio + inc, cap)
            }
        }
    }

    // displayRatio 统一走平滑动画（OutCubic 缓动，模拟 macOS 非线性推进）
    Behavior on displayRatio {
        enabled: overlayWindow.fillAnimated
        NumberAnimation {
            duration: 1200
            easing.type: Easing.OutCubic
        }
    }

    function begin(totalSteps_) {
        totalSteps = totalSteps_
        completedSteps = 0
        emaStepSeconds = 0
        firstStepPending = true
        restarting = false
        lastStepTime = new Date()
        timeLabel.text = "正在准备更新…"
        // 窗口展示给用户之前必须完成归零：关动画 → 瞬间置 0 → 再显示窗口
        fillAnimated = false
        displayRatio = 0
        visible = true
        requestActivate()
    }

    function updateProgress(stepName, completed, total) {
        completedSteps = completed
        totalSteps = total
        fillAnimated = true
        if (totalSteps > 0) {
            const target = Math.min(completedSteps / totalSteps, 1)
            if (target > displayRatio)
                displayRatio = target     // 只进不退，平滑动画逼近真实进度
        }
        if (firstStepPending) {
            firstStepPending = false
        } else if (lastStepTime !== null) {
            const stepSeconds = (new Date() - lastStepTime) / 1000
            emaStepSeconds = (emaStepSeconds === 0)
                ? stepSeconds
                : emaStepSeconds * 0.6 + stepSeconds * 0.4
        }
        lastStepTime = new Date()
        updateTimeEstimate()
    }

    function finish(success) {
        if (success) {
            restarting = true
            displayRatio = 1              // 收尾：进度条平滑充满
            timeLabel.text = "正在准备重新启动…"
        } else {
            // 失败：隐藏遮挡层，让错误弹窗可见
            visible = false
        }
    }

    function updateTimeEstimate() {
        if (restarting)
            return
        const remainingSteps = Math.max(totalSteps - completedSteps, 0)
        if (emaStepSeconds <= 0 || remainingSteps === 0) {
            timeLabel.text = "剩余约 1 分钟"
            return
        }
        let minutes = Math.ceil(emaStepSeconds * remainingSteps / 60)
        if (minutes < 1) minutes = 1
        if (minutes > 99) minutes = 99
        timeLabel.text = "剩余约 " + minutes + " 分钟"
    }

    Rectangle {
        anchors.fill: parent
        color: "#000000"

        // 居中组：苹果标 + 进度条
        Column {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -parent.height * 0.02
            spacing: overlayWindow.height * 0.05

            Image {
                id: appleLogo
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/res/apple.svg"
                sourceSize.height: overlayWindow.height * 0.085
                fillMode: Image.PreserveAspectFit
                mipmap: true
            }

            // 细圆角进度条（macOS 原生比例：宽约屏宽 14%，高 5px 胶囊形）
            Rectangle {
                id: barTrack
                anchors.horizontalCenter: parent.horizontalCenter
                width: Math.min(overlayWindow.width * 0.14, 260)
                height: 5
                radius: height / 2
                color: "#38FFFFFF"   // 白色 22% 轨道

                Rectangle {
                    id: progressFill
                    width: barTrack.width * overlayWindow.displayRatio
                    height: parent.height
                    radius: parent.radius
                    color: "#FFFFFF"
                }
            }
        }

        // 底部预估时间（距底部约 12%，小号萍方字体）
        Text {
            id: timeLabel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height * 0.12
            text: "正在准备更新…"
            color: "#FFFFFF"
            opacity: 0.85
            font.pixelSize: 13
            font.family: "PingFang SC0"   // pingfang0.ttf 内部注册名（带尾缀 0）
        }

        // 全屏吞掉所有鼠标事件，防止穿透到下面的安装器窗口
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: function(mouse) { mouse.accepted = true }
            onWheel: function(wheel) { wheel.accepted = true }
        }
    }

    onClosing: function(close) { close.accepted = false }
}
