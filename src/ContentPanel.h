#pragma once
#include <Windows.h>
#include <string>
#include <format>
#include <vector>
#include <chrono>
#include "include/core/SkCanvas.h"
#include "ControlBase.h"
#include "ContentHeader.h"
#include "ContentList.h"
#include "ContentBottom.h"

class WindowMain;
class TitleBarTab;
class ContentPanel : public ControlBase
{
public:
    ContentPanel(WindowMain* root);
    ~ContentPanel();
    void paint(SkCanvas* canvas) override;
    void resize(const int& w, const int& h);
    void tabChange(TitleBarTab* tab, TitleBarTab* tabNew);
    std::shared_ptr<ContentHeader> contentHeader;
    std::shared_ptr<ContentList> contentList;
    std::shared_ptr<ContentBottom> contentBottom;
private:
};