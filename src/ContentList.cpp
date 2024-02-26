﻿#include "ContentList.h"

#include <Windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <chrono>

#include "App.h"
#include "WindowMain.h"
#include "ContentPanel.h"
#include "FileColumnTime.h"

ContentList::ContentList(WindowMain* root) :ControlBase(root)
{
	getRecentFiles();
}

ContentList::~ContentList()
{
}

void ContentList::paint(SkCanvas* canvas)
{
	if (!needPaint(canvas)) return;
	SkPaint paint;
	auto parent = (ContentPanel*)root->contentPanel.get();
	auto paddingLeft = parent->contentHeader->paddingLeft;
	auto paddingRight = parent->contentHeader->paddingRight;
	auto columns = parent->contentHeader->columns;
	auto fontText = App::GetFontText();
	fontText->setSize(16.8);
	canvas->save();
	canvas->clipRect(rect);
	paint.setColor(0xFF555555);
	auto top = 0 - (scrollerRect.fTop - rect.fTop) / rect.height() * totalHeight;
	auto y = top + rect.fTop + 32.f;
	//if (y > 195.f)y = 195.f; // magic num

	for (auto& file : files)
	{
		for (size_t i = 0; i < columns.size(); i++)
		{
			canvas->save();
			canvas->clipRect(SkRect::MakeLTRB(columns[i].left + paddingLeft, y - 20, columns[i].right - paddingRight, y + 20));
			auto len = wcslen(file[i].text.data()) * 2;
			canvas->drawSimpleText(file[i].text.data(), len, SkTextEncoding::kUTF16,
				columns[i].left + paddingLeft, y, *fontText, paint);
			canvas->restore();
		}
		y += 40;
	}
	if (totalHeight > rect.height()) {
		if (hoverScroller) {
			paint.setColor(0xFFD3E3FD);
		}
		else {
			paint.setColor(0x11333333);
		}
		canvas->drawRoundRect(scrollerRect, 3, 3, paint);
	}
	canvas->restore();
	paint.setColor(0xFFE8E8E8);
	canvas->drawLine(rect.fLeft, rect.fBottom, rect.fRight, rect.fBottom, paint);
}

void ContentList::mouseMove(const int& x, const int& y)
{
	bool flag{ false };
	if (scrollerRect.contains(x, y)) {
		flag = true;
	}
	if (flag != hoverScroller) {
		hoverScroller = flag;
		repaint();
	}
}

void ContentList::mouseDown(const int& x, const int& y)
{
	downY = y;
}

void ContentList::mouseUp(const int& x, const int& y)
{
	if (!scrollerRect.contains(x, y)) {
		if (hoverScroller) {
			hoverScroller = false;
			repaint();
		}
	}
}

void ContentList::mouseDrag(const int& x, const int& y)
{
	if (hoverScroller) {
		float span = y - downY;
		if (span > 0) {
			if (scrollerRect.fBottom < rect.fBottom) {
				auto v = std::min(scrollerRect.fBottom + span, rect.fBottom);
				scrollerRect.offsetTo(rect.fRight - 16, scrollerRect.fTop + v - scrollerRect.fBottom);
				repaint();
			}
		}
		else {
			if (scrollerRect.fTop > rect.fTop) {
				auto v = std::max(scrollerRect.fTop + span, rect.fTop);
				scrollerRect.offsetTo(rect.fRight - 16, v);
				repaint();
			}
		}
		downY = y;
	}
}

void ContentList::resize(const int& w, const int& h)
{
	auto parent = (ContentPanel*)root->contentPanel.get();
	rect.setLTRB(root->contentPanel->rect.fLeft+1,
		parent->contentHeader->rect.fBottom+1,
		parent->rect.fRight,
		parent->contentBottom->rect.fTop
	);
	if (totalHeight > rect.height()) {
		auto h = rect.height() * (rect.height() / totalHeight);
		if (h < 40.f) h = 40.f;
		scrollerRect.setXYWH(rect.fRight - 16, rect.fTop, 16, h);
	}
}

void ContentList::mouseWheel(const int& x, const int& y, const int& delta)
{
	if (totalHeight <= rect.height()) {
		return;
	}
	if (!rect.contains(x, y)) {
		return;
	}
	auto span = 12.f;
	if (delta > 0) {
		if (scrollerRect.fTop > rect.fTop) {
			auto v = std::max(scrollerRect.fTop - span, rect.fTop);
			scrollerRect.offsetTo(rect.fRight - 16, v);
			repaint();
		}
	}
	else {
		if (scrollerRect.fBottom < rect.fBottom) {
			auto v = std::min(scrollerRect.fBottom + span, rect.fBottom);
			scrollerRect.offsetTo(rect.fRight - 16, scrollerRect.fTop + v - scrollerRect.fBottom);
			repaint();
		}
	}
}

void ContentList::getRecentFiles()
{
	PWSTR pszPath = nullptr;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Recent, 0, NULL, &pszPath);
	std::wstring pathStr(pszPath);
	CoTaskMemFree(pszPath);

	auto zone = std::chrono::current_zone();
	std::filesystem::path path(pathStr);
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		auto fileName = entry.path().stem().wstring();
		auto fileTime = entry.last_write_time();
		auto sysClock = std::chrono::clock_cast<std::chrono::system_clock>(entry.last_write_time());
		auto zTime = std::chrono::zoned_time(zone, sysClock);
		auto str = std::format(L"{:%Y-%m-%d %H:%M:%S}", zTime);
		str = str.substr(0, str.find_last_of(L"."));
		files.push_back({ FileColumn(fileName), FileColumnTime(str,fileTime)});
	}
	totalHeight = 40 * files.size();
	std::sort(files.begin(), files.end(), [](const auto& a, const auto& b) {
		return a[1] > b[1];
		});
}