#pragma once
#include "ControlBase.h"
#include "include/core/SkCanvas.h"
#include <Windows.h>
class WindowMain;
class QuickBtn : public ControlBase
{
public:
	QuickBtn(WindowMain* root);
	~QuickBtn();
	void paint(SkCanvas* canvas) override;
private:
	void resize(const int& w, const int& h) override;
	void mouseMove(const int& x, const int& y) override;
	void mouseDown(const int& x, const int& y) override;
	int hoverIndex{ -1 };
	std::wstring getKnownFolderPath(GUID id);
};