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

#include "controllerManager.h"

// class ControllerManager {
// private:
//   PVIGEM_CLIENT vigem_client;
//   PVIGEM_TARGET target;
//   std::vector<hid_device *> connected_devices;
//   hid_device *selected_device;
//   bool is_running;

// public:
//   ControllerManager() : vigem_client(nullptr), target(nullptr) {}

//   bool initialize() {
//     // Initialize HID API
//     if (hid_init() != 0) {
//       std::wcerr << "Failed to initialize HID API" << std::endl;
//       return false;
//     }

//     // Initialize ViGEm client
//     vigem_client = vigem_alloc();
//     if (vigem_client == nullptr) {
//       std::wcerr << "Failed to allocate ViGEm client" << std::endl;
//       return false;
//     }

//     const auto ret = vigem_connect(vigem_client);
//     if (!VIGEM_SUCCESS(ret)) {
//       std::wcerr << "Failed to connect to ViGEm bus: 0x" << std::hex << ret
//                  << std::endl;
//       return false;
//     }

//     // Allocate Xbox 360 controller
//     target = vigem_target_x360_alloc();
//     if (target == nullptr) {
//       std::wcerr << "Failed to allocate Xbox 360 controller" << std::endl;
//       return false;
//     }

//     // Add controller to ViGEm bus
//     const auto plugin_result = vigem_target_add(vigem_client, target);
//     if (!VIGEM_SUCCESS(plugin_result)) {
//       std::wcerr << "Failed to add target to ViGEm bus: 0x" << std::hex
//                  << plugin_result << std::endl;
//       return false;
//     }

//     return true;
//   }

//   std::vector<hid_device_info *> detectControllers() {
//     std::vector<hid_device_info *> device_list;
//     struct hid_device_info *devices = hid_enumerate(0x0, 0x0);
//     struct hid_device_info *current = devices;
//     int index = 0;

//     while (current) {
//       std::wcout << L"[" << index++ << L"] Found device:" << std::endl
//                  << L"  Vendor ID: 0x" << std::hex << current->vendor_id
//                  << std::endl
//                  << L"  Product ID: 0x" << current->product_id << std::endl
//                  << L"  Manufacturer: "
//                  << (current->manufacturer_string ? current->manufacturer_string
//                                                   : L"Unknown")
//                  << std::endl
//                  << L"  Product: "
//                  << (current->product_string ? current->product_string
//                                              : L"Unknown")
//                  << std::endl
//                  << L"  Path: " << current->path << std::endl
//                  << L"----------------------------------------" << std::endl;

//       device_list.push_back(current);
//       current = current->next;
//     }

//     return device_list;
//   }

//   bool selectDevice(const std::vector<hid_device_info *> &device_list,
//                     int selection) {
//     if (selection < 0 || selection >= static_cast<int>(device_list.size())) {
//       std::wcerr << L"Invalid selection" << std::endl;
//       return false;
//     }

//     selected_device = hid_open_path(device_list[selection]->path);
//     if (!selected_device) {
//       std::wcerr << L"Failed to open device" << std::endl;
//       return false;
//     }

//     std::wcout << L"Successfully opened device" << std::endl;
//     return true;
//   }

//   void readControllerInput() {
//     if (!selected_device) {
//       std::wcerr << L"No device selected" << std::endl;
//       return;
//     }

//     std::wcout << L"Reading input data. Press Ctrl+C to stop." << std::endl;

//     while (is_running) {
//       unsigned char buffer[64] = {0};
//       int result =
//           hid_read_timeout(selected_device, buffer, sizeof(buffer), 100);

//       if (result > 0) {
//         // Print the raw data in hex
//         std::wcout << L"Raw data: ";
//         for (int i = 0; i < result; i++) {
//           std::wcout << std::hex << std::setw(2) << std::setfill(L'0')
//                      << static_cast<int>(buffer[i]) << L" ";
//         }
//         std::wcout << std::endl;

//         // Example of how to update the virtual controller (commented out for
//         // now)
//         /*
//         XUSB_REPORT report = {};
//         report.wButtons = buffer[1];
//         report.sThumbLX = buffer[2];
//         report.sThumbLY = buffer[3];
//         vigem_target_x360_update(vigem_client, target, report);
//         */
//       }
//     }
//   }

//   void stop() { is_running = false; }

//   ~ControllerManager() {
//     if (selected_device) {
//       hid_close(selected_device);
//     }

//     if (target) {
//       vigem_target_remove(vigem_client, target);
//       vigem_target_free(target);
//     }

//     if (vigem_client) {
//       vigem_disconnect(vigem_client);
//       vigem_free(vigem_client);
//     }

//     hid_exit();
//   }
// };

int main() {
  ControllerManager manager;

  if (!manager.initialize()) {
    std::wcerr << "Failed to initialize controller manager" << std::endl;
    return 1;
  }

  auto device_list = manager.detectControllers();

  if (device_list.empty()) {
    std::wcerr << "No controllers found" << std::endl;
    return 1;
  }

  std::wcout << L"Select a device to use: ";
  int selection{};
  std::cin >> selection;

  if (!manager.selectDevice(device_list, selection)) {
    return 1;
  }

  manager.readControllerInput();

  // Main loop
  // while (true) {
  //   manager.readControllerInput();
  //   // Add a small sleep here to prevent CPU overload
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1));
  // }

  return 0;
}