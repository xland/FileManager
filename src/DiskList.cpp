#include "DiskList.h"
#include "WindowBase.h"
#include "SystemIcon.h"
#include "App.h"

DiskList::DiskList(WindowBase* root) :ControlBase(root)
{
	DWORD drives = GetLogicalDrives();
	for (char drive = 'A'; drive <= 'Z'; ++drive) {
		if (drives & 1 << (drive - 'A')) {
			ULARGE_INTEGER total, avail;
			auto nameStr = std::format(L"{}:\\", drive);
			if (GetDiskFreeSpaceEx(nameStr.c_str(), &avail, &total, nullptr))
			{
				double total_space = static_cast<double>(total.QuadPart);
				double free_space = static_cast<double>(avail.QuadPart);
				bool isDisk = GetDriveType(nameStr.c_str()) == DRIVE_FIXED;
				driveInfo.push_back({ drive,total_space,free_space,isDisk });
			}
		}
	}
	root->resizeHandlers.push_back(
		std::bind(&DiskList::resize, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseMoveHandlers.push_back(
		std::bind(&DiskList::mouseMove, this, std::placeholders::_1, std::placeholders::_2)
	);
	root->mouseDownHandlers.push_back(
		std::bind(&DiskList::mouseDown, this, std::placeholders::_1, std::placeholders::_2)
	);
}

DiskList::~DiskList()
{
}

void DiskList::paint(SkCanvas* canvas)
{
	SkPaint paint;
	SkRect itemRect = SkRect::MakeXYWH(12.f, y, rect.width() - 24.f, 46.f);
	for (size_t i = 0; i < driveInfo.size(); i++)
	{
		auto str1 = std::format(L"{}:\\", std::get<0>(driveInfo[i]));
		auto img = SystemIcon::getIcon(str1, 24);
		paint.setColor(0x101677ff);
		canvas->drawRoundRect(itemRect, 6, 6, paint);
		canvas->drawImage(img, itemRect.fLeft + 12, itemRect.fTop + 9.f);
		std::wstring str = std::format(L"���ش���({}:)", std::get<0>(driveInfo[i]));
		auto textLength = wcslen(str.data()) * 2;
		auto fontText = App::GetFontText();
		fontText->setSize(16.6f);
		paint.setColor(0xFF333333);
		canvas->drawSimpleText(str.data(), textLength, SkTextEncoding::kUTF16,
			itemRect.fLeft + 48.f, itemRect.fTop + 28.f, *fontText, paint);
		itemRect.setXYWH(12.f, itemRect.fBottom + 8.f, rect.width() - 24.f, 46.f);
	}
}

void DiskList::resize(const int& w, const int& h)
{
	auto h1 = driveInfo.size() * (46 + 8) + 60 + 16;
	rect.setXYWH(0.f, y, 380.f, h1);
}

void DiskList::mouseMove(const int& x, const int& y)
{
}

void DiskList::mouseDown(const int& x, const int& y)
{
}
