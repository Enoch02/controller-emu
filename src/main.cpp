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

    std::wcout << L"Select a device to use: ";
    int selection{};
    std::cin >> selection;

    if (!manager.selectDevice(device_list, selection))
    {
        return 1;
    }

    manager.readControllerInput();

    return 0;
}