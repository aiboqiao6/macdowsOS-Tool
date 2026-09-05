#include <filesystem>
#include <cstdlib>
#include <string>
#include "LogSystem.h"
#include "WindowControl.h"

namespace {

    void UninstallStartAllBack() {
        namespace fs = std::filesystem;

        std::wstring path =L"C:\\Program Files\\StartAllBack\\StartAllBackCfg.exe";

        if (!fs::exists(path)) {
            const wchar_t* localAppData = _wgetenv(L"LOCALAPPDATA");

            if (localAppData) {
                path = std::wstring(localAppData) + L"\\StartAllBack\\StartAllBackCfg.exe";
            }
        }

        if (!fs::exists(path)) {
            ERROR_(L"[StartAllBack卸载器]未找到卸载程序");
            return;
        }

        INFO_(L"[StartAllBack卸载器]开始卸载");

        // 路径加双引号，防止空格导致路径截断
        std::wstring command =  L"\"" + path + L"\" /uninstall";

        int result = _wsystem(command.c_str());

        if (result == 0) {
            INFO_(L"[StartAllBack卸载器]卸载程序启动成功");
        }
        else {
            ERROR_(L"[StartAllBack卸载器]卸载程序启动失败");
        }

        INFO_(L"[StartAllBack卸载器]等待应用程序");
        HWND hWnd_Window = FindWindow_(L"卸载", L"StartAllBack卸载器");

        INFO_(L"[StartAllBack卸载器]已找到窗口");
        //前置
        SetForegroundWindow(hWnd_Window);
        SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);

        ClickButtonMode1(hWnd_Window, L"StartAllBack卸载器", L"是(&Y)");

        return;
    }

}
