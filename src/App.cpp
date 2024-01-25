﻿#include "App.h"
#include "../res/res.h"
#include "include/core/SkFontStyle.h"
#include "include/core/SkData.h"
#include "include/core/SkGraphics.h"


HINSTANCE hinstance;
std::shared_ptr<SkFont> fontIcon{ nullptr };
std::shared_ptr<SkFont> fontText{ nullptr };

App::~App()
{
}

bool App::Init(HINSTANCE hins)
{
    hinstance = hins;
    SkGraphics::Init();
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Failed to initialize COM library.", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }
    initFontText();
    initFontIcon();
    return true;
}

void App::Dispose()
{
    CoUninitialize();
}

std::shared_ptr<SkFont> App::GetFontIcon()
{
	return fontIcon;
}

std::shared_ptr<SkFont> App::GetFontText()
{
	return fontText;
}



void App::Quit(const int& code) {
    PostQuitMessage(0);
}

void App::initFontIcon()
{
    HMODULE instance = GetModuleHandle(NULL);
    HRSRC resID = FindResource(instance, MAKEINTRESOURCE(IDR_ICON_FONT), L"ICON_FONT");
    if (resID == 0)
    {
        MessageBox(NULL, L"查找字体图标资源失败", L"系统提示", NULL);
        return;
    }
    size_t resSize = SizeofResource(instance, resID);
    HGLOBAL res = LoadResource(instance, resID);
    if (res == 0)
    {
        MessageBox(NULL, L"加载字体图标资源失败", L"系统提示", NULL);
        return;
    }
    LPVOID resData = LockResource(res);
    auto fontData = SkData::MakeWithoutCopy(resData, resSize);
    auto iconFace = SkTypeface::MakeFromData(fontData);
    fontIcon = std::make_shared<SkFont>(iconFace);
}

void App::initFontText()
{
	auto fontFace = SkTypeface::MakeFromName("Microsoft YaHei", SkFontStyle::Normal());
    fontText = std::make_shared<SkFont>(fontFace);
}

void App::Log(std::string&& info) {
    SkDebugf(info.data());
}