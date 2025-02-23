#include "controllerManager.h"

ControllerManager::ControllerManager()
{
}

bool ControllerManager::initialize()
{
    // Initialize HID API
    if (hid_init() != 0)
    {
        std::wcerr << "Failed to initialize HID API" << std::endl;
        return false;
    }

    // Initialize ViGEm client
    vigem_client = vigem_alloc();
    if (vigem_client == nullptr)
    {
        std::wcerr << "Failed to allocate ViGEm client" << std::endl;
        return false;
    }

    const auto ret = vigem_connect(vigem_client);
    if (!VIGEM_SUCCESS(ret))
    {
        std::wcerr << "Failed to connect to ViGEm bus: 0x" << std::hex << ret << std::endl;
        return false;
    }

    // Allocate Xbox 360 controller
    target = vigem_target_x360_alloc();
    if (target == nullptr)
    {
        std::wcerr << "Failed to allocate Xbox 360 controller" << std::endl;
        return false;
    }

    // Add controller to ViGEm bus
    const auto plugin_result = vigem_target_add(vigem_client, target);
    if (!VIGEM_SUCCESS(plugin_result))
    {
        std::wcerr << "Failed to add target to ViGEm bus: 0x" << std::hex << plugin_result << std::endl;
        return false;
    }

    return true;
}

std::vector<hid_device_info *> ControllerManager::detectControllers()
{
    std::vector<hid_device_info *> device_list{};
    hid_device_info *devices = hid_enumerate(0x0, 0x0);
    hid_device_info *current = devices;
    int index{};

    while (current)
    {
        std::wcout << L"[" << index++
                   << L"] "
                   //<< L"  Vendor ID: 0x" << std::hex << current->vendor_id << std::endl
                   //<< L"  Product ID: 0x" << current->product_id << std::endl
                   << L"  Manufacturer: " << (current->manufacturer_string ? current->manufacturer_string : L"Unknown")
                   << std::endl
                   << L"  Product: " << (current->product_string ? current->product_string : L"Unknown")
                   << std::endl
                   //<< L"  Path: " << current->path << std::endl
                   << L"----------------------------------------" << std::endl;

        device_list.push_back(current);
        current = current->next;
    }

    return device_list;
}

bool ControllerManager::selectDevice(const std::vector<hid_device_info *> &device_list, int selection)
{
    if (selection < 0 || selection >= static_cast<int>(device_list.size()))
    {
        std::wcerr << L"Invalid selection" << std::endl;
        return false;
    }

    selected_device = hid_open_path(device_list[selection]->path);
    if (!selected_device)
    {
        std::wcerr << L"Failed to open device" << std::endl;
        return false;
    }

    std::wcout << L"Successfully opened device" << std::endl;
    return true;
}

void ControllerManager::readControllerInput()
{
    if (!selected_device)
    {
        std::wcerr << L"No device selected" << std::endl;
        return;
    }

    std::wcout << L"Reading input data. Press Ctrl+C to stop." << std::endl;

    // Store previous state to detect changes
    std::vector<unsigned char> previousBuffer(64, 0);
    std::vector<unsigned char> buffer(64, 0);

    while (is_running)
    {
        int result = hid_read_timeout(selected_device, buffer.data(), buffer.size(), 100);

        if (result > 0)
        {
            // Check if the current state is different from previous state
            bool stateChanged = false;
            for (int i = 0; i < result; i++)
            {
                if (buffer[i] != previousBuffer[i])
                {
                    stateChanged = true;
                    break;
                }
            }

            // Only print if state changed
            if (stateChanged)
            {
                auto now = std::chrono::system_clock::now();
                auto time = std::chrono::system_clock::to_time_t(now);
                std::wcout << L"Input changed at " << std::put_time(std::localtime(&time), L"%H:%M:%S")
                           << L" - Raw data: ";
                for (int i = 0; i < result; i++)
                {
                    if (buffer[i] != previousBuffer[i])
                    {
                        // Highlight changed bytes
                        std::wcout << L"[" << std::hex << std::setw(2) << std::setfill(L'0')
                                   << static_cast<int>(buffer[i]) << L"] ";
                    }
                    else
                    {
                        std::wcout << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<int>(buffer[i])
                                   << L" ";
                    }
                }
                std::wcout << "\n";
            }

            ControllerState state = parser.parseRawData(buffer);
            parser.updateXInputState(vigem_client, target, state);

            // Copy current state to previous state
            previousBuffer = buffer;
        }

        if (result == -1)
        {
            std::wcerr << "An error has occurred:\n" << hid_error(selected_device);
            stop();
        }
    }
}

void ControllerManager::stop()
{
    is_running = false;
}

ControllerManager::~ControllerManager()
{
    if (selected_device)
    {
        hid_close(selected_device);
    }

    if (target)
    {
        vigem_target_remove(vigem_client, target);
        vigem_target_free(target);
    }

    if (vigem_client)
    {
        vigem_disconnect(vigem_client);
        vigem_free(vigem_client);
    }

    hid_exit();
}