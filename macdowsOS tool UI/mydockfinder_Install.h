#pragma once
#include <string>
#include <cwctype>
#include"FilesSystem.h"
#include<windows.h>
#include"LogSystem.h"
#include"All.h"
bool steam_mydockfinder(std::wstring path) {
    MESSAGE_(L"[mydockfinder安装器]路径", path);
    //忽略文件
    std::wstring exe1 = path + L"/Dock64.exe";
    std::wstring exe2 = path + L"/Dock.exe";
    INFO_(L"[mydockfiner安装器]检查dock程序是否存在");
    if(!(fs::exists(exe1)||fs::exists(exe2))){
        return false;
    }
    INFO_(L"[mydockfiner安装器]检测安装路径是否为steam路径");
    //比较路径
    std::wstring ovc_path1 = L"steamapps";
    std::wstring ovc_path2 = L"steam";
    size_t str_ = path.size();
    size_t str_1 = ovc_path1.size();
    size_t str_2 = ovc_path2.size();
    for (size_t j = 0; j < str_; j++) {
        bool temp1 = true, temp2 = true;
        for (size_t i = 0; i < str_1; i++) {
            if (path[j + i] != ovc_path1[i]) {
                temp1 = false;
               // break;
            }
        }
        for (size_t i = 0; i < str_2; i++) {
            if (path[j + i] != ovc_path2[i]) {
                temp2 = false;
               // break;
            }
        }
        if (temp1 || temp2) {
            INFO_(L"[mydockfinder安装器]检测到mydockfinder为steam版");
            return true;
        }
    }

    INFO_(L"[mydockfinder安装器]未检测到steam");
    return false;
}
//比较路径尾部是否为mydockfinder/MyDockFinder 是返回true 否则返回false
bool pathback(std::wstring *path_,size_t str_,std::wstring *temp1,std::wstring *temp2) {
    //路径尾部去除
    if (str_ < 16) return false;
    for (size_t j = str_ - 16; j <= str_ - 13; j++) {
        bool temp3=true, temp4=true;
        for (int i = 0; i < 13; i++) {
            if (path_[j] != temp1[i]) {
                //检测到字符串不同
                temp3 = false;
            }
        }
        for (int i = 0; i < 13; i++) {
            if (path_[j] != temp1[i]) {
                //检测到字符串不同
                temp4 = false;
            }
        }
        if (!temp3 && !temp4) {
            return true;
        }
    }
    return false;
}
//安装mydockfinder
void copy_mydockfinder(std::wstring Path) {
    std::wstring line_copyfiles[40] = {
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
        MESSAGE_(L"[mydockfinder安装器]已安装", line_copyfiles[j]);
    }
    return;
    
}
inline bool mydockfinder_install() {
    INFO_(L"[mydockfinder安装器]开始安装");
    //文件路径
    INFO_(L"[mydockfinder安装器]获取路径");
    MessageBox(NULL, (LPCTSTR)L"请选择安装目录，不要选择C盘目录，无效目录", (LPCTSTR)L" mydockfinder安装工具", MB_OK);
    std::wstring InstallFolder_temp = BrowseFolder(L" 请选择mydockfinder安装目录，不要选C盘");
    // 检查路径合法性
    wchar_t driveLetter = towupper(InstallFolder_temp[0]);
    if (InstallFolder_temp.empty() || driveLetter == L'C') {
        ERROR_(L"[mydockfinder安装器]目录为空或不可用 程序退出");
        MessageBox(NULL, (LPCTSTR)L"请重新选择目录，选择不可用目录 安装程序退出", (LPCTSTR)L" mydockfinder安装工具", MB_OK);
        return false;
    }
    std::wstring InstallPath;
    //关闭mydockfinder相关
    INFO_(L"[mydockfinder安装器]关闭mydockfinder相关");
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
    //检查文件路径最后是否有/mydockfinder
    std::wstring temp_path = L"/mydockfinder";
    std::wstring temp_path1 = L"/MyDockFinder";
    if (pathback(&InstallFolder_temp, InstallFolder_temp.size(), & temp_path, &temp_path1)) {
        InstallPath = InstallFolder_temp + L"/MyDockFinder";
    }
    else {
        InstallPath = InstallFolder_temp;
    }
    //检测安装模式
    if (steam_mydockfinder(InstallPath)) {
        INFO_(L"[mydockfinder安装器]模式1 steam版本安装");

    }
    else {
        //备份dock图标配置文件
        std::wstring configfiles_1 = InstallPath + L"/ico.ini";
        std::wstring configfiles_2 = InstallPath + L"/ico_bak.ini";
        //检测是否存在配置文件（检测是否已安装）
        if (fs::exists(configfiles_1) && fs::exists(configfiles_2)) {
            INFO_(L"[mydockfinder安装器]检测到原有配置");
            std::wstring backupfiles = L"AppData/MyDockFinder/MyDockFinder_ConfigData";
            copyPath(configfiles_1, backupfiles);
            copyPath(configfiles_2, backupfiles);
            INFO_(L"[mydockfinder安装器]完成备份");
            copy_mydockfinder(InstallPath);
            INFO_(L"[mydockfinder安装器]文件复制完成 开始恢复配置");
            copyPath(backupfiles + L"/ico.ini", InstallPath);
            copyPath(backupfiles + L"/ico_bak.ini", InstallPath);
        }
        else {
            INFO_(L"[mydockfinder安装器]未检测到原有配置");
            copy_mydockfinder(InstallPath);
        }
        

    }
    INFO_(L"[mydockfinder安装器]设置开机自启动");
    const wchar_t* APP_NAME = L"MyDockFinder";
    std::wstring appPath = InstallPath + L"/steamclient_loader.exe";
    AddAutoStart(appPath, APP_NAME);
    INFO_(L"[mydockfinder安装器]退出");
	return true;
}
