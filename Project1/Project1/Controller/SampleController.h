#pragma once

#include "Model/SampleRepository.h"
#include "View/ISampleView.h"

class SampleController
{
public:
    SampleController(SampleRepository& repository, ISampleView& view)
        : m_repository(repository)
        , m_view(view)
    {
    }

    void RunOnce();
    void Run();
    bool IsRunning() const { return m_running; }

private:
    void RegisterSample();
    void ListSamples();

    SampleRepository& m_repository;
    ISampleView& m_view;
    bool m_running = true;
};
