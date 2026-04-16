#pragma once
//C++
#include<Windows.h>
#include<bits/stdc++.h>
//macdowsOS tool
#include"Theme_Install_win10.h"
#include"Theme_Install_win11.h"
#include"Theme_patch_install.h"
#include"mydockfinder_Install.h"
#include"StartAllBack_Install.h"
#include"Icon_install.h"
#include"DeleteTheme.h"
#include"restart.h"
#include"LogSystem.h"
#include"CreateSafePoint.h"
#include"WindowsFix.h"
#include"WindowConfig.h"
#include"OldNewExplorerInstall.h"
#include"FinalSetting.h"
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
//Windows
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
using namespace std;

void WinToMacWin10() {
	if (MessageBox(NULL, (LPCTSTR)L"点击确定进入安装进程", (LPCTSTR)L" macdowsOS tool", MB_YESNO) == 6) {
		MessageBox(NULL, (LPCTSTR)L"请在安装前保存正在编辑的文档 代码等重要文件 安装成功后程序会自动重启 ", (LPCTSTR)L"macdowsOS tool", MB_OK | MB_ICONINFORMATION);
		MessageBox(NULL, (LPCTSTR)L"macdowsOS Tool安装某些组件使用的是自动操作技术 请勿在macdowsOS Tool安装时关闭这些窗口 同时请注意若安装过程中有权限提升弹窗请点击授权 不点的话大概率卡住", (LPCTSTR)L"macdowsOS tool", MB_OK | MB_ICONINFORMATION);
		INFO_(L"[WinToMac]启动");
		// 窗口
		sf::RenderWindow WinToMac_window(sf::VideoMode({ 1500, 1200 }), L"macOS Tahoe 安装助理", sf::Style::None);
		int Window_x = 0, Window_y = 0;
		//
		INFO_(L"[WinToMac]设置垂直同步");
		//垂直同步
		WinToMac_window.setVerticalSyncEnabled(ChuiZhiTongBu);
		INFO_(L"[WinToMac]设置字体");
		//字体
		sf::Font font;
		if (!font.openFromFile("pingfang0.ttf")) {
			ERROR_(L"[WinToMac]字体文件无法使用");
			MessageBox(NULL, (LPCTSTR)L" 字体文件不存在或不可用,请检查字体文件完整性和可用性", (LPCTSTR)L" macdowsOS tool", MB_OK | MB_ICONERROR);
			MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 程序即将退出", (LPCTSTR)L" macdowsOS tool", MB_OK | MB_ICONERROR);
			exit(0);
		}
		INFO_(L"[WinToMac]初始化组件");
		//文字
			//标题
		sf::Text install_text(font);
		install_text.setString(wstring(L"macOS Tahoe"));
		install_text.setCharacterSize(60);
		install_text.setFillColor(sf::Color::Color(230, 230, 230));
		install_text.setPosition({ 560,480 });
		install_text.setStyle(sf::Text::Bold);
		//标题
		sf::Text install_text1(font);
		install_text1.setString(wstring(L"macOS Tahoe 将安装在磁盘 Macintosh HD 上"));
		install_text1.setCharacterSize(30);
		install_text1.setFillColor(sf::Color::Color(200, 200, 200));
		install_text1.setPosition({ 435,600 });
		//磁盘文字
		sf::Text disk_text(font);
		disk_text.setString(wstring(L"Macintosh HD \n 10 PB 可用"));
		disk_text.setCharacterSize(20);
		disk_text.setFillColor(sf::Color::Color(240, 240, 240));
		disk_text.setPosition({ 672.5,910 });
		disk_text.setStyle(sf::Text::Bold);

		//图标
			//加载图标
		sf::Texture mac26_icon;
		sf::Texture disk_icon;
		INFO_(L"[WinToMac]加载图标");
		if (!mac26_icon.loadFromFile("./res/install/icon1.jpg") || !disk_icon.loadFromFile("./res/install/icon2.png")) {
			ERROR_(L"[WinToMac]无法加载图标");
			MessageBox(NULL, (LPCTSTR)L" 图标文件缺失或不存在或不可用,请检查res文件夹下图标的完整性和可用性", (LPCTSTR)L" macdowsOS tool", MB_OK | MB_ICONERROR);
			MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 安装进程即将退出", (LPCTSTR)L" macdowsOS tool", MB_OK | MB_ICONERROR);
			return;
		}
		//tahoe 图标
		sf::CircleShape ico_26(170);
		ico_26.setPointCount(100);
		ico_26.setPosition({ 580,100 });
		ico_26.setOutlineThickness(15);
		ico_26.setOutlineColor(sf::Color(200, 200, 200));
		ico_26.setTexture(&mac26_icon);

		//磁盘图标
		sf::RectangleShape diskicon({ 200,200 });
		diskicon.setTexture(&disk_icon);
		diskicon.setPosition({ 640,700 });
		//右下角按钮
		sf::CircleShape button1(50);
		button1.setPointCount(100);
		button1.setPosition({ 1150,1050 });
		button1.setFillColor(sf::Color(30, 100, 190));

		sf::RectangleShape button2({ 200,100 });
		button2.setPosition({ 1200,1050 });
		button2.setFillColor(sf::Color(30, 100, 190));

		sf::CircleShape button3(50);
		button3.setPointCount(100);
		button3.setPosition({ 1350,1050 });
		button3.setFillColor(sf::Color(30, 100, 190));

		sf::Text buttontext(font);
		buttontext.setString(wstring(L"继续"));
		buttontext.setCharacterSize(50);
		buttontext.setFillColor(sf::Color::Color(240, 240, 240));
		buttontext.setPosition({ 1250,1070 });
		//加载进度条
		sf::RectangleShape loudingline({ 0,50 });
		loudingline.setPosition({ 0,1150 });
		loudingline.setFillColor(sf::Color(255, 255, 255));
		//红绿灯
			//红
		sf::CircleShape op_button_red(15);
		op_button_red.setFillColor(sf::Color(255, 0, 0));
		op_button_red.setPosition({ 30,30 });
		op_button_red.setOutlineThickness(0.2);
		op_button_red.setOutlineColor(sf::Color(0, 0, 0));

		//状态变量
			//拖动
		bool isDragging = false;
		sf::Vector2i dragOffset;
		//
		bool buttonbackgound = false;
		//窗口循环
		while (WinToMac_window.isOpen()) {
			//缩放
			if (Window_xx == 1) {
				Window_x = 750;
				Window_y = 600;
				WinToMac_window.setSize(sf::Vector2u(Window_x, Window_y));
			}
			else if (Window_xx == 2) {
				Window_x = 1125;
				Window_y = 900;
				WinToMac_window.setSize(sf::Vector2u(Window_x, Window_y));
			}
			else if (Window_xx == 3) {
				Window_x = 1500;
				Window_y = 1200;
				WinToMac_window.setSize(sf::Vector2u(Window_x, Window_y));
			}
			else {
				Window_x = 2250;
				Window_y = 1800;
				WinToMac_window.setSize(sf::Vector2u(Window_x, Window_y));
			}
		//阴影和圆角
			// 获取句柄
			HWND hwnd_setting = WinToMac_window.getNativeHandle();
			//设置窗口圆角
			HRGN hRgn = CreateRoundRectRgn(0, 0, Window_x, Window_y, 90, 90);
			SetWindowRgn(hwnd_setting, hRgn, TRUE);
			DeleteObject(hRgn); // 记得删除区域对象[citation:8]
			// 启用窗口阴影
			const MARGINS shadow = { 100, 100, 100, 100 };
			DwmExtendFrameIntoClientArea(hwnd_setting, &shadow);
			//垂直同步
			WinToMac_window.setVerticalSyncEnabled(ChuiZhiTongBu);
			while (const std::optional event = WinToMac_window.pollEvent()) {
				sf::Vector2f worldPos = WinToMac_window.mapPixelToCoords(sf::Mouse::getPosition(WinToMac_window));
				int mouse_x = worldPos.x;
				int mouse_y = worldPos.y;
				//日志
				if (open_debug_window) {
					MESSAGE_(L"[WinToMac]鼠标坐标 X:", mouse_x);
					MESSAGE_(L"[WinToMac]鼠标坐标 Y:", mouse_y);
					MESSAGE_(L"[WinToMac]拖动状态", isDragging);
				}
				//鼠标拖动窗口
					//检测按下鼠标
				if (event->is<sf::Event::MouseButtonPressed>() && mouse_y < 80) {
					isDragging = true;
					dragOffset = WinToMac_window.getPosition() - sf::Mouse::getPosition();
				}
				//检测松开鼠标
				if (event->is<sf::Event::MouseButtonReleased>() || !(0 <= mouse_y && mouse_y <= 1200) || !(0 <= mouse_x && mouse_x <= 1500)) {
					isDragging = false;
				}
				//移动窗口
				if (event->is<sf::Event::MouseMoved>() && isDragging) {
					WinToMac_window.setPosition(sf::Mouse::getPosition() + dragOffset);
				}
				//红绿灯
					//按下鼠标
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					//y坐标
					if (30 <= mouse_y && mouse_y <= 60) {
						//红
						if (30 <= mouse_x && mouse_x <= 60) {
							INFO_(L"[WinToMac]退出");
							WinToMac_window.close();
						}
					}
				}
				//按钮
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					//y坐标
					if (1050 <= mouse_y && mouse_y <= 1150) {
						//x
						if (1150 <= mouse_x && mouse_x <= 1450) {
							install_text1.setString(wstring(L"正在将 macOS Tahoe 安装在磁盘 Macintosh HD 上"));
							install_text1.setPosition({ 405,600 });
							WinToMac_window.clear(sf::Color::Color(70, 70, 70));
							WinToMac_window.draw(install_text);
							WinToMac_window.draw(install_text1);
							WinToMac_window.draw(ico_26);
							WinToMac_window.display();

							//INT64 seq = Start_CreateSafePoint(L"macdowsOS tool 还原点");
							INFO_(L"[Win仿macWin10]mydockfinder安装");
							if (!mydockfinder_install()) {
								return;
							}

							WinToMac_window.clear(sf::Color::Color(70, 70, 70));
							WinToMac_window.draw(install_text);
							WinToMac_window.draw(install_text1);
							WinToMac_window.draw(ico_26);
							loudingline.setSize({ 187.5, 100 });
							WinToMac_window.draw(loudingline);
							WinToMac_window.display();

							INFO_(L"[Win仿macWin10]OldNewExplorer安装");
							OldNewExplorer_Install();

							WinToMac_window.clear(sf::Color::Color(70, 70, 70));
							WinToMac_window.draw(install_text);
							WinToMac_window.draw(install_text1);
							WinToMac_window.draw(ico_26);
							loudingline.setSize({ 355, 100 });
							WinToMac_window.draw(loudingline);
							WinToMac_window.display();

							INFO_(L"[Win仿macWin10]主题补丁安装");
							theme_path_install();

							WinToMac_window.clear(sf::Color::Color(70, 70, 70));
							WinToMac_window.draw(install_text);
							WinToMac_window.draw(install_text1);
							WinToMac_window.draw(ico_26);
							loudingline.setSize({ 545.5, 100 });
							WinToMac_window.draw(loudingline);
							WinToMac_window.display();

							INFO_(L"[Win仿macWin10]删除原有主题");
							Delete_theme();

							WinToMac_window.clear(sf::Color::Color(70, 70, 70));
							WinToMac_window.draw(install_text);
							WinToMac_window.draw(install_text1);
							WinToMac_window.draw(ico_26);
							loudingline.setSize({ 710, 100 });
							WinToMac_window.draw(loudingline);
							WinToMac_window.display();

							INFO_(L"[Win仿macWin10]安装主题");
							theme_install_win10();

							WinToMac_window.clear(sf::Color::Color(70, 70, 70));
							WinToMac_window.draw(install_text);
							WinToMac_window.draw(install_text1);
							WinToMac_window.draw(ico_26);
							loudingline.setSize({ 897.5, 100 });
							WinToMac_window.draw(loudingline);
							WinToMac_window.display();

							INFO_(L"[Win仿macWin10]安装图标");
							Icon_Install();

							WinToMac_window.clear(sf::Color::Color(70, 70, 70));
							WinToMac_window.draw(install_text);
							WinToMac_window.draw(install_text1);
							WinToMac_window.draw(ico_26);
							loudingline.setSize({ 1272.5, 100 });
							WinToMac_window.draw(loudingline);
							WinToMac_window.display();

							INFO_(L"[Win仿macWin10]最终优化设置");
							FinalSetting();

							WinToMac_window.clear(sf::Color::Color(70, 70, 70));
							WinToMac_window.draw(install_text);
							WinToMac_window.draw(install_text1);
							WinToMac_window.draw(ico_26);
							loudingline.setSize({ 1460, 100 });
							WinToMac_window.draw(loudingline);
							WinToMac_window.display();
							//Finish_CreateSafePoint(seq);
							INFO_(L"[Win仿macWin10]重启");
							restart_nonotice();
							return;
						}
					}
				}

			}

			WinToMac_window.clear(sf::Color::Color(70, 70, 70));
			//红绿灯
			WinToMac_window.draw(op_button_red);
			//
			WinToMac_window.draw(install_text);
			WinToMac_window.draw(install_text1);
			WinToMac_window.draw(ico_26);
			WinToMac_window.draw(diskicon);
			WinToMac_window.draw(disk_text);

			WinToMac_window.draw(button1);
			WinToMac_window.draw(button2);
			WinToMac_window.draw(button3);
			WinToMac_window.draw(buttontext);

			WinToMac_window.display();
		}
	}
	else {
		return;
	}

	return;
}