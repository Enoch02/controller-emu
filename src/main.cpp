#define WIN32_LEAN_AND_MEAN
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <time.h>
#include <windows.h> // this needs to come before ViGEm
#include <ViGEm/Client.h>
#include <hidapi/hidapi.h>

class ControllerManager {
private:
  PVIGEM_CLIENT vigem_client;
  PVIGEM_TARGET target;
  std::vector<hid_device *> connected_devices;

public:
  ControllerManager() : vigem_client(nullptr), target(nullptr) {}

  bool initialize() {
    // Initialize HID API
    if (hid_init() != 0) {
      std::cerr << "Failed to initialize HID API" << std::endl;
      return false;
    }

    // Initialize ViGEm client
    vigem_client = vigem_alloc();
    if (vigem_client == nullptr) {
      std::cerr << "Failed to allocate ViGEm client" << std::endl;
      return false;
    }

    const auto ret = vigem_connect(vigem_client);
    if (!VIGEM_SUCCESS(ret)) {
      std::cerr << "Failed to connect to ViGEm bus: 0x" << std::hex << ret
                << std::endl;
      return false;
    }

    // Allocate Xbox 360 controller
    target = vigem_target_x360_alloc();
    if (target == nullptr) {
      std::cerr << "Failed to allocate Xbox 360 controller" << std::endl;
      return false;
    }

    // Add controller to ViGEm bus
    const auto plugin_result = vigem_target_add(vigem_client, target);
    if (!VIGEM_SUCCESS(plugin_result)) {
      std::cerr << "Failed to add target to ViGEm bus: 0x" << std::hex
                << plugin_result << std::endl;
      return false;
    }

    return true;
  }

  void detectControllers() {
    struct hid_device_info *devices =
        hid_enumerate(0x0, 0x0); // List all HID devices
    struct hid_device_info *current = devices;

    while (current) {
      std::wcout << L"Found device:" << std::endl
                 << L"  Vendor ID: 0x" << std::hex << current->vendor_id
                 << std::endl
                 << L"  Product ID: 0x" << current->product_id << std::endl
                 << L"  Manufacturer: "
                 << (current->manufacturer_string
                         ? std::wstring(current->manufacturer_string)
                         : L"Unknown")
                 << std::endl
                 << L"  Product: "
                 << (current->product_string
                         ? std::wstring(current->product_string)
                         : L"Unknown")
                 << std::endl;

      // Open the device
      hid_device *device = hid_open(current->vendor_id, current->product_id,
                                    current->serial_number);
      if (device) {
        connected_devices.push_back(device);
      }

      current = current->next;
    }

    hid_free_enumeration(devices);
  }

  void readControllerInput() {
    for (auto device : connected_devices) {
      unsigned char buffer[64];
      int result = hid_read_timeout(device, buffer, sizeof(buffer), 100);

      if (result > 0) {
        // Process the input data
        // This is where you would map the input to Xbox 360 controls
        XUSB_REPORT report = {};
        // Example mapping (you'll need to customize this based on your
        // controller): report.wButtons = buffer[1]; // Map buttons
        // report.sThumbLX = buffer[2]; // Map left stick X
        // report.sThumbLY = buffer[3]; // Map left stick Y

        vigem_target_x360_update(vigem_client, target, report);
      }
    }
  }

  ~ControllerManager() {
    // Cleanup
    for (auto device : connected_devices) {
      hid_close(device);
    }

    if (target) {
      vigem_target_remove(vigem_client, target);
      vigem_target_free(target);
    }

    if (vigem_client) {
      vigem_disconnect(vigem_client);
      vigem_free(vigem_client);
    }

    hid_exit();
  }
};

int main() {
  ControllerManager manager;

  if (!manager.initialize()) {
    std::cerr << "Failed to initialize controller manager" << std::endl;
    return 1;
  }

  manager.detectControllers();

  // Main loop
  while (true) {
    manager.readControllerInput();
    // Add a small sleep here to prevent CPU overload
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}