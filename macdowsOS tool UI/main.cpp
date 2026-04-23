#pragma once
#include<bits/stdc++.h>
#include "MainWindow.h"
#include"LogSystem.h"
#include"WindowConfig.h"
#include"WindowButtonChange.h"
//https://gitcode.com/aiboqiao/sendmessage/releases/download/12/AppData.zip

//int main(){
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //控制台格式
    static bool initialized = false;
    if (!initialized) {
        std::wcout.imbue(std::locale("chs"));
        setlocale(LC_ALL, "chs");
        initialized = true;
    }
    
    buttonchangemain();
    /**
    ChuiZhiTongBu = true;
    Window_xx = 2;
    open_debug_window = false;
    freopen("log.txt", "w", stdout);
    INFO_(L"程序启动");
    INFO_(L"调用声明组件");
    shenm();
    INFO_(L"调用主窗口");
    home_window_();*/
    return 0;
}