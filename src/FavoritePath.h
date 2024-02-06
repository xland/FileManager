#pragma once
#include "ControlBase.h"
#include "include/core/SkCanvas.h"

class WindowBase;
class FavoritePath : public ControlBase
{
public:
	FavoritePath(WindowBase* root);
	~FavoritePath();
	void paint(SkCanvas* canvas) override;
	void mouseMove(const int& x, const int& y);
	void mouseDown(const int& x, const int& y);
	void mouseUp(const int& x, const int& y);
	void mouseDrag(const int& x, const int& y);
	void resize(const int& w, const int& h);
	float y;
private:
	float totalHeight{0};
	SkRect rect;
	SkRect scrollerRect;
	bool hoverScroller{ false };
	double top{ 0.f };
	double downY{ 0.f };
};