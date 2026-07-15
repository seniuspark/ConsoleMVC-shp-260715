#include "gtest/gtest.h"
#include "Model/Sample.h"
#include "View/FakeSampleView.h"

namespace ViewTests
{
    TEST(FakeSampleViewTest, ReturnsQueuedMenuChoicesInOrder)
    {
        FakeSampleView view;
        view.EnqueueMenuChoice(1);
        view.EnqueueMenuChoice(2);

        EXPECT_EQ(view.ReadMenuChoice(), 1);
        EXPECT_EQ(view.ReadMenuChoice(), 2);
    }

    TEST(FakeSampleViewTest, RecordsShownSamples)
    {
        FakeSampleView view;
        std::vector<Sample> samples{
            Sample("S-001", "Wafer-A", 12.5, 0.95, 100)
        };

        view.ShowSampleList(samples);

        const auto& shown = view.LastShownSamples();
        ASSERT_EQ(shown.size(), 1u);
        EXPECT_EQ(shown[0].SampleId(), "S-001");
    }

    TEST(FakeSampleViewTest, RecordsShownMessage)
    {
        FakeSampleView view;

        view.ShowMessage("registered");

        EXPECT_EQ(view.LastMessage(), "registered");
    }
}
