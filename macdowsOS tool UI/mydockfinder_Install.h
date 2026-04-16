#pragma once
#include<bits/stdc++.h>
#include"FilesSystem.h"
#include<windows.h>
#include"LogSystem.h"
#include"All.h"
using namespace std;
bool steam_mydockfinder(wstring path) {
    MESSAGE_(L"[mydockfinder安装组件]路径", path);
    //检测文件
    wstring exe1 = path + L"/Dock64.exe";
    wstring exe2 = path + L"/Dock.exe";
    INFO_(L"[mydockfiner安装组件]检测主程序是否存在");
    if(!(fs::exists(exe1)||fs::exists(exe2))){
        return false;
    }
    INFO_(L"[mydockfiner安装组件]检测安装路径是否为steam路径");
    //检测路径
    wstring ovc_path1 = L"steamapps";
    wstring ovc_path2 = L"steam";
    int str_ = path.size();
    int str_1 = ovc_path1.size();
    int str_2 = ovc_path2.size();
    for (int j = 0; j < str_; j++) {
        bool temp1 = true, temp2 = true;
        for (int i = 0; i < str_1; i++) {
            if (path[j + i] != ovc_path1[i]) {
                temp1 = false;
               // break;
            }
        }
        for (int i = 0; i < str_2; i++) {
            if (path[j + i] != ovc_path2[i]) {
                temp2 = false;
               // break;
            }
        }
        if (temp1 || temp2) {
            INFO_(L"[mydockfinder安装组件]检测到mydockfinder为steam版");
            return true;
        }
    }

    INFO_(L"[mydockfinder安装组件]未检测到steam");
    return false;
}
//检查路径尾部是否有/mydockfinder和/MyDockFinder 是返回true 否返回false
bool pathback(wstring *path_,int str_,wstring *temp1,wstring *temp2) {
    //路径尾部去重
    for (int j = str_ - 16; j <= str_ - 13; j++) {
        bool temp3=true, temp4=true;
        for (int i = 0; i < 13; i++) {
            if (path_[j] != temp1[i]) {
                //检测到字符串1不符
                temp3 = false;
            }
        }
        for (int i = 0; i < 13; i++) {
            if (path_[j] != temp1[i]) {
                //检测到字符串2不符
                temp4 = false;
            }
        }
        if (!temp3 && !temp4) {
            return true;
        }
    }
    return false;
}
//复制mydockfinder
void copy_mydockfinder(wstring Path) {
    wstring line_copyfiles[40] = {
        L"dll",
        L"lang",
        L"launchpad",
        L"Profile",
        L"steamsettings",
        L"themes",
        L"wallpaper",
        L"weather",
        L"calfont.ttf",
        L"ColdClientLoader.ini",
        L"config.ini",
        L"dock.exe",
        L"dock.md5",
        L"Dock_64.exe",
        L"Dockmod.exe",
        L"Dockmod32.dll",
        L"Dockmod64.dll",
        L"Dockmod64.exe",
        L"Dockmod64arm.dll",
        L"Dockmod64arm.exe",
        L"holiday.xml",
        L"ico.ini",
        L"ico_bak.ini",
        L"keyboard.ini",
        L"Mydock.exe",
        L"padconfig.xml",
        L"pinyin.ini",
        L"ScreenRound.exe",
        L"steam_api.dll",
        L"steam_api64.dll",
        L"steamclient.dll",
        L"steamclient_loader.exe",
        L"steamclient64.dll",
        L"temp.exe",
        L"Temperature.exe",
        L"trayico.exe",
        L"UiAccess.exe",
        L"weather_abroad_district_id.json",
        L"weather_district_id.json",
    };
    for (int j = 0; j < 39; j++) {
        copyPath(L"AppData/MyDockFinder/MyDockFinder/" + line_copyfiles[j], Path);
        MESSAGE_(L"[mydockfinder安装组件]操作完成", line_copyfiles[j]);
    }
    return;
    
}
bool mydockfinder_install() {
    INFO_(L"[mydockfinder安装组件]运行组件");
    //文件路径
    INFO_(L"[mydockfinder安装组件]读取路径");
    MessageBox(NULL, (LPCTSTR)L"请在选择安装目录时不要选择C盘目录或无效目录", (LPCTSTR)L" mydockfinder安装组件", MB_OK);
    wstring InstallFolder_temp = BrowseFolder(L" 请选择mydockfinder安装目录（不要选C盘）");
    // 检查路径可用
    wchar_t driveLetter = towupper(InstallFolder_temp[0]);
    if (InstallFolder_temp.empty() || driveLetter == L'C') {
        ERROR_(L"[mydockfinder安装组件]目录为空或不可用 程序退出");
        MessageBox(NULL, (LPCTSTR)L"不要不选择目录或选择不可用目录 安装进程退出", (LPCTSTR)L" mydockfinder安装组件", MB_OK);
        return false;
    }
    wstring InstallPath;
    //结束mydockfinder进程
    INFO_(L"[mydockfinder安装组件]结束mydockfinder组件");
    killapp(L"trayico.exe");
    killapp(L"UiAccess.exe");
    killapp(L"Dockmod64arm.exe");
    killapp(L"Temperature.exe");
    killapp(L"temp.exe");
    killapp(L"ScreenRound.exe");
    killapp(L"Mydock.exe");
    killapp(L"Dock_64.exe");
    killapp(L"Dockmod.exe");
    killapp(L"Dockmod64.exe");
    killapp(L"dock.exe");
    killapp(L"steamclient_loader.exe");
    killapp(L"ApplicationFrameHost.exe");
    //补全文件路径保证一定有/mydockfinder
    wstring temp_path = L"/mydockfinder";
    wstring temp_path1 = L"/MyDockFinder";
    if (pathback(&InstallFolder_temp, InstallFolder_temp.size(), & temp_path, &temp_path1)) {
        InstallPath = InstallFolder_temp + L"/MyDockFinder";
    }
    else {
        InstallPath = InstallFolder_temp;
    }
    //检测安装模式
    if (steam_mydockfinder(InstallPath)) {
        INFO_(L"[mydockfinder安装组件]模式1 steam版本安装");

    }
    else {
        //两个dock图标配置文件
        wstring configfiles_1 = InstallPath + L"/ico.ini";
        wstring configfiles_2 = InstallPath + L"/ico_bak.ini";
        //检测是否存在配置文件(检测是否存在配置)
        if (fs::exists(configfiles_1) && fs::exists(configfiles_2)) {
            INFO_(L"[mydockfinder安装组件]检测到原有配置");
            wstring backupfiles = L"AppData/MyDockFinder/MyDockFinder_ConfigData";
            copyPath(configfiles_1, backupfiles);
            copyPath(configfiles_2, backupfiles);
            INFO_(L"[mydockfinder安装组件]完成备份");
            copy_mydockfinder(InstallPath);
            INFO_(L"[mydockfinder安装组件]复制文件完成 开始还原备份");
            copyPath(backupfiles + L"/ico.ini", InstallPath);
            copyPath(backupfiles + L"/ico_bak.ini", InstallPath);
        }
        else {
            INFO_(L"[mydockfinder安装组件]未检测到原有配置");
            copy_mydockfinder(InstallPath);
        }
        

    }
    INFO_(L"[mydockfinder安装组件]设置开机自启动");
    const wchar_t* APP_NAME = L"MyDockFinder";
    wstring appPath = InstallPath + L"/steamclient_loader.exe";
    AddAutoStart(appPath, APP_NAME);
    INFO_(L"[mydockfinder安装组件]退出");
	return true;
}
