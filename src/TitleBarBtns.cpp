﻿#include "TitleBarBtns.h"
#include "WindowMain.h"
#include "App.h"
#include "include/core/SkFontMetrics.h"

TitleBarBtns::TitleBarBtns()
{
	YGNodeStyleSetFlexDirection(layout, YGFlexDirectionRow);
	YGNodeStyleSetHeight(layout, 60.f);
	YGNodeStyleSetWidth(layout, 198.f);
	
}

TitleBarBtns::~TitleBarBtns()
{
}

void TitleBarBtns::paint(SkCanvas *canvas)
{
	auto rect = getOffsetRect();
	SkPaint paint;
	paint.setColor(0xAAFFFFFF);
	if (hoverIndex == 0) {
		canvas->drawRect(SkRect::MakeXYWH(rect.fLeft, rect.fTop, 66, 60),paint);
	}
	else if (hoverIndex == 1) {
		canvas->drawRect(SkRect::MakeXYWH(rect.fLeft+66, rect.fTop, 66, 60), paint);
	}
	else if (hoverIndex == 2) {
		canvas->drawRect(SkRect::MakeXYWH(rect.fLeft + 132, rect.fTop, 66, 60), paint);
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
	canvas->drawString(iconCode, x1 + 132, y1, *font, paint);
}

void TitleBarBtns::mousemove(const int& x, const int& y)
{
	auto rect = getOffsetRect();
	if (!rect.contains(x, y)) {
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
	auto ptr = YGNodeGetOwner(layout);
}
