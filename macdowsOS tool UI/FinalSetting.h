#pragma once
#include<bits/stdc++.h>
#include"All.h"
#include"LogSystem.h"
#include<Windows.h>
using namespace std;
void FinalSetting() {
	INFO_(L"[最终设置组件]隐藏桌面图标");
	SetDesktopIconsPermanent(false);
	return;
}