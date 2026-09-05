#pragma once
#include"FilesSystem.h"
#include"WindowControl.h"
#include"All.h"
#include<Windows.h>
#include <string>
namespace {
    // 直接启动程序（替代键盘模拟+运行对话框方案，解决 CFG/UIPI 兼容性问题）
    bool ExecuteInRunDialog(const std::wstring& command) {
        INFO_(L"[OldNewExplorer卸载工具]直接启动程序");

        // 提取程序所在目录作为工作目录
        std::wstring workingDir;
        size_t pos = command.find_last_of(L'\\');
        if (pos != std::wstring::npos) {
            workingDir = command.substr(0, pos);
        }

        // 通过 ShellExecuteW 启动（与运行对话框内部行为一致）
        HINSTANCE result = ShellExecuteW(NULL, L"open", command.c_str(), NULL,
            workingDir.c_str(), SW_SHOWNORMAL);

        // ShellExecuteW 返回值 <= 32 表示启动失败
        if ((INT_PTR)result <= 32) {
            ERROR_(L"[OldNewExplorer卸载工具]直接启动失败");
            return false;
        }

        INFO_(L"[OldNewExplorer卸载工具]程序已启动");
        return true;
    }

    void OldNewExplorer_Delete() {
        INFO_(L"[OldNewExplorer卸载工具]检查文件是否存在");
        if (!fs::exists(L"C:\\OldNewExplorer\\OldNewExplorerCfg.exe")) {
            ERROR_(L"[OldNewExplorer卸载工具]未找到OldNewExplorer主程序 OldNewExplorer卸载进程退出");
            return;
        }
        INFO_(L"[OldNewExplorer卸载工具]打开配置程序");
        ExecuteInRunDialog(L"C:\\OldNewExplorer\\OldNewExplorerCfg.exe");
        INFO_(L"[OldNewExplorer卸载工具]等待配置窗口出现");
        HWND hWnd_Window = FindWindow_(L"致美化 OldNewExplorer 配置", L"OldNewExplorer卸载工具");

        INFO_(L"[OldNewExplorer卸载工具]配置窗口出现");
        SetForegroundWindow(hWnd_Window);
        SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);

        INFO_(L"[OldNewExplorer卸载工具]开始配置设置");
        TurnOffOn(false, hWnd_Window, L"OldNewExplorer卸载工具", L"使用经典驱动器分组");
        TurnOffOn(false, hWnd_Window, L"OldNewExplorer卸载工具", L"使用库、并隐藏顶部文件夹");

        TurnOffOn(false, hWnd_Window, L"OldNewExplorer卸载工具", L"使用工具栏代替功能区（必选）");

        TurnOffOn(false, hWnd_Window, L"OldNewExplorer卸载工具", L"隐藏标题栏小图标（可选）");
        TurnOffOn(false, hWnd_Window, L"OldNewExplorer卸载工具", L"隐藏标题栏文字（可选）");
        TurnOffOn(false, hWnd_Window, L"OldNewExplorer卸载工具", L"隐藏返回上级目录按钮（必选）");
        TurnOffOn(false, hWnd_Window, L"OldNewExplorer卸载工具", L"开启导航栏特效（部分主题需要勾选）");
        TurnOffOn(false, hWnd_Window, L"OldNewExplorer卸载工具", L"不要勾选我");

        TurnOffOn(true, hWnd_Window, L"OldNewExplorer卸载工具", L"显示底部状态栏（不用勾选）");
        TurnOffOn(true, hWnd_Window, L"OldNewExplorer卸载工具", L"显示底部详细信息窗格（大部分情况下都不用勾选）");

        ClickButtonMode1(hWnd_Window, L"OldNewExplorer卸载工具", L"卸载");

        INFO_(L"[OldNewExplorer卸载工具]配置完成关闭程序");
        killapp(L"OldNewExplorerCfg.exe");
        INFO_(L"[OldNewExplorer卸载工具]删除文件");
        TakeOwnershipAndGrantFullControl(L"C:\\OldNewExplorer\\OldNewExplorerCfg.exe");
        std::filesystem::remove_all(L"C:\\OldNewExplorer");
        
        INFO_(L"[OldNewExplorer卸载工具]配置结束 退出");
        return;
    }

}
