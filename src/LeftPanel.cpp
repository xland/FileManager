﻿#include "LeftPanel.h"
#include "WindowMain.h"
#include "SystemIcon.h"
#include "App.h"
#include "TitleBar.h"
#include "ToolBar.h"


LeftPanel::LeftPanel(WindowMain* root) :ControlBase(root)
{
	quickBtn = std::make_shared<QuickBtn>(root);
	diskList = std::make_shared<DiskList>(root);
	favoritePath = std::make_shared<FavoritePath>(root);
	settingBar = std::make_shared<SettingBar>(root);
}

LeftPanel::~LeftPanel()
{
}

void LeftPanel::paint(SkCanvas* canvas)
{
	if (!needPaint(canvas)) return;
	SkPaint paint;
	paint.setColor(0xFFF8FBFF);
	paint.setStyle(SkPaint::kFill_Style);
	canvas->drawRect(rect, paint);
}

void LeftPanel::resize(const int& w, const int& h)
{
	isDirty = true;
	quickBtn->isDirty = true;
	diskList->isDirty = true;
	favoritePath->isDirty = true;
	settingBar->isDirty = true;
	auto topVal = root->titleBar->rect.height() + root->toolBar->rect.height();
	rect.setXYWH(0.f, topVal, 380.f, h - topVal);
	diskList->y = topVal + 98.f;
	favoritePath->y = diskList->y + diskList->driveInfo.size() * (46 + 8)+6.f;
}
