﻿#include "QuickBtn.h"
#include <Windows.h>
#include "SystemIcon.h"
#include "App.h"
#include <include/core/SkPaint.h>
#include "WindowBase.h"

QuickBtn::QuickBtn(WindowBase* root) :ControlBase(root)
{
	root->resizeHandlers.push_back(
		std::bind(&QuickBtn::resize, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseMoveHandlers.push_back(
		std::bind(&QuickBtn::mouseMove, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseDownHandlers.push_back(
		std::bind(&QuickBtn::mouseDown, this, std::placeholders::_1, std::placeholders::_2)
	);
}

QuickBtn::~QuickBtn()
{
}

void QuickBtn::paint(SkCanvas* canvas)
{
	SkPaint paint;
	if (hoverIndex > -1) {
		paint.setColor(0x221677ff);
		SkRect r = SkRect::MakeXYWH(8.f + 60 * hoverIndex, rect.fTop+22.f, 60.f,63.f);
		canvas->drawRoundRect(r, 6.f,6.f,paint);
	}
	std::vector<std::wstring> names{ L"桌面",L"音乐",L"视频",L"下载",L"图片",L"文档" };
	std::vector<GUID> ids{ FOLDERID_Desktop,FOLDERID_Music,FOLDERID_Videos,
						  FOLDERID_Downloads,FOLDERID_Pictures,FOLDERID_Documents };
	
	for (size_t i = 0; i < 6; i++)
	{
		auto img = SystemIcon::getIcon(ids[i], 24);
		canvas->drawImage(img, 10.f + 60 * i + 16.f, rect.fTop+16.f+12.f);
		auto textLength = wcslen(names[i].data()) * 2;
		auto fontText = App::GetFontText();
		fontText->setSize(16.f);
		if (hoverIndex == i) {
			paint.setColor(0xFF000000);
		}
		else {
			paint.setColor(0xFF666666);
		}
		
		canvas->drawSimpleText(names[i].data(), textLength, 
			SkTextEncoding::kUTF16, 
			10.f + 60 * i + 12.f, rect.fTop + 12.f + 62.f, *fontText, paint);
	}
}

void QuickBtn::resize(const int& w, const int& h)
{
	auto yVal = root->ctrls[2]->rect.fTop;
	rect.setXYWH(0.f, yVal, 380.f, 92.f);
}

void QuickBtn::mouseMove(const int& x, const int& y)
{
	int index = -1;
	if (rect.contains(x, y)&&x>10.f&&x<rect.fRight-16.f) {
		index = x / 64.f;		
	}
	else {
		index = -1;
	}
	if (index != hoverIndex) {
		hoverIndex = index;
		InvalidateRect(root->hwnd, nullptr, false);
	}
}

void QuickBtn::mouseDown(const int& x, const int& y)
{
}
