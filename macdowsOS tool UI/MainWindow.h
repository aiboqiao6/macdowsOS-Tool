#pragma once
//UI
//c++
#include<bits/stdc++.h>
#include<Windows.h>
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
//winAPI
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
//macdowsOS tool
#include"WinToMacWin10.h"
#include"WinToMacWin11.h"
#include"WindowsFix.h"
#include"SettingWindow.h"
#include"WindowRoundWindow.h"
#include"WindowRound.h"
#include"WindowConfig.h"
#include"CeleCraft.h"
//log
#include"LogSystem.h"
// 需要链接 NtDll.lib
#pragma comment(lib, "NtDll.lib")
//每日一言
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#pragma comment(lib, "dwmapi.lib")
using namespace std;
//测试
HWND hwnd_main;
//声明界面
void shenm() {
	MessageBox(NULL, (LPCTSTR)L" 用户应自行确保所使用的所有组件/工具/软件均符合版权法规。开发者不对用户使用或获得的任何文件的合法性/版权纠纷负责,本程序仅用于学习研究Windows程序操控原理及c++基础知识,禁止用于其他任何用途(包括但不限于商业侵权用途)。请于获得此程序的24小时内彻底删除本程序。使用macdowsOS Tool即表示用户同意以上内容且同意自行承担相关风险。", (LPCTSTR)L" macdowsOS tool 免责声明", MB_OK | MB_ICONINFORMATION);
	MessageBox(NULL, (LPCTSTR)L" 此工具完全免费 如果你是付费获得 说明你被骗了", (LPCTSTR)L" macdowsOS tool 反诈中心", MB_OK);
	return;
}

//主界面
wstring GetHitokoto() {
	INFO_(L"[主界面]一言API获取文本");
	HINTERNET hSession = WinHttpOpen(L"Hitokoto", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	HINTERNET hConnect = WinHttpConnect(hSession, L"v1.hitokoto.cn", INTERNET_DEFAULT_HTTPS_PORT, 0);
	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	WinHttpReceiveResponse(hRequest, NULL);
	// 读取响应
	string response;
	char buffer[4096] = "";
	DWORD downloaded;
	while (WinHttpReadData(hRequest, buffer, sizeof(buffer) - 1, &downloaded) && downloaded > 0) {
		buffer[downloaded] = '\0';
		response.append(buffer);
	}
	// 关闭句柄
	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
	// 提取内容和出处
	size_t contentStart = response.find("\"hitokoto\":\"");
	size_t fromStart = response.find("\"from\":\"");
	if (contentStart == string::npos) return L"";
	contentStart += 12;
	size_t contentEnd = response.find("\"", contentStart);
	string content = response.substr(contentStart, contentEnd - contentStart);
	string from = "";
	if (fromStart != string::npos) {
		fromStart += 8;
		size_t fromEnd = response.find("\"", fromStart);
		from = response.substr(fromStart, fromEnd - fromStart);
	}
	// 正确转换UTF-8到UTF-16
	auto utf8ToWide = [](const string& utf8) -> wstring {
		if (utf8.empty()) return L"";
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
		wstring wide(len, 0);
		MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wide[0], len);
		wide.pop_back(); // 移除末尾的null字符
		return wide;
	};
	wstring result;
	if (!from.empty()) {
		result = utf8ToWide(content) + L"\n\n—— " + utf8ToWide(from);
	} else {
		result = utf8ToWide(content);
	}
	INFO_(L"[主界面]一言获取文本结束");
	return result;
}
//windows版本

// 从注册表中读取 Windows 版本信息，返回 wstring
void GetWindowsVersionInfo(wstring& productName, wstring& displayVersion, wstring& currentBuild) {
	INFO_(L"[主界面]获取Windows版本");
	HKEY hKey;
	DWORD dwType, dwSize = 512;
	wchar_t buffer[512]=L" ";

	// 打开注册表键
	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		0, KEY_READ, &hKey) == ERROR_SUCCESS) {

		// 读取产品名称
		dwSize = sizeof(buffer);
		if (RegQueryValueExW(hKey, L"ProductName", NULL, &dwType,
			(LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
			productName = wstring(buffer);
		}
		else {
			productName = L"Unknown";
		}

		// 读取显示版本 (Windows 10/11 的版本号如 21H2, 22H2 等)
		dwSize = sizeof(buffer);
		if (RegQueryValueExW(hKey, L"DisplayVersion", NULL, &dwType,
			(LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
			displayVersion = wstring(buffer);
		}
		else {
			// 如果没有 DisplayVersion，尝试读取 ReleaseId
			dwSize = sizeof(buffer);
			if (RegQueryValueExW(hKey, L"ReleaseId", NULL, &dwType,
				(LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
				displayVersion = wstring(buffer);
			}
			else {
				displayVersion = L"Unknown";
			}
		}

		// 读取当前构建号
		dwSize = sizeof(buffer);
		if (RegQueryValueExW(hKey, L"CurrentBuild", NULL, &dwType,
			(LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
			currentBuild = wstring(buffer);
		}
		else {
			currentBuild = L"Unknown";
		}

		RegCloseKey(hKey);
	}
	else {
		productName = L"Unknown";
		displayVersion = L"Unknown";
		currentBuild = L"Unknown";
	}
	INFO_(L"[主界面]获取系统版本结束");
}
void home_window_() {
	INFO_(L"[主界面]进入主界面");
//窗口
	sf::RenderWindow home_window(sf::VideoMode({ 1600, 900 }), "macdowsOS tool", sf::Style::None);
	int Window_x = 0, Window_y = 0;
	//
	INFO_(L"[主界面]设置垂直同步");
	//垂直同步
	home_window.setVerticalSyncEnabled(ChuiZhiTongBu);
	INFO_(L"[主界面]设置字体");
//字体
	sf::Font font;
	if (!font.openFromFile("pingfang0.ttf")){
		ERROR_(L"[主界面]字体文件无法使用");
		MessageBox(NULL, (LPCTSTR)L" 字体文件不存在或不可用,请检查字体文件完整性和可用性", (LPCTSTR)L" macdowsOS tool 主界面", MB_OK|MB_ICONERROR);
		MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 程序即将退出", (LPCTSTR)L" macdowsOS tool 主界面", MB_OK | MB_ICONERROR);
		exit(0);
	}
	INFO_(L"[主界面]设置组件");

//红绿灯
	//红
	sf::CircleShape op_button_red(15);
	op_button_red.setFillColor(sf::Color(245, 0, 0));
	op_button_red.setPosition({ 30,30 });
	// set a 10-pixel wide orange outline
	op_button_red.setOutlineThickness(0.2);
	op_button_red.setOutlineColor(sf::Color(0, 0, 0));
	//黄
	sf::CircleShape op_button_yellow(15);
	op_button_yellow.setFillColor(sf::Color(255, 215, 0));
	op_button_yellow.setPosition({ 75,30 });
	// set a 10-pixel wide orange outline
	op_button_yellow.setOutlineThickness(0.2);
	op_button_yellow.setOutlineColor(sf::Color(0, 0, 0));
	//绿
	sf::CircleShape op_button_green(15);
	op_button_green.setFillColor(sf::Color(0, 245, 0));
	op_button_green.setPosition({ 120,30 });
	// set a 10-pixel wide orange outline
	op_button_green.setOutlineThickness(0.2);
	op_button_green.setOutlineColor(sf::Color(0, 0, 0));

//标题栏
	//标题栏 上
	sf::RectangleShape top_titlebar({ 1600, 150 });
	top_titlebar.setPosition({ 0,0 });
	top_titlebar.setFillColor(sf::Color(250, 250, 250));
	//标题栏 左
	sf::RectangleShape left_titlebar({ 400, 900 });
	left_titlebar.setPosition({ 0,0 });
	left_titlebar.setFillColor(sf::Color(240, 240, 240));

//上标题栏
	//上标题栏-副标题
	sf::Text top_titlebar_sim_title(font);
	top_titlebar_sim_title.setString(wstring(L"作者:雾蓝回针MistBlueSt\n版本:1.21"));
	top_titlebar_sim_title.setCharacterSize(30);
	top_titlebar_sim_title.setFillColor(sf::Color::Color(100, 100, 100));
	top_titlebar_sim_title.setPosition({ 425,40 });
	//上标题栏-系统版本 
	sf::Text top_titlebar_Windows_version(font);
	wstring productName, displayVersion, currentBuild;
	GetWindowsVersionInfo(productName, displayVersion, currentBuild);
	top_titlebar_Windows_version.setString(wstring(L" 系统名称:")+ productName+L"\n"+ wstring(L" 版本:")+displayVersion+L"\n"+ wstring(L" 版本号:")+ currentBuild);
	top_titlebar_Windows_version.setCharacterSize(25);
	top_titlebar_Windows_version.setFillColor(sf::Color::Color(130, 130, 130));
	top_titlebar_Windows_version.setPosition({ 1050,20 });

//侧边栏
	//侧边栏文字-主页
	sf::Text left_titlebar_homebutton_text(font);
	left_titlebar_homebutton_text.setString(L"主页");
	left_titlebar_homebutton_text.setCharacterSize(50);
	left_titlebar_homebutton_text.setFillColor(sf::Color::Color(80, 80, 80));
	left_titlebar_homebutton_text.setPosition({ 130,175 });
	//侧边栏文字-Windows仿macOS
	sf::Text left_titlebar_WindowsToMacbutton_text(font);
	left_titlebar_WindowsToMacbutton_text.setString(L"系统美化");
	left_titlebar_WindowsToMacbutton_text.setCharacterSize(50);
	left_titlebar_WindowsToMacbutton_text.setFillColor(sf::Color::Color(80, 80, 80));
	left_titlebar_WindowsToMacbutton_text.setPosition({ 130,275 });
	//侧边栏文字-系统修复
	sf::Text left_titlebar_WindowsFixbutton_text(font);
	left_titlebar_WindowsFixbutton_text.setString(L"系统修复");
	left_titlebar_WindowsFixbutton_text.setCharacterSize(50);
	left_titlebar_WindowsFixbutton_text.setFillColor(sf::Color::Color(80, 80, 80));
	left_titlebar_WindowsFixbutton_text.setPosition({ 130,375 });
	//侧边栏文字=工具箱
	sf::Text left_titlebar_toolboxbutton_text(font);
	left_titlebar_toolboxbutton_text.setString(L"工具箱");
	left_titlebar_toolboxbutton_text.setCharacterSize(50);
	left_titlebar_toolboxbutton_text.setFillColor(sf::Color::Color(80, 80, 80));
	left_titlebar_toolboxbutton_text.setPosition({ 130,475 });
	//侧边栏文字=设置 
	sf::Text left_titlebar_settingbutton_text(font);
	left_titlebar_settingbutton_text.setString(L"设置");
	left_titlebar_settingbutton_text.setCharacterSize(50);
	left_titlebar_settingbutton_text.setFillColor(sf::Color::Color(80, 80, 80));
	left_titlebar_settingbutton_text.setPosition({ 130,800 });
	

	//加载侧边栏图标
	sf::Texture homepage_icon;
	sf::Texture WinToMac_icon;
	sf::Texture WinFix_icon;
	sf::Texture ToolBox_icon;
	sf::Texture Setting_icon;
	INFO_(L"[主界面]加载图标");
	if (!homepage_icon.loadFromFile("./res/home.png") || !WinToMac_icon.loadFromFile("./res/WindowsToMac.png") || !WinFix_icon.loadFromFile("./res/WindowsFix.png") || !Setting_icon.loadFromFile("./res/Setting.png")|| !ToolBox_icon.loadFromFile("./res/Box.png")) {
		ERROR_(L"[主界面]无法加载图标");
		MessageBox(NULL, (LPCTSTR)L" 图标文件缺失或不存在或不可用,请检查res文件夹下图标的完整性和可用性", (LPCTSTR)L" macdowsOS tool 主界面", MB_OK | MB_ICONERROR);
		MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 程序即将退出", (LPCTSTR)L" macdowsOS tool 主界面", MB_OK | MB_ICONERROR);
		exit(0);
	}

	//侧边栏图标-主页
	sf::RectangleShape homepage_ico({ 50, 50 });
	homepage_ico.setPosition({ 70,180 });
	homepage_ico.setTexture(&homepage_icon);
	//侧边栏图标-Win仿mac
	sf::RectangleShape WinToMac_ico({ 50, 50 });
	WinToMac_ico.setPosition({ 70,280 });
	WinToMac_ico.setTexture(&WinToMac_icon);
	//侧边栏图标 Win修复
	sf::RectangleShape WinFix_ico({ 50, 50 });
	WinFix_ico.setPosition({ 70,380 });
	WinFix_ico.setTexture(&WinFix_icon);
	//侧边栏图标 工具箱
	sf::RectangleShape ToolBox_ico({ 50, 50 });
	ToolBox_ico.setPosition({ 70,480 });
	ToolBox_ico.setTexture(&ToolBox_icon);
	//侧边栏图标-设置
	sf::RectangleShape Setting_ico({ 50, 50 });
	Setting_ico.setPosition({ 70,805 });
	Setting_ico.setTexture(&Setting_icon);

	//侧边栏背景-主页
	sf::RectangleShape homepage_icon_background({ 400, 100 });
	homepage_icon_background.setPosition({ 0,155 });
	homepage_icon_background.setFillColor(sf::Color::Color(210, 210, 210));
	//侧边栏背景-Win仿mac
	sf::RectangleShape WinToMac_icon_background({ 400, 100 });
	WinToMac_icon_background.setPosition({ 0,255 });
	WinToMac_icon_background.setFillColor(sf::Color::Color(210, 210, 210));
	//侧边栏背景-WinFix
	sf::RectangleShape WinFix_icon_background({ 400, 100 });
	WinFix_icon_background.setPosition({ 0,355 });
	WinFix_icon_background.setFillColor(sf::Color::Color(210, 210, 210));
	//侧边栏背景-工具箱
	sf::RectangleShape ToolBox_icon_background({ 400, 100 });
	ToolBox_icon_background.setPosition({ 0,455 });
	ToolBox_icon_background.setFillColor(sf::Color::Color(210, 210, 210));
	//侧边栏背景-设置
	sf::RectangleShape Setting_icon_background({ 400, 120 });
	Setting_icon_background.setPosition({ 0,780 });
	Setting_icon_background.setFillColor(sf::Color::Color(210, 210, 210));
//主页
	//主页-标题
	sf::Text homepage_main_title(font);
	homepage_main_title.setString(L"欢迎使用 MacdowsOS Tool");
	homepage_main_title.setCharacterSize(80);
	homepage_main_title.setFillColor(sf::Color::Color(80, 80, 80));
	homepage_main_title.setPosition({ 525,300 });
	//主页-副标题
	sf::Text homepage_sim_title(font);
	homepage_sim_title.setString(L"一个自动化Windows仿macOS美化工具");
	homepage_sim_title.setCharacterSize(40);
	homepage_sim_title.setFillColor(sf::Color::Color(120, 120, 120));
	homepage_sim_title.setPosition({ 660,400 });
	//主页-分界线
	sf::RectangleShape homepage_line({925, 3});
	homepage_line.rotate(sf::degrees(0));
	homepage_line.setFillColor(sf::Color::Color(220, 220, 220));
	homepage_line.setPosition({ 550,475 });
	//主页=更新日志标题
	sf::Text homepage_update_note_title(font);
	homepage_update_note_title.setString(L"工具注意事项:");
	homepage_update_note_title.setCharacterSize(30);
	homepage_update_note_title.setFillColor(sf::Color::Color(90, 90, 90));
	homepage_update_note_title.setPosition({ 500,500 });
	//主页-更新日志
	sf::Text homepage_update_note(font);
	homepage_update_note.setString(L"1.本工具适用于Windows10 1809以上的所有Windows版本\n2.请以管理员身份运行本程序\n3.建议在安装前备份好重要文件和创建还原点 作者不保证本程序100%不出问题");
	homepage_update_note.setCharacterSize(30);
	homepage_update_note.setFillColor(sf::Color::Color(90, 90, 90));
	homepage_update_note.setPosition({ 525,550 });

//WinToMac页面
	//WinToMac页面-win11
	sf::Text WinToMac_Win11_ButtonText(font);
	WinToMac_Win11_ButtonText.setString(L"Windows仿macOS(win11)");
	WinToMac_Win11_ButtonText.setCharacterSize(50);
	WinToMac_Win11_ButtonText.setFillColor(sf::Color::Color(90, 90, 90));
	WinToMac_Win11_ButtonText.setPosition({ 700,300 });
	//WinToMac页面-win10
	sf::Text WinToMac_Win10_ButtonText(font);
	WinToMac_Win10_ButtonText.setString(L"Windows仿macOS(win10)");
	WinToMac_Win10_ButtonText.setCharacterSize(50);
	WinToMac_Win10_ButtonText.setFillColor(sf::Color::Color(90, 90, 90));
	WinToMac_Win10_ButtonText.setPosition({ 700,400 });
	//WinToMac页面-单独选择一个部分安装
	sf::Text WinToMac_OnlyOnecraft_ButtonText(font);
	WinToMac_OnlyOnecraft_ButtonText.setString(L"单独选择一个部分安装");
	WinToMac_OnlyOnecraft_ButtonText.setCharacterSize(50);
	WinToMac_OnlyOnecraft_ButtonText.setFillColor(sf::Color::Color(90, 90, 90));
	WinToMac_OnlyOnecraft_ButtonText.setPosition({ 700,500 });

	//WinToMac背景-win11
	sf::RectangleShape WinToMac_Buttonbackground_win11({ 1200, 100 });
	WinToMac_Buttonbackground_win11.setPosition({ 400,275 });
	WinToMac_Buttonbackground_win11.setFillColor(sf::Color::Color(225, 225, 225));
	//WinToMac背景-win10
	sf::RectangleShape WinToMac_Buttonbackground_win10({ 1200, 100 });
	WinToMac_Buttonbackground_win10.setPosition({ 400,375 });
	WinToMac_Buttonbackground_win10.setFillColor(sf::Color::Color(225, 225, 225));
	//WinToMac背景-单独选择一个部分安装
	sf::RectangleShape WinToMac_Buttonbackground_celecraft({ 1200, 100 });
	WinToMac_Buttonbackground_celecraft.setPosition({ 400,475 });
	WinToMac_Buttonbackground_celecraft.setFillColor(sf::Color::Color(225, 225, 225));

//WinFix页面
	//WinFix页面-文字
	sf::Text WinFix_ButtonText(font);
	WinFix_ButtonText.setString(L"开始");
	WinFix_ButtonText.setCharacterSize(50);
	WinFix_ButtonText.setFillColor(sf::Color::Color(90, 90, 90));
	WinFix_ButtonText.setPosition({ 900,400 });
	//WinFix页面-背景
	sf::RectangleShape WinFix_Buttonbackground({ 1200, 100 });
	WinFix_Buttonbackground.setPosition({ 400,375 });
	WinFix_Buttonbackground.setFillColor(sf::Color::Color(225, 225, 225));

//工具箱页面
	//加载资源
	sf::Texture WindowRound_icon;
	INFO_(L"[主界面]加载图标ToolBox");
	if (!WindowRound_icon.loadFromFile("./res/tool/WindowRound.png")) {
		ERROR_(L"[主界面]无法加载图标ToolBox");
		MessageBox(NULL, (LPCTSTR)L" 图标文件缺失或不存在或不可用,请检查res文件夹下图标的完整性和可用性", (LPCTSTR)L" macdowsOS tool 主界面", MB_OK | MB_ICONERROR);
		MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 程序即将退出", (LPCTSTR)L" macdowsOS tool 主界面", MB_OK | MB_ICONERROR);
		exit(0);
	}
	//WindowRound背景
	sf::RectangleShape WindowRound_background({ 1200, 100 });
	WindowRound_background.setPosition({ 400,150 });
	WindowRound_background.setFillColor(sf::Color::Color(225, 225, 225));
	//WindowRound图标
	sf::RectangleShape WindowRound_ico({ 60, 60 });
	WindowRound_ico.setPosition({ 430,170 });
	WindowRound_ico.setTexture(&WindowRound_icon);
	//WindowRound文字
	sf::Text WindowRound_text(font);
	WindowRound_text.setString(L"设置窗口R角");
	WindowRound_text.setCharacterSize(50);
	WindowRound_text.setFillColor(sf::Color::Color(90, 90, 90));
	WindowRound_text.setPosition({ 800,165 });
//状态变量
 	//拖动
	bool isDragging = false;
	sf::Vector2i dragOffset;
	//左侧菜单栏背景
	int left_titlebar_backgournd = 0;
	//当前页面
	int now_page = 1;
	//WinToMac背景
	int WinToMac_Button_background = 0;
	//WinFix背景
	bool WinFix_Button_background = false;
	//ToolBox背景
	int ToolBox_Button_background = 0;
//循环
	INFO_(L"[主界面]主循环");
	while (home_window.isOpen()){
		//缩放
		if (Window_xx == 1) {
			Window_x = 800;
			Window_y = 450;
			home_window.setSize(sf::Vector2u(800, 450));
		}
		else if (Window_xx == 2) {
			Window_x = 1200;
			Window_y = 675;
			home_window.setSize(sf::Vector2u(1200, 675));
		}
		else if (Window_xx == 3) {
			Window_x = 1600;
			Window_y = 900;
			home_window.setSize(sf::Vector2u(1600, 900));
		}
		else {
			Window_x = 2000;
			Window_y = 1125;
			home_window.setSize(sf::Vector2u(2000, 1125));
		}
	//阴影和圆角
		// 获取句柄
		hwnd_main = home_window.getNativeHandle();
		//设置窗口圆角
		HRGN hRgn = CreateRoundRectRgn(0, 0, Window_x, Window_y, 90, 90);
		SetWindowRgn(hwnd_main, hRgn, TRUE);
		DeleteObject(hRgn); // 记得删除区域对象[citation:8]
		// 启用窗口阴影
		const MARGINS shadow = { 1, 1, 1, 1 };
		DwmExtendFrameIntoClientArea(hwnd_main, &shadow);
		//窗口
		WindowRound();
		home_window.setVerticalSyncEnabled(ChuiZhiTongBu);
		while (const optional event = home_window.pollEvent()){
			sf::Vector2f worldPos = home_window.mapPixelToCoords(sf::Mouse::getPosition(home_window));
			int mouse_x = worldPos.x;
			int mouse_y = worldPos.y;
			if (open_debug_window) {
				MESSAGE_(L"[主界面]鼠标坐标 X:", mouse_x);
				MESSAGE_(L"[主界面]鼠标坐标 Y:", mouse_y);
				MESSAGE_(L"[主界面]拖动状态", isDragging);
			}
		//鼠标拖动窗口
			//检测按下鼠标
			if (event->is<sf::Event::MouseButtonPressed>() && mouse_y < 150) {
				isDragging = true;
				dragOffset = home_window.getPosition() - sf::Mouse::getPosition();
			}	
			//检测松开鼠标
			if (event->is<sf::Event::MouseButtonReleased>()||!(0<=mouse_y&&mouse_y <= 150)|| !(0 <= mouse_x && mouse_x <= 1600)) {
				isDragging = false;
			}
			//移动窗口
			if (event->is<sf::Event::MouseMoved>()&& isDragging) {
				home_window.setPosition(sf::Mouse::getPosition() + dragOffset);
			}
		//红绿灯
			//按下鼠标
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				//y坐标
				if (30 <= mouse_y && mouse_y <= 60) {
					//红
					if (30 <= mouse_x && mouse_x <= 60) {
						INFO_(L"[主界面]窗口关闭");
						home_window.close();
					}
					//黄
					if (75 <= mouse_x && mouse_x <= 105) {
						INFO_(L"[主界面]最小化");
						PostMessage(hwnd_main, WM_SYSCOMMAND, SC_MINIMIZE, 0);
						isDragging = false;
					}
					//绿
					if (120 <= mouse_x && mouse_x <= 150) {
						INFO_(L"[主界面]每日一言");
						wstring hitokoto = GetHitokoto();
						MessageBoxW(NULL,hitokoto.c_str(),L"macdowsOS tool 每日亿言",MB_OK);
						Sleep(100);
					}
				}
			}
		//左侧菜单栏
			if (0 <= mouse_x && mouse_x <= 400) {
				if (155 <= mouse_y && mouse_y < 255) {
					left_titlebar_backgournd = 1;
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						INFO_(L"[主界面]点击左侧菜单 1");
						now_page = 1;
					}
				}else if (255 <= mouse_y && mouse_y < 355) {
					left_titlebar_backgournd = 2;
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						INFO_(L"[主界面]点击左侧菜单 2");
						now_page = 2;
					}
				}
				else if (355 <= mouse_y && mouse_y < 455) {
					left_titlebar_backgournd = 3;
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						INFO_(L"[主界面]点击左侧菜单 3");
						now_page = 3;
					}
				}
				else if (455 <= mouse_y && mouse_y < 555) {
					left_titlebar_backgournd = 4;
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						INFO_(L"[主界面]点击左侧菜单 4");
						now_page = 4;
					}
				}
				else if (780 <= mouse_y && mouse_y < 900) {
					left_titlebar_backgournd = 5;
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						//设置
						isDragging = false;
						home_window.setSize({ 1,1 });
						INFO_(L"[主界面]点击左侧菜单 5");
						SettingWindow_();
						home_window.setSize(sf::Vector2u(Window_x,Window_y ));
						now_page = 1;
						isDragging = false;
						while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
							;
						}
					}
				}else {
					left_titlebar_backgournd = 0;
				}
			}
			else {
				left_titlebar_backgournd =0;
			}
		//每个页面的事件
			if (now_page == 1) {
				//主页
				;
			}
			else if (now_page == 2) {
				//WinToMac
				if (400 < mouse_x && mouse_x <= 1600) {
					if (275 < mouse_y && mouse_y <= 375) {
						WinToMac_Button_background = 1;
						if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
							INFO_(L"[主界面]点击WinToMac按钮 1");
							WinToMacWin11();
						}
					}
					else if (375 < mouse_y && mouse_y <= 475) {
						WinToMac_Button_background = 2;
						if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
							INFO_(L"[主界面]点击WinToMac按钮 2");
							WinToMacWin10();
						}
					}
					else if (475 < mouse_y && mouse_y <= 575) {
						WinToMac_Button_background = 3;
						if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
							INFO_(L"[主界面]点击WinToMac按钮 3");
							//设置
							isDragging = false;
							home_window.setSize({ 1,1 });
							INFO_(L"[主界面]点击左侧菜单 5");
							CeleCraft_();
							home_window.setSize(sf::Vector2u(Window_x,Window_y ));
							isDragging = false;
							while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
								;
							}
						}
					}
					else {
						WinToMac_Button_background = 0;
					}
				}
				else {
					WinToMac_Button_background = 0;
				}
			}
			else if (now_page == 3) {
				//WinFix
				if (400 < mouse_x && mouse_x <= 1600 && 375 < mouse_y && mouse_y <= 475) {
					WinFix_Button_background = true;
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						INFO_(L"[主界面]点击WinFix按钮 1");
						WindowsFix();
					}
				}
				else {
					WinFix_Button_background = false;
				}

			}
			else if (now_page == 4) {
				//ToolBox
				if (400 < mouse_x && mouse_x <= 1600) {
					if (150 < mouse_y && mouse_y <= 250) {
						ToolBox_Button_background = 1;
						if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
							INFO_(L"[主界面]点击ToolBox按钮 1");
							isDragging = false;
							home_window.setSize({ 1,1 });
							WinroundWindow_();
							home_window.setSize(sf::Vector2u(Window_x,Window_y ));
							isDragging = false;
							while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
								;
							}
						}
					}
					else {
						ToolBox_Button_background = 0;
					}
				}
				else {
					ToolBox_Button_background = 0;
				}
			}

		}
	//绘制
		home_window.clear(sf::Color::White);
		//页面
		if (now_page == 1) {
			//主页
			home_window.draw(homepage_main_title);
			home_window.draw(homepage_sim_title);
			home_window.draw(homepage_line);
			home_window.draw(homepage_update_note_title);
			home_window.draw(homepage_update_note);
		}
		else if (now_page == 2) {
			//WinToMac
			//背景
			if (WinToMac_Button_background == 1) home_window.draw(WinToMac_Buttonbackground_win11);
			if (WinToMac_Button_background == 2) home_window.draw(WinToMac_Buttonbackground_win10);
			if (WinToMac_Button_background == 3) home_window.draw(WinToMac_Buttonbackground_celecraft);
			//文字
			home_window.draw(WinToMac_Win11_ButtonText);
			home_window.draw(WinToMac_Win10_ButtonText);
			home_window.draw(WinToMac_OnlyOnecraft_ButtonText);
		}
		else if (now_page == 3) {
			//WinFix
			if (WinFix_Button_background) home_window.draw(WinFix_Buttonbackground);
			home_window.draw(WinFix_ButtonText);
		}
		else if (now_page == 4) {
			//ToolBox
			if(ToolBox_Button_background==1)home_window.draw(WindowRound_background);
			home_window.draw(WindowRound_ico);
			home_window.draw(WindowRound_text);
		}
		else if (now_page == 5) {

		}
		//UI
		home_window.draw(top_titlebar);
		home_window.draw(left_titlebar);
		//红绿灯
		home_window.draw(op_button_red);
		home_window.draw(op_button_yellow);
		home_window.draw(op_button_green);
		//标题栏文字
		home_window.draw(top_titlebar_sim_title);
		home_window.draw(top_titlebar_Windows_version);
		//左侧菜单栏背景
		if (left_titlebar_backgournd == 1||now_page==1) home_window.draw(homepage_icon_background);
		if (left_titlebar_backgournd == 2||now_page==2) home_window.draw(WinToMac_icon_background);
		if (left_titlebar_backgournd == 3||now_page==3) home_window.draw(WinFix_icon_background);
		if (left_titlebar_backgournd == 4||now_page==4) home_window.draw(ToolBox_icon_background);
		if (left_titlebar_backgournd == 5||now_page==5) home_window.draw(Setting_icon_background);
		//左侧菜单图标文字
		home_window.draw(left_titlebar_homebutton_text);
		home_window.draw(left_titlebar_WindowsToMacbutton_text);
		home_window.draw(left_titlebar_WindowsFixbutton_text);
		home_window.draw(left_titlebar_toolboxbutton_text);
		home_window.draw(left_titlebar_settingbutton_text);
		//左侧菜单图标
		home_window.draw(homepage_ico);
		home_window.draw(WinToMac_ico);
		home_window.draw(WinFix_ico);
		home_window.draw(ToolBox_ico);
		home_window.draw(Setting_ico);
	
		

		home_window.display();
	}
	return;
}