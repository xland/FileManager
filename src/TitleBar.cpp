﻿#include "TitleBar.h"
#include "WindowMain.h"
#include "App.h"
#include "include/core/SkFontMetrics.h"
#include "TitleBarBtns.h"

class WindowBase;
TitleBar::TitleBar(WindowMain* root) :ControlBase(root)
{
	addTab(false);
	btns = std::make_shared<TitleBarBtns>(root);
	root->mouseMoveHandlers.push_back(
		std::bind(&TitleBar::mouseMove, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseDownHandlers.push_back(
		std::bind(&TitleBar::mouseDown, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseDragHandlers.push_back(
		std::bind(&TitleBar::mouseDrag, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseUpHandlers.push_back(
		std::bind(&TitleBar::mouseUp, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->resizeHandlers.push_back(
		std::bind(&TitleBar::resize, this, std::placeholders::_1, std::placeholders::_2)
	);
}

TitleBar::~TitleBar()
{
}

void TitleBar::paint(SkCanvas* canvas)
{	
	if (!needPaint(canvas)) return;
	SkPaint paint;
	paint.setColor(0xFFD3E3FD);
	paint.setStyle(SkPaint::kFill_Style);
	canvas->drawRect(rect, paint);
}

void TitleBar::mouseMove(const int& x, const int& y)
{
	if (!rect.contains(x, y)) {
		return;
	}
	bool flag{ false };
	for (auto& tab:tabs)
	{
		auto tempFlag = tab->hoverChange(x, y);
		if (tempFlag) {
			flag = true;
		}
	}
	if (flag) {
		InvalidateRect(root->hwnd, nullptr, false);
	}	
}

void TitleBar::mouseDown(const int& x, const int& y)
{
	if (!rect.contains(x, y)) {
		return;
	}
	auto it = std::find_if(tabs.begin(), tabs.end(), [](auto& item) {return item->hoverIndex != -1; });
	if (it == tabs.end()) {
		GetCursorPos(&startPos);
		ScreenToClient(root->hwnd, &startPos);
		SetCapture(root->hwnd);
		draggingWindow = true;
		return;
	}
	auto tab = *it;
	if (tab->isSelected) {
		return;
	}
	auto it2 = std::find_if(tabs.begin(), tabs.end(), [](auto& item) { return item->isSelected; });
	(*it2)->isSelected = false;
	(*it2)->isDirty = true;
	tab->isSelected = true;
	tab->isDirty = true;
	for (auto& item:tabs)
	{
		if (item->historyNum > tab->historyNum) {
			item->historyNum -= 1;
		}
	}
	tab->historyNum = tabs.size() - 1;
	InvalidateRect(root->hwnd, nullptr, false);
}

void TitleBar::mouseUp(const int& x, const int& y)
{
	draggingWindow = false;
	ReleaseCapture();
}

void TitleBar::mouseDrag(const int& x, const int& y)
{
	if (draggingWindow) {
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(root->hwnd, &point);
		RECT windowRect;
		GetWindowRect(root->hwnd, &windowRect);
		int dx = windowRect.left + point.x - startPos.x;
		int dy = windowRect.top + point.y - startPos.y;
		SetWindowPos(root->hwnd, nullptr, dx, dy, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

void TitleBar::resize(const int& w, const int& h)
{
	isDirty = true;
	rect.setXYWH(0, 0, w, 56.f);
	for (size_t i = 0; i < tabs.size(); i++)
	{
		tabs[i]->rect.setXYWH(12.f+i*200.f+i*3.f, 10.f, 200.f, 46.f);
	}	
}

void TitleBar::addTab(bool needRefresh)
{
	for (auto& tab:tabs)
	{
		if (tab->isSelected) {
			tab->isDirty = true;
			tab->isSelected = false;
			break;
		}
	}
	auto tab = std::make_shared<TitleBarTab>(root, std::wstring(L"最近使用的文件"));	
	tab->rect.setXYWH(12.f + tabs.size() * 200.f + tabs.size() * 3.f, 10.f, 200.f, 46.f);
	tab->historyNum = tabs.size();
	tabs.push_back(std::move(tab));	
	if (needRefresh) {
		InvalidateRect(root->hwnd, nullptr, false);
	}

}
