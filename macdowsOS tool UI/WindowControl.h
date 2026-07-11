#pragma once
#include<windows.h>
#include<string>
#include<cstdlib>
#include<cwchar>
#include"LogSystem.h"
//回车
void enter_key() {
    INFO_(L"[窗口控制工具]发送回车按键");
    // 模拟按键按下 + 释放，共2个事件
    INPUT inputs[2] = {};

    // 1. 模拟回车按下：VK_RETURN是回车键虚拟键码
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_RETURN;
    inputs[0].ki.dwFlags = 0;

    // 2. 模拟回车释放
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_RETURN;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    // 发送按键事件
    SendInput(_countof(inputs), inputs, sizeof(INPUT));
}
//查找
HWND FindChildControlExact(HWND hParent, const wchar_t* targetTitle) {
    bool recursive = true;
    if (!hParent || !targetTitle) return NULL;
    // 标准化目标标题（移除&符号和多余空格）
    wchar_t normalizedTarget[256] = { 0 };
    int destIndex = 0;
    for (int i = 0; targetTitle[i] && destIndex < 255; i++) {
        // 跳过&符号（通常用于窗口快捷键标识，如"I &accept"中的&）
        if (targetTitle[i] == L'&' && targetTitle[i + 1] != L'&') continue;
        // 处理空格：合并连续空格为首个，首尾空格去除
        if (iswspace(targetTitle[i])) {
            if (destIndex > 0 && !iswspace(normalizedTarget[destIndex - 1])) {
                normalizedTarget[destIndex++] = L' ';
            }
            continue;
        }
        normalizedTarget[destIndex++] = targetTitle[i];
    }
    normalizedTarget[destIndex] = L'\0';
    // 遍历当前父窗口的所有子控件
    HWND hChild = GetWindow(hParent, GW_CHILD);
    while (hChild) {
        // 获取子窗口标题
        wchar_t childText[256] = { 0 };
        GetWindowTextW(hChild, childText, 256);
        // 标准化子窗口标题（同目标标题处理逻辑）
        wchar_t normalizedChild[256] = { 0 };
        int childIndex = 0;
        for (int i = 0; childText[i] && childIndex < 255; i++) {
            if (childText[i] == L'&' && childText[i + 1] != L'&') continue;
            if (iswspace(childText[i])) {
                if (childIndex > 0 && !iswspace(normalizedChild[childIndex - 1])) {
                    normalizedChild[childIndex++] = L' ';
                }
                continue;
            }
            normalizedChild[childIndex++] = childText[i];
        }
        normalizedChild[childIndex] = L'\0';
        // 精确匹配则返回控件句柄
        if (wcscmp(normalizedChild, normalizedTarget) == 0) return hChild;
        // 递归查找子窗口的子控件（深度优先）
        if (recursive) {
            HWND hFound = FindChildControlExact(hChild, targetTitle);
            if (hFound) return hFound;
        }
        // 继续查找下一个兄弟控件
        hChild = GetWindow(hChild, GW_HWNDNEXT);
    }
    return NULL;  // 未找到目标控件
}
//模拟鼠标真实点击
void RealisticClick(HWND hwnd) {
    if (!hwnd) return;

    // 置顶窗口
    SetForegroundWindow(GetAncestor(hwnd, GA_ROOT));
    // 获取控件位置和大小
    RECT rc;
    GetWindowRect(hwnd, &rc);
    int x = (rc.left + rc.right) / 2;  // 控件中心x坐标
    int y = (rc.top + rc.bottom) / 2;  // 控件中心y坐标
    MESSAGE_(L"[macdowsOS Tool窗口控制工具]控件x坐标", x);
    MESSAGE_(L"[macdowsOS Tool窗口控制工具]控件y坐标", y);

    // 保存原始鼠标位置，完成后恢复
    POINT origPos;
    GetCursorPos(&origPos);
    SetCursorPos(x, y);  // 移动鼠标到控件中心
    // 准备鼠标点击事件
    INPUT inputs[4] = {};
    // 1. 移动鼠标到目标位置
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = static_cast<LONG>(x * (65535.0 / GetSystemMetrics(SM_CXSCREEN)));  // 转换为0-65535范围
    inputs[0].mi.dy = static_cast<LONG>(y * (65535.0 / GetSystemMetrics(SM_CYSCREEN)));
    inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    // 2. 鼠标按下
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // 3. 鼠标释放（延迟50ms模拟真实点击）
    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    inputs[2].mi.time = 50;

    // 4. 恢复鼠标到原始位置
    inputs[3].type = INPUT_MOUSE;
    inputs[3].mi.dx = static_cast<LONG>(origPos.x * (65535.0 / GetSystemMetrics(SM_CXSCREEN)));
    inputs[3].mi.dy = static_cast<LONG>(origPos.y * (65535.0 / GetSystemMetrics(SM_CYSCREEN)));
    inputs[3].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    // 发送所有鼠标事件
    SendInput(4, inputs, sizeof(INPUT));
}
//获取最后一个子窗口ID
HWND GetLastChildWindow(HWND hParent) {
    HWND hChild = GetWindow(hParent, GW_CHILD);
    if (!hChild) return NULL;

    HWND hLast = hChild;
    // 遍历找到最后一个子窗口
    while ((hChild = GetWindow(hChild, GW_HWNDNEXT)) != NULL) {
        hLast = hChild;
    }
    return hLast;
}

// ==================== 内部窗口查找工具 ====================
namespace {
    HWND findwindow(HWND hwnd_window, std::wstring pack_name, std::wstring button_name) {
        SYSTEMTIME now;
        GetLocalTime(&now);
        INFO_(L"[" + pack_name + L"]" + L"开始查找" + button_name);
        const wchar_t* button_ = button_name.data();
        HWND temp = FindChildControlExact(hwnd_window, button_);
        while (temp == NULL) {
            SYSTEMTIME now_temp;
            GetLocalTime(&now_temp);
            if (now_temp.wSecond - now.wSecond >= 20) {
                ERROR_(L"[" + pack_name + L"]" + L"超时未找到" + button_name);
                MessageBox(NULL, (LPCTSTR)L" 未找到按钮，窗口可能没有出现，请重试", (LPCTSTR)L" macdowsOS tool 窗口控制工具", MB_OK);
            }
            temp = FindChildControlExact(hwnd_window, button_);
        }
        INFO_(L"[" + pack_name + L"]" + L"成功找到" + button_name);
        return temp;
    }
}
HWND FindWindow_(std::wstring title, std::wstring pack_name) {
    //计数器
    SYSTEMTIME now;
    GetLocalTime(&now);
    //
    LPCTSTR temp = title.c_str();
    //
    HWND hWnd_Window = FindWindowW(NULL, temp);
    while (hWnd_Window == NULL) {
        SYSTEMTIME now_temp;
        GetLocalTime(&now_temp);
        if (now_temp.wSecond - now.wSecond >= 20) {
            ERROR_(L"[" + pack_name + L"]" + L"超时未找到" + title);
            MessageBox(NULL, (LPCTSTR)L" 无法查找到窗口 请检查组件完整性或将log.txt发送给开发者获取帮助", (LPCTSTR)L" macdowsOS tool 窗口控制工具", MB_OK);
        }
        hWnd_Window = FindWindowW(NULL, temp);
    }
    return hWnd_Window;
}
// ==================== 按钮点击操作 ====================
void ClickButtonMode1(HWND hwnd_window, std::wstring pack_name, std::wstring button_name) {
    HWND temp = findwindow(hwnd_window, pack_name, button_name);
    SetForegroundWindow(hwnd_window);
    SendMessageW(temp, BM_CLICK, 0, 0);
    return;
}
//模拟点击
void ClickButtonMode2(HWND hwnd_window, std::wstring pack_name, std::wstring button_name) {
    HWND temp = findwindow(hwnd_window, pack_name, button_name);
    SetForegroundWindow(hwnd_window);
    RealisticClick(temp);
    return;
}
//回车点击
void ClickButtonMode3(HWND hwnd_window, std::wstring pack_name, std::wstring button_name) {
    HWND temp = findwindow(hwnd_window, pack_name, button_name);
    SetForegroundWindow(hwnd_window);
    enter_key();
    return;
}
// ==================== 开关控件 ====================
void TurnOffOn(bool OffOn, HWND hwnd_window, std::wstring pack_name, std::wstring button_name) {
    HWND temp = findwindow(hwnd_window, pack_name, button_name);
    SetForegroundWindow(hwnd_window);
    if ((SendMessageW(temp, BM_GETCHECK, 0, 0) == BST_CHECKED) != OffOn) {
        SendMessageW(GetParent(temp), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(temp), BN_CLICKED), (LPARAM)temp);
    }
    return;
}
