#include "gtest/gtest.h"
#include "Model/Sample.h"
#include "Model/SampleRepository.h"
#include "View/FakeSampleView.h"
#include "Controller/SampleController.h"

namespace ControllerTests
{
    TEST(SampleControllerTest, RegisterSampleAddsToRepository)
    {
        SampleRepository repository;
        FakeSampleView view;
        view.EnqueueMenuChoice(1);
        view.EnqueueSampleInput(Sample("S-001", "Wafer-A", 12.5, 0.95, 100));
        SampleController controller(repository, view);

        controller.RunOnce();

        auto found = repository.FindById("S-001");
        ASSERT_TRUE(found.has_value());
        EXPECT_EQ(found->Name(), "Wafer-A");
    }

    TEST(SampleControllerTest, RegisterSampleShowsSuccessMessage)
    {
        SampleRepository repository;
        FakeSampleView view;
        view.EnqueueMenuChoice(1);
        view.EnqueueSampleInput(Sample("S-001", "Wafer-A", 12.5, 0.95, 100));
        SampleController controller(repository, view);

        controller.RunOnce();

        EXPECT_NE(view.LastMessage().find("S-001"), std::string::npos);
    }

    TEST(SampleControllerTest, ListSamplesCallsShowSampleListWithAllSamples)
    {
        SampleRepository repository;
        repository.Add(Sample("S-001", "Wafer-A", 12.5, 0.95, 100));
        repository.Add(Sample("S-002", "Wafer-B", 5.0, 0.8, 50));
        FakeSampleView view;
        view.EnqueueMenuChoice(2);
        SampleController controller(repository, view);

        controller.RunOnce();

        const auto& shown = view.LastShownSamples();
        ASSERT_EQ(shown.size(), 2u);
        EXPECT_EQ(shown[0].SampleId(), "S-001");
        EXPECT_EQ(shown[1].SampleId(), "S-002");
    }

    TEST(SampleControllerTest, ListSamplesWhenEmptyShowsEmptyList)
    {
        SampleRepository repository;
        FakeSampleView view;
        view.EnqueueMenuChoice(2);
        SampleController controller(repository, view);

        controller.RunOnce();

        EXPECT_TRUE(view.LastShownSamples().empty());
    }

    TEST(SampleControllerTest, UnknownMenuChoiceShowsErrorMessage)
    {
        SampleRepository repository;
        FakeSampleView view;
        view.EnqueueMenuChoice(99);
        SampleController controller(repository, view);

        controller.RunOnce();

        EXPECT_FALSE(view.LastMessage().empty());
        EXPECT_TRUE(controller.IsRunning());
    }

    TEST(SampleControllerTest, ExitChoiceStopsLoop)
    {
        SampleRepository repository;
        FakeSampleView view;
        view.EnqueueMenuChoice(0);
        SampleController controller(repository, view);

        controller.RunOnce();

        EXPECT_FALSE(controller.IsRunning());
    }
}
