#pragma once

#include <string>
#include <vector>

#include "Model/Sample.h"

// Controller와 View가 공유하는 메뉴 번호. 매직 넘버가 양쪽에 흩어지는 것을
// 막기 위해 Controller/View 경계 인터페이스에 둔다.
enum class MenuOption
{
    Exit = 0,
    RegisterSample = 1,
    ListSamples = 2
};

class ISampleView
{
public:
    virtual ~ISampleView() = default;

    virtual int ReadMenuChoice() = 0;
    virtual Sample ReadNewSampleInput() = 0;
    virtual void ShowSampleList(const std::vector<Sample>& samples) = 0;
    virtual void ShowMessage(const std::string& message) = 0;
};
