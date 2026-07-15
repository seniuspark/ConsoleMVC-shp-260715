#include "SampleRepository.h"

#include <algorithm>

bool SampleRepository::Add(Sample sample)
{
    bool alreadyExists = std::any_of(m_samples.begin(), m_samples.end(),
        [&sample](const Sample& existing) { return existing.SampleId() == sample.SampleId(); });

    if (alreadyExists)
    {
        return false;
    }

    m_samples.push_back(std::move(sample));
    return true;
}

const std::vector<Sample>& SampleRepository::GetAll() const
{
    return m_samples;
}

std::optional<Sample> SampleRepository::FindById(const std::string& id) const
{
    auto it = std::find_if(m_samples.begin(), m_samples.end(),
        [&id](const Sample& existing) { return existing.SampleId() == id; });

    if (it == m_samples.end())
    {
        return std::nullopt;
    }

    return *it;
}

std::size_t SampleRepository::Count() const
{
    return m_samples.size();
}
