#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Sample.h"

class SampleRepository
{
public:
    bool Add(Sample sample);
    const std::vector<Sample>& GetAll() const;
    std::optional<Sample> FindById(const std::string& id) const;
    std::size_t Count() const;

private:
    std::vector<Sample> m_samples;
};
