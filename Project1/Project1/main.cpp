#include <iostream>

#include "Controller/SampleController.h"
#include "Model/SampleRepository.h"
#include "View/ConsoleSampleView.h"

int main()
{
    SampleRepository repository;
    ConsoleSampleView view(std::cin, std::cout);
    SampleController controller(repository, view);

    controller.Run();

    return 0;
}
