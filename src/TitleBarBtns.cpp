﻿#include "TitleBarBtns.h"
#include <Windows.h>
#include <functional>
#include "include/core/SkFontMetrics.h"
#include "App.h"
#include "WindowBase.h"


TitleBarBtns::TitleBarBtns(WindowBase* root):Layout(root)
{
	YGNodeStyleSetFlexDirection(layout, YGFlexDirectionRow);
	YGNodeStyleSetWidth(layout, 198.f);
	root->mouseMoveHandlers.push_back(
		std::bind(&TitleBarBtns::mouseMove, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseDownHandlers.push_back(
		std::bind(&TitleBarBtns::mouseDown, this, std::placeholders::_1, std::placeholders::_2)
	);
}

TitleBarBtns::~TitleBarBtns()
{
}

void TitleBarBtns::paint(SkCanvas *canvas)
{
	auto h = YGNodeLayoutGetHeight(layout);
	auto rect = getOffsetRect();
	SkPaint paint;
	if (hoverIndex == 0) {
		paint.setColor(0xAAFFFFFF);
		canvas->drawRect(SkRect::MakeXYWH(rect.fLeft, rect.fTop, 66, h),paint);
	}
	else if (hoverIndex == 1) {
		paint.setColor(0xAAFFFFFF);
		canvas->drawRect(SkRect::MakeXYWH(rect.fLeft+66, rect.fTop, 66, h), paint);
	}
	else if (hoverIndex == 2) {
		paint.setColor(SK_ColorRED);
		canvas->drawRect(SkRect::MakeXYWH(rect.fLeft + 132, rect.fTop, 66, h), paint);
	}
	rect.fRight = rect.fLeft + 66;
	auto font = App::GetFontIcon();
	font->setSize(18.f);
	SkFontMetrics metrics;
	font->getMetrics(&metrics);
	auto iconCode = (const char *)u8"\ue6e8"; //最小化
	SkRect textBounds;
	font->measureText(iconCode, 3, SkTextEncoding::kUTF8, &textBounds);
	SkScalar x1 = rect.centerX() - textBounds.width() / 2;
	SkScalar y1 = rect.centerY() + (metrics.fDescent - metrics.fAscent) / 2 - metrics.fDescent;
	
	paint.setColor(0xAA000000);
	canvas->drawString(iconCode, x1, y1, *font, paint);
	iconCode = (const char*)u8"\ue6e5"; //最大化
	canvas->drawString(iconCode, x1+66, y1, *font, paint);
	iconCode = (const char*)u8"\ue6e7"; // 关闭
	if (hoverIndex == 2) {
		paint.setColor(SK_ColorWHITE);
	}
	canvas->drawString(iconCode, x1 + 132, y1, *font, paint);
}

void TitleBarBtns::mouseDown(const int& x, const int& y)
{
	if (hoverIndex == 0) {

	}
	else if (hoverIndex == 1) {

	}
	else if (hoverIndex == 2) {
		PostMessage(root->hwnd, WM_CLOSE, 0, 0); //PostMessage异步，SendMessage同步
	}
}

void TitleBarBtns::mouseMove(const int& x, const int& y)
{
	auto rect = getOffsetRect();
	if (!rect.contains(x, y)) {
		if (hoverIndex >= 0) {
			hoverIndex = -1;
			POINT mousePoint;
			GetCursorPos(&mousePoint);
			HWND hwnd = WindowFromPoint(mousePoint);
			InvalidateRect(hwnd, nullptr, false);
		}
		return;
	}
	if (x < rect.fLeft + 66) {
		hoverIndex = 0;
	}
	else if (x < rect.fLeft + 132) {
		hoverIndex = 1;
	}
	else {
		hoverIndex = 2;
	}
	InvalidateRect(root->hwnd, nullptr, false);
}