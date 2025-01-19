#include "ControllerParser.h"

ControllerState ControllerParser::ParseRawData(const std::vector<uint8_t> &data)
{
    ControllerState state{};

    // Example parsing for a common controller format
    // Note: Actual implementation depends on your specific controller

    // First byte often contains button states
    if (data.size() >= 8)
    { // Typical minimum size
        // Parse analog sticks (assuming 8-bit values centered at 128)
        state.leftStickX = (data[1] - 128) * 256; // Convert to 16-bit range
        state.leftStickY = (data[2] - 128) * 256;
        state.rightStickX = (data[3] - 128) * 256;
        state.rightStickY = (data[4] - 128) * 256;

        // Parse buttons (example bit mapping)
        uint8_t buttons = data[5];
        state.a = (buttons & 0x01) != 0;
        state.b = (buttons & 0x02) != 0;
        state.x = (buttons & 0x04) != 0;
        state.y = (buttons & 0x08) != 0;

        // Parse triggers
        state.leftTrigger = data[6];
        state.rightTrigger = data[7];
    }

    return state;
}

void ControllerParser::UpdateXInputState(PVIGEM_CLIENT client, PVIGEM_TARGET target, const ControllerState &state)
{
    XUSB_REPORT report;
    memset(&report, 0, sizeof(XUSB_REPORT));

    // Map analog sticks
    report.sThumbLX = state.leftStickX;
    report.sThumbLY = state.leftStickY;
    report.sThumbRX = state.rightStickX;
    report.sThumbRY = state.rightStickY;

    // Map triggers
    report.bLeftTrigger = state.leftTrigger;
    report.bRightTrigger = state.rightTrigger;

    // Map buttons
    if (state.a)
        report.wButtons |= XUSB_GAMEPAD_A;
    if (state.b)
        report.wButtons |= XUSB_GAMEPAD_B;
    if (state.x)
        report.wButtons |= XUSB_GAMEPAD_X;
    if (state.y)
        report.wButtons |= XUSB_GAMEPAD_Y;
    if (state.leftShoulder)
        report.wButtons |= XUSB_GAMEPAD_LEFT_SHOULDER;
    if (state.rightShoulder)
        report.wButtons |= XUSB_GAMEPAD_RIGHT_SHOULDER;
    if (state.start)
        report.wButtons |= XUSB_GAMEPAD_START;
    if (state.back)
        report.wButtons |= XUSB_GAMEPAD_BACK;
    if (state.leftThumb)
        report.wButtons |= XUSB_GAMEPAD_LEFT_THUMB;
    if (state.rightThumb)
        report.wButtons |= XUSB_GAMEPAD_RIGHT_THUMB;

    // Send report to virtual controller
    vigem_target_x360_update(client, target, report);
}