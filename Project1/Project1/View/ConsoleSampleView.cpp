#include "ConsoleSampleView.h"

int ConsoleSampleView::ReadMenuChoice()
{
    m_out << "\n메뉴를 선택하세요 ("
          << static_cast<int>(MenuOption::RegisterSample) << ": 시료 등록, "
          << static_cast<int>(MenuOption::ListSamples) << ": 목록 조회, "
          << static_cast<int>(MenuOption::Exit) << ": 종료): ";

    int choice = 0;
    m_in >> choice;
    m_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return choice;
}

Sample ConsoleSampleView::ReadNewSampleInput()
{
    std::string sampleId;
    std::string name;
    double avgProductionTime = 0.0;
    double yield = 0.0;
    int stock = 0;

    m_out << "SampleId: ";
    std::getline(m_in, sampleId);

    m_out << "Name: ";
    std::getline(m_in, name);

    m_out << "AvgProductionTime: ";
    m_in >> avgProductionTime;

    m_out << "Yield: ";
    m_in >> yield;

    m_out << "Stock: ";
    m_in >> stock;
    m_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return Sample(sampleId, name, avgProductionTime, yield, stock);
}

void ConsoleSampleView::ShowSampleList(const std::vector<Sample>& samples)
{
    m_out << "SampleId\tName\tAvgProductionTime\tYield\tStock\n";

    for (const auto& sample : samples)
    {
        m_out << sample.SampleId() << '\t'
              << sample.Name() << '\t'
              << sample.AvgProductionTime() << '\t'
              << sample.Yield() << '\t'
              << sample.Stock() << '\n';
    }
}

void ConsoleSampleView::ShowMessage(const std::string& message)
{
    m_out << message << '\n';
}
