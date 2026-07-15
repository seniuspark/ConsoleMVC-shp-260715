#pragma once

#include <string>

class Sample
{
public:
    Sample(std::string sampleId, std::string name, double avgProductionTime, double yield, int stock)
        : m_sampleId(std::move(sampleId))
        , m_name(std::move(name))
        , m_avgProductionTime(avgProductionTime)
        , m_yield(yield)
        , m_stock(stock)
    {
    }

    const std::string& SampleId() const { return m_sampleId; }
    const std::string& Name() const { return m_name; }
    double AvgProductionTime() const { return m_avgProductionTime; }
    double Yield() const { return m_yield; }
    int Stock() const { return m_stock; }

private:
    std::string m_sampleId;
    std::string m_name;
    double m_avgProductionTime;
    double m_yield;
    int m_stock;
};
