#pragma once

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Model/Sample.h"
#include "View/ISampleView.h"

class ConsoleSampleView : public ISampleView
{
public:
    ConsoleSampleView(std::istream& in = std::cin, std::ostream& out = std::cout)
        : m_in(in)
        , m_out(out)
    {
    }

    int ReadMenuChoice() override;
    Sample ReadNewSampleInput() override;
    void ShowSampleList(const std::vector<Sample>& samples) override;
    void ShowMessage(const std::string& message) override;

private:
    std::istream& m_in;
    std::ostream& m_out;
};
