#include "FavoritePath.h"
#include <string>
#include <format>
#include <Windows.h>
#include <include/core/SkPaint.h>

#include "WindowMain.h"
#include "SystemIcon.h"
#include "App.h"
#include "LeftPanel.h"

FavoritePath::FavoritePath(WindowMain* root) :ControlBase(root)
{
	totalHeight = 46 * 26;

	root->mouseMoveHandlers.push_back(
		std::bind(&FavoritePath::mouseMove, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseDownHandlers.push_back(
		std::bind(&FavoritePath::mouseDown, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseDragHandlers.push_back(
		std::bind(&FavoritePath::mouseDrag, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseUpHandlers.push_back(
		std::bind(&FavoritePath::mouseUp, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->resizeHandlers.push_back(
		std::bind(&FavoritePath::resize, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseWheelHandlers.push_back(
		std::bind(&FavoritePath::mouseWheel, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3)
	);
}

FavoritePath::~FavoritePath()
{
}

void FavoritePath::paint(SkCanvas* canvas)
{
	auto rectHeight = rect.height();
	SkPaint paint;
	canvas->save();
	canvas->clipRect(rect);
	auto top = 0 - (scrollerRect.fTop-rect.fTop) / rectHeight * totalHeight;
	for (size_t i = 0; i < 26; i++)
	{
		auto img = SystemIcon::getIcon(SIID_FOLDER, 26); //CSIDL_QUICKACCESS		
		canvas->drawImage(img, 12, rect.fTop + top+ i*46 + 8);
		std::wstring str = std::format(L"这是一条收藏的路径({}:)", i);
		auto textLength = wcslen(str.data()) * 2;
		auto fontText = App::GetFontText();
		fontText->setSize(16.6f);
		paint.setColor(0xFF333333);
		canvas->drawSimpleText(str.data(), textLength, SkTextEncoding::kUTF16, 42, rect.fTop + top+ i * 46 + 26, *fontText, paint);
	}

	if (totalHeight > rectHeight) {
		if (hoverScroller) {
			paint.setColor(0xFFD3E3FD);
		}
		else {
			paint.setColor(0x11333333);
		}		
		canvas->drawRoundRect(scrollerRect,3,3, paint);
	}
	canvas->restore();
}

void FavoritePath::mouseMove(const int& x, const int& y)
{
	bool flag{ false };
	if (scrollerRect.contains(x, y)) {
		flag = true;
	}
	if (flag != hoverScroller) {
		hoverScroller = flag;
		InvalidateRect(root->hwnd, nullptr, false);
	}
}

void FavoritePath::mouseDown(const int& x, const int& y)
{
	downY = y;
}

void FavoritePath::mouseUp(const int& x, const int& y)
{
	if (!scrollerRect.contains(x,y)) {
		if (hoverScroller) {
			hoverScroller = false;
		}
		InvalidateRect(root->hwnd, nullptr, false);
	}
}

void FavoritePath::mouseDrag(const int& x, const int& y)
{
	if (hoverScroller) {
		float span = y - downY;
		if (span > 0) {
			if (scrollerRect.fBottom < rect.fBottom) {
				auto v = std::min(scrollerRect.fBottom + span, rect.fBottom);
				scrollerRect.offsetTo(rect.fRight - 8, scrollerRect.fTop + v- scrollerRect.fBottom);
				InvalidateRect(root->hwnd, nullptr, false);
			}
		}
		else {
			if (scrollerRect.fTop > rect.fTop) {
				auto v = std::max(scrollerRect.fTop + span, rect.fTop);
				scrollerRect.offsetTo(rect.fRight - 8, v);
				InvalidateRect(root->hwnd, nullptr, false);
			}
		}		
		downY = y;
	}
}

void FavoritePath::mouseWheel(const int& x, const int& y,const int& delta)
{
	if (rect.contains(x, y)) {
		auto span = 12.f;
		if (delta > 0) {
			if (scrollerRect.fTop > rect.fTop) {
				auto v = std::max(scrollerRect.fTop - span, rect.fTop);
				scrollerRect.offsetTo(rect.fRight - 8, v);
				InvalidateRect(root->hwnd, nullptr, false);
			}
		}
		else {
			if (scrollerRect.fBottom < rect.fBottom) {
				auto v = std::min(scrollerRect.fBottom + span, rect.fBottom);
				scrollerRect.offsetTo(rect.fRight - 8, scrollerRect.fTop + v - scrollerRect.fBottom);
				InvalidateRect(root->hwnd, nullptr, false);
			}
		}
	}	
}

void FavoritePath::resize(const int& w, const int& h)
{
	rect.setXYWH(0, y, root->leftPanel->rect.width(), h - y - 50);
	if (totalHeight > rect.height()) {
		auto h = rect.height() * (rect.height() / totalHeight);
		if (h < 40.f) h = 40.f;
		scrollerRect.setXYWH(rect.fRight - 8, rect.fTop, 8, h);
	}
}
