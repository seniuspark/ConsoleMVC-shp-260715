#include "gtest/gtest.h"
#include "Model/Sample.h"
#include "Model/SampleRepository.h"

namespace ModelTests
{
    TEST(SampleRepositoryTest, AddIncreasesCount)
    {
        SampleRepository repository;

        repository.Add(Sample("S-001", "Wafer-A", 12.5, 0.95, 100));

        EXPECT_EQ(repository.Count(), 1u);
    }

    TEST(SampleRepositoryTest, AddDuplicateIdReturnsFalse)
    {
        SampleRepository repository;
        repository.Add(Sample("S-001", "Wafer-A", 12.5, 0.95, 100));

        bool added = repository.Add(Sample("S-001", "Wafer-B", 5.0, 0.8, 50));

        EXPECT_FALSE(added);
        EXPECT_EQ(repository.Count(), 1u);
    }

    TEST(SampleRepositoryTest, GetAllReturnsInsertedSamplesInOrder)
    {
        SampleRepository repository;
        repository.Add(Sample("S-001", "Wafer-A", 12.5, 0.95, 100));
        repository.Add(Sample("S-002", "Wafer-B", 5.0, 0.8, 50));

        const auto& all = repository.GetAll();

        ASSERT_EQ(all.size(), 2u);
        EXPECT_EQ(all[0].SampleId(), "S-001");
        EXPECT_EQ(all[1].SampleId(), "S-002");
    }

    TEST(SampleRepositoryTest, FindByIdReturnsNulloptWhenMissing)
    {
        SampleRepository repository;

        auto found = repository.FindById("S-999");

        EXPECT_FALSE(found.has_value());
    }

    TEST(SampleRepositoryTest, FindByIdReturnsSampleWhenPresent)
    {
        SampleRepository repository;
        repository.Add(Sample("S-001", "Wafer-A", 12.5, 0.95, 100));

        auto found = repository.FindById("S-001");

        ASSERT_TRUE(found.has_value());
        EXPECT_EQ(found->Name(), "Wafer-A");
    }
}
