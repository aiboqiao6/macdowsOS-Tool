#pragma once
// ============================================================
// FinalSetting.h
// 描述：最终设置工具，提供安装完成后的系统配置功能
//      包含桌面图标显示等最终调整设置
// ============================================================
#include"All.h"
#include"LogSystem.h"
#include<Windows.h>

namespace {
void FinalSetting() {
	INFO_(L"[最终设置工具]隐藏桌面图标");
	SetDesktopIconsPermanent(false);
	return;
}
void FinalSetting_back() {
	INFO_(L"[最终设置工具]显示桌面图标");
	SetDesktopIconsPermanent(true);
	return;
}

}
