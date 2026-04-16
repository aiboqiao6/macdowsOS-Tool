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
//log
#include"LogSystem.h"
//macdowsOS tool
#include"WindowConfig.h"
#include"AboutWindow.h"

#include"DeleteTheme.h"
#include"Icon_install.h"
#include"mydockfinder_Install.h"
#include"Theme_Install_win10.h"
#include"Theme_Install_win11.h"
#include"Theme_patch_install.h"
#include"WinToMacWin11.h"
#include"WinToMacWin10.h"
#include"restart.h"

using namespace std;

void CeleCraft_() {
	//等待图标释放
	while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		;
	}
	INFO_(L"[选择一个部分安装]启动");
	// 窗口
	sf::RenderWindow CeleCraft_window(sf::VideoMode({ 600, 900 }), L"选择一个部分安装", sf::Style::None);
	int Window_x = 0, Window_y = 0;
	//
	INFO_(L"[选择一个部分安装]设置垂直同步");
	//垂直同步
	CeleCraft_window.setVerticalSyncEnabled(ChuiZhiTongBu);
	INFO_(L"[选择一个部分安装]设置窗口阴影");
//字体
	sf::Font font;
	if (!font.openFromFile("pingfang0.ttf")) {
		ERROR_(L"[选择一个部分安装]字体文件无法使用");
		MessageBox(NULL, (LPCTSTR)L" 字体文件不存在或不可用,请检查字体文件完整性和可用性", (LPCTSTR)L" macdowsOS tool 选择一个部分安装", MB_OK | MB_ICONERROR);
		MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 程序即将退出", (LPCTSTR)L" macdowsOS tool 选择一个部分安装", MB_OK | MB_ICONERROR);
		exit(0);
	}
	INFO_(L"[选择一个部分安装]设置字体");
//红绿灯
	//红
	sf::CircleShape op_button_red(15);
	op_button_red.setFillColor(sf::Color(255, 0, 0));
	op_button_red.setPosition({ 30,30 });
	// set a 10-pixel wide orange outline
	op_button_red.setOutlineThickness(0.2);
	op_button_red.setOutlineColor(sf::Color(0, 0, 0));
//标题
	sf::Text title(font);
	title.setString(L"选择一个部分安装");
	title.setCharacterSize(40);
	title.setFillColor(sf::Color::Color(80, 80, 80));
	title.setPosition({ 140,20 });
	//项目文字
	sf::Text program_text(font);
	program_text.setString(L"安装mydockfinder\n\n安装图标\n\n安装主题(win11)\n\n安装主题(win10)\n\n主题补丁\n\n卸载主题\n\n刷新图标缓存\n\n安装StartAllBack(win11)");
	program_text.setCharacterSize(40);
	program_text.setFillColor(sf::Color::Color(80, 80, 80));
	program_text.setPosition({ 70,110 });
	//背景
	sf::RectangleShape text_background({ 600, 110 });
	text_background.setPosition({ 0,70 });
	text_background.setFillColor(sf::Color::Color(210, 210, 210));
	//标题背景
	sf::RectangleShape title_background({ 600, 80 });
	title_background.setPosition({ 0,0 });
	title_background.setFillColor(sf::Color(255, 255, 255));
//状态变量
	//拖动
	bool isDragging = false;
	sf::Vector2i dragOffset;
	//背景
	bool aboutback = false;
	//列表 偏移
	float y_oran_list = 0;
	//背景
	bool background = false;
	//窗口循环
	while (CeleCraft_window.isOpen()) {
		//缩放
		if (Window_xx == 1) {
			Window_x = 300;
			Window_y = 450;
			CeleCraft_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else if (Window_xx == 2) {
			Window_x = 450;
			Window_y = 675;
			CeleCraft_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else if (Window_xx == 3) {
			Window_x = 600;
			Window_y = 900;
			CeleCraft_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else {
			Window_x = 900;
			Window_y = 1350;
			CeleCraft_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
	//阴影和圆角
		// 获取句柄
		HWND hwnd_setting = CeleCraft_window.getNativeHandle();
		//选择一个部分安装窗口圆角
		HRGN hRgn = CreateRoundRectRgn(0, 0, Window_x, Window_y, 90, 90);
		SetWindowRgn(hwnd_setting, hRgn, TRUE);
		DeleteObject(hRgn); // 记得删除区域对象[citation:8]
		// 启用窗口阴影
		const MARGINS shadow = { 100, 100, 100, 100 };
		DwmExtendFrameIntoClientArea(hwnd_setting, &shadow);
		//垂直同步
		CeleCraft_window.setVerticalSyncEnabled(ChuiZhiTongBu);
		while (const std::optional event = CeleCraft_window.pollEvent()) {
			sf::Vector2f worldPos = CeleCraft_window.mapPixelToCoords(sf::Mouse::getPosition(CeleCraft_window));
			int mouse_x = worldPos.x;
			int mouse_y = worldPos.y;
			//日志
			if (open_debug_window) {
				MESSAGE_(L"[选择一个部分安装]鼠标坐标 X:", mouse_x);
				MESSAGE_(L"[选择一个部分安装]鼠标坐标 Y:", mouse_y);
				MESSAGE_(L"[选择一个部分安装]拖动状态", isDragging);
				MESSAGE_(L"[选择一个部分安装]列表滚动。偏移量 y_oran_list:", y_oran_list);
			}
		//鼠标拖动窗口
			//检测按下鼠标
			if (event->is<sf::Event::MouseButtonPressed>() && mouse_y < 80) {
				isDragging = true;
				dragOffset = CeleCraft_window.getPosition() - sf::Mouse::getPosition();
			}
			//检测松开鼠标
			if (event->is<sf::Event::MouseButtonReleased>() || !(0 <= mouse_y && mouse_y <= 900) || !(0 <= mouse_x && mouse_x <= 600)) {
				isDragging = false;
			}
			//移动窗口
			if (event->is<sf::Event::MouseMoved>() && isDragging) {
				CeleCraft_window.setPosition(sf::Mouse::getPosition() + dragOffset);
			}

		//红绿灯
			//按下鼠标
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				//y坐标
				if (30 <= mouse_y && mouse_y <= 60) {
					//红
					if (30 <= mouse_x && mouse_x <= 60) {
						INFO_(L"[选择一个部分安装]退出");
						CeleCraft_window.close();
					}
				}
			}
		//列表实时偏移坐标
			if (event->is<sf::Event::MouseWheelScrolled>()&& -90 <= y_oran_list && y_oran_list <= 100) {
				const auto& wheelEvent = event->getIf<sf::Event::MouseWheelScrolled>();
				if (0 <= mouse_y && mouse_y <= 900 && 0 <= mouse_x && mouse_x <= 600) {
					// 确认是垂直滚轮
					if (wheelEvent->wheel == sf::Mouse::Wheel::Vertical) {
						const int scrollSpeed = 20; // 滚动速度因子，您可以根据手感调整
						y_oran_list -= static_cast<int>(wheelEvent->delta * scrollSpeed);
						//维护位置 文字
						program_text.setPosition({ 70,110 + y_oran_list });
						//
						
					}
				}
			}
			//偏移量维护
			if (y_oran_list < -90) {
				y_oran_list = -90;
			}
			if (100 < y_oran_list) {
				y_oran_list = 100;
			}
		
		//按钮事件
			//x
			//x
			int oac_y = mouse_y - y_oran_list;
			if (open_debug_window) {
				MESSAGE_(L"[选择一个部分安装]偏移鼠标Y", oac_y);
			}
			
			if (0 <= mouse_x && mouse_x <= 600 && 100 < mouse_y) {
				if (85 <= oac_y && oac_y < 195) {
					background = true;
					text_background.setPosition({ 0,85 + y_oran_list });
					//按下鼠标
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						if (!mydockfinder_install()) {
							break;
						}
						restart_();
					}
				}
				else if (195 <= oac_y && oac_y < 305) {
					background = true;
					text_background.setPosition({ 0,195 + y_oran_list });
					//按下鼠标
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						Icon_Install();
						MessageBox(NULL, (LPCTSTR)L" 操作完成 原有的系统文件已经保存到./AppData/Icon/Icon_backup文件夹 部分图标可能因为缓存仍然为旧图标 请尝试使用本工具或win轻松设置的刷新图标缓存功能删除系统图标缓存", (LPCTSTR)L"选择一个部分安装", MB_OK);
					}
				}
				else if (305 <= oac_y && oac_y < 415) {
					background = true;
					text_background.setPosition({ 0,305 + y_oran_list });
					//按下鼠标
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						theme_install_win11();
						MessageBox(NULL, (LPCTSTR)L" 操作完成 请重新启动以应用更改", (LPCTSTR)L"选择一个部分安装", MB_OK);
						restart_();
					}
				}
				else if (415 <= oac_y && oac_y < 525) {
					background = true;
					text_background.setPosition({ 0,415 + y_oran_list });
					//按下鼠标
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						theme_install_win10();
						MessageBox(NULL, (LPCTSTR)L" 操作完成 请重新启动以应用更改", (LPCTSTR)L"选择一个部分安装", MB_OK);
						restart_();
					}
				}
				else if (525 <= oac_y && oac_y < 635) {
					background = true;
					text_background.setPosition({ 0,525 + y_oran_list });
					//按下鼠标
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						theme_path_install();
						MessageBox(NULL, (LPCTSTR)L" 操作完成 请重新启动以应用更改", (LPCTSTR)L"选择一个部分安装", MB_OK);
						restart_();
					}
				}
				else if (635 <= oac_y && oac_y < 745) {
					background = true;
					text_background.setPosition({ 0,635 + y_oran_list });
					//按下鼠标
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						Delete_theme();
						MessageBox(NULL, (LPCTSTR)L" 操作完成 主题已删除 ", (LPCTSTR)L"选择一个部分安装", MB_OK);
					}
				}
				else if (745 <= oac_y && oac_y < 855) {
					background = true;
					text_background.setPosition({ 0,745 + y_oran_list });
					//按下鼠标
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						system("start AppData\\Icon\\Icon.bat");
						MessageBox(NULL, (LPCTSTR)L" 操作完成", (LPCTSTR)L"选择一个部分安装", MB_OK);
					}
				}
				else if (855 <= oac_y && oac_y < 960) {
					background = true;
					text_background.setPosition({ 0,855 + y_oran_list });
					//按下鼠标
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						StartAllBack_SetupRun();
						MessageBox(NULL, (LPCTSTR)L" 操作完成", (LPCTSTR)L"选择一个部分安装", MB_OK);
					}
				}
				else {
					background = false;
				}
			}
			else {
				background = false;
			}


		}
		

		//
		CeleCraft_window.clear(sf::Color::Color(245, 245, 245));
		//项目背景
		if (background)CeleCraft_window.draw(text_background);
		//项目文字
		CeleCraft_window.draw(program_text);
		
		//标题背景
		CeleCraft_window.draw(title_background);
		//红绿灯
		CeleCraft_window.draw(op_button_red);
		
		//标题
		CeleCraft_window.draw(title);
		
		
		



		CeleCraft_window.display();
	}
	return;
}