#pragma once
// ============================================================
// WindowRound.h
// 描述：窗口圆角修改工具，提供修改窗口圆角半径的功能
//      通过 DWM API 和区域裁剪实现窗口圆角效果
// ============================================================
#include<Windows.h>
#include<dwmapi.h>
#include<fstream>
#include<cstdlib>
#include"LogSystem.h"
void WindowRoundMain(wchar_t window_name[], bool WindowSizeFix,bool DEFC) {
	HWND hWnd_Window = FindWindowW(NULL, window_name);
	/*
	
	const DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
	DwmSetWindowAttribute(hWnd_Window, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));
	*/
    // DEFC
	if (DEFC) {
		const MARGINS shadow = { 1, 1, 1, 1 };
		DwmExtendFrameIntoClientArea(hWnd_Window, &shadow);
	}
    RECT rect;
	//获取窗口大小
	GetWindowRect(hWnd_Window, &rect);
	int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
	if (WindowSizeFix) {
		width *= 2;
		height *= 2;
	}
	HRGN hRgn = CreateRoundRectRgn(0, 0, width, height, 90, 90);
	//应用区域
	SetWindowRgn(hWnd_Window, hRgn, TRUE);
	DeleteObject(hRgn); // 记得删除区域对象[citation:8]

	return;
}
void WindowRound() {
	std::wifstream fin("./config/WindowRound.txt");
	if (!fin.is_open()) {
		ERROR_(L"[窗口圆角修改工具]读取配置文件时出现错误");
		MessageBox(NULL, (LPCTSTR)L" 读取配置文件时出现错误 程序将退出", (LPCTSTR)L" macdowsOS tool 窗口圆角修改工具", MB_OK | MB_ICONERROR);
		exit(0);
	}
	int temp1=0, temp2=0;
	wchar_t temp[1000];
	wchar_t ovc = '!',ovc1=' ';
	while (fin >> temp) {
		bool oc1, oc2;
		if (temp[0] == ovc||temp[0]==ovc1) return;
		if (temp1 == 1) {
			oc1 = true;
		}
		else {
			oc1 = false;
		}
		if (temp2 == 1) {
			oc2 = true;
		}
		else {
			oc2 = false;
		}
		WindowRoundMain(temp, oc1, oc2);
	}
	return;
}
