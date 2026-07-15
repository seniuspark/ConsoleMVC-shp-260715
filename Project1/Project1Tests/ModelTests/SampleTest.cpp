#include "gtest/gtest.h"
#include "Model/Sample.h"

namespace ModelTests
{
    TEST(SampleTest, ConstructsWithGivenFields)
    {
        Sample sample("S-001", "Wafer-A", 12.5, 0.95, 100);

        EXPECT_EQ(sample.SampleId(), "S-001");
        EXPECT_EQ(sample.Name(), "Wafer-A");
        EXPECT_DOUBLE_EQ(sample.AvgProductionTime(), 12.5);
        EXPECT_DOUBLE_EQ(sample.Yield(), 0.95);
        EXPECT_EQ(sample.Stock(), 100);
    }
}
