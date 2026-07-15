#include "gtest/gtest.h"

#include <sstream>

#include "Model/Sample.h"
#include "View/ConsoleSampleView.h"

namespace ViewTests
{
    TEST(ConsoleSampleViewTest, ShowSampleListFormatsHeaderAndRows)
    {
        std::istringstream in;
        std::ostringstream out;
        ConsoleSampleView view(in, out);

        std::vector<Sample> samples{
            Sample("S-001", "Wafer-A", 12.5, 0.95, 100),
            Sample("S-002", "Wafer-B", 5.0, 0.8, 50)
        };

        view.ShowSampleList(samples);

        std::string output = out.str();
        EXPECT_NE(output.find("SampleId"), std::string::npos);
        EXPECT_NE(output.find("S-001"), std::string::npos);
        EXPECT_NE(output.find("Wafer-A"), std::string::npos);
        EXPECT_NE(output.find("S-002"), std::string::npos);
        EXPECT_NE(output.find("Wafer-B"), std::string::npos);
    }

    TEST(ConsoleSampleViewTest, ReadMenuChoiceParsesIntegerFromInputStream)
    {
        std::istringstream in("2\n");
        std::ostringstream out;
        ConsoleSampleView view(in, out);

        int choice = view.ReadMenuChoice();

        EXPECT_EQ(choice, 2);
    }

    TEST(ConsoleSampleViewTest, ReadNewSampleInputParsesAllFields)
    {
        std::istringstream in("S-010\nWafer-Z\n7.5\n0.9\n30\n");
        std::ostringstream out;
        ConsoleSampleView view(in, out);

        Sample sample = view.ReadNewSampleInput();

        EXPECT_EQ(sample.SampleId(), "S-010");
        EXPECT_EQ(sample.Name(), "Wafer-Z");
        EXPECT_DOUBLE_EQ(sample.AvgProductionTime(), 7.5);
        EXPECT_DOUBLE_EQ(sample.Yield(), 0.9);
        EXPECT_EQ(sample.Stock(), 30);
    }

    TEST(ConsoleSampleViewTest, ShowMessageWritesMessageToOutputStream)
    {
        std::istringstream in;
        std::ostringstream out;
        ConsoleSampleView view(in, out);

        view.ShowMessage("hello world");

        EXPECT_NE(out.str().find("hello world"), std::string::npos);
    }
}
