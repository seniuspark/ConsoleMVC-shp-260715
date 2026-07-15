#pragma once

#include <queue>
#include <string>
#include <vector>

#include "Model/Sample.h"
#include "View/ISampleView.h"

class FakeSampleView : public ISampleView
{
public:
    void EnqueueMenuChoice(int choice) { m_menuChoices.push(choice); }
    void EnqueueSampleInput(Sample sample) { m_sampleInputs.push(std::move(sample)); }

    int ReadMenuChoice() override
    {
        int choice = m_menuChoices.front();
        m_menuChoices.pop();
        return choice;
    }

    Sample ReadNewSampleInput() override
    {
        Sample sample = m_sampleInputs.front();
        m_sampleInputs.pop();
        return sample;
    }

    void ShowSampleList(const std::vector<Sample>& samples) override
    {
        m_lastShownSamples = samples;
    }

    void ShowMessage(const std::string& message) override
    {
        m_lastMessage = message;
    }

    const std::vector<Sample>& LastShownSamples() const { return m_lastShownSamples; }
    const std::string& LastMessage() const { return m_lastMessage; }

private:
    std::queue<int> m_menuChoices;
    std::queue<Sample> m_sampleInputs;
    std::vector<Sample> m_lastShownSamples;
    std::string m_lastMessage;
};
