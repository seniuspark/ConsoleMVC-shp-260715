#pragma once

#include <string>
#include <vector>

#include "Model/Sample.h"

class ISampleView
{
public:
    virtual ~ISampleView() = default;

    virtual int ReadMenuChoice() = 0;
    virtual Sample ReadNewSampleInput() = 0;
    virtual void ShowSampleList(const std::vector<Sample>& samples) = 0;
    virtual void ShowMessage(const std::string& message) = 0;
};
