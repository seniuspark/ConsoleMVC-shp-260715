#include "SampleController.h"

void SampleController::RunOnce()
{
    int choice = m_view.ReadMenuChoice();

    switch (static_cast<MenuOption>(choice))
    {
    case MenuOption::Exit:
        m_running = false;
        break;
    case MenuOption::RegisterSample:
        RegisterSample();
        break;
    case MenuOption::ListSamples:
        ListSamples();
        break;
    default:
        m_view.ShowMessage("알 수 없는 메뉴 선택입니다: " + std::to_string(choice));
        break;
    }
}

void SampleController::Run()
{
    while (m_running)
    {
        RunOnce();
    }
}

void SampleController::RegisterSample()
{
    Sample sample = m_view.ReadNewSampleInput();
    std::string sampleId = sample.SampleId();

    bool added = m_repository.Add(std::move(sample));

    if (added)
    {
        m_view.ShowMessage("시료가 등록되었습니다: " + sampleId);
    }
    else
    {
        m_view.ShowMessage("이미 존재하는 시료 ID입니다: " + sampleId);
    }
}

void SampleController::ListSamples()
{
    m_view.ShowSampleList(m_repository.GetAll());
}
