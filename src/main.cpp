#include "controllerManager.h"

int main()
{
    ControllerManager manager;

    if (!manager.initialize())
    {
        std::wcerr << "Failed to initialize controller manager" << std::endl;
        return 1;
    }

    auto device_list = manager.detectControllers();

    if (device_list.empty())
    {
        std::wcerr << "No controllers found" << std::endl;
        return 1;
    }

    int selection{};

    // input validation loop
    for (;;)
    {
        std::wcout << L"Select a device to use: ";

        if (std::cin >> selection)
        {
            if (manager.selectDevice(device_list, selection))
            {
                break;
            }
        }
        else
        {
            std::wcout << L"Enter a valid integer" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    manager.readControllerInput();

    return 0;
}