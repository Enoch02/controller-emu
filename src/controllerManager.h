#pragma once
#define WIN32_LEAN_AND_MEAN
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <windows.h> // this needs to come before ViGEm
#include <ViGEm/Client.h>
#include <hidapi/hidapi.h>
#include <iomanip>
#include <filesystem>

#include "ControllerParser.h"

class ControllerManager
{
  private:
    PVIGEM_CLIENT vigem_client{};
    PVIGEM_TARGET target{};
    std::vector<hid_device *> connected_devices{};
    hid_device *selected_device{};
    bool is_running{true};
    ControllerParser parser{};

  public:
    ControllerManager();

    bool initialize();

    std::vector<hid_device_info *> detectControllers();

    bool selectDevice(const std::vector<hid_device_info *> &device_list, int selection);

    void readControllerInput();

    void stop();

    ~ControllerManager();

    const size_t report_size = 64; // default report size
};