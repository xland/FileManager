#pragma once
#include <Windows.h>
#include <string>
#include <format>
#include <vector>
#include <include/core/SkCanvas.h>
#include "ControlBase.h"

class WindowMain;
class ContentBottom : public ControlBase
{
public:
    ContentBottom(WindowMain *root);
    ~ContentBottom();
    void resize(const int& w, const int& h) override;
private:
    void paint(SkCanvas* canvas) override;
};