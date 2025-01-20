#include "ControllerParser.h"

ControllerParser::ControllerParser()
{
}

// TODO: reimplement the mapping function
//  TODO: load the mapping then use it to parse the raw data
ControllerState ControllerParser::parseRawData(const std::vector<uint8_t> &data)
{
    ControllerState state{};

    // Example parsing for a common controller format
    // Note: Actual implementation depends on your specific controller

    // First byte often contains button states
    // if (data.size() >= 8)
    // { // Typical minimum size
    //     // Parse analog sticks (assuming 8-bit values centered at 128)
    //     state.leftStickX = (data[1] - 128) * 256; // Convert to 16-bit range
    //     state.leftStickY = (data[2] - 128) * 256;
    //     state.rightStickX = (data[3] - 128) * 256;
    //     state.rightStickY = (data[4] - 128) * 256;

    //     // Parse buttons (example bit mapping)
    //     uint8_t buttons = data[5];
    //     state.a = (buttons & 0x01) != 0;
    //     state.b = (buttons & 0x02) != 0;
    //     state.x = (buttons & 0x04) != 0;
    //     state.y = (buttons & 0x08) != 0;

    //     // Parse triggers
    //     state.leftTrigger = data[6];
    //     state.rightTrigger = data[7];
    // }

    // parsing for my specific controller
    if (data.size() >= 8)
    {
        // parse analog sticks
        // analog buttons
        uint8_t analog_buttons = data[6];
        state.leftThumb = analog_buttons == 0x40;
        state.rightThumb = analog_buttons == 0x80;

        // parse buttons
        uint8_t buttons = data[5];
        state.a = buttons == 0x4F;
        state.b = buttons == 0x2F;
        state.x = buttons == 0x8F;
        state.y = buttons == 0x1F;

        // parse start and select
        uint8_t start_select = data[6];
        state.start = start_select == 0x20;
        state.back = start_select == 0x10;

        // parse l2 and r2
        state.leftTrigger = data[6] == 0x04 ? 255 : 0;
        state.rightTrigger = data[6] == 0x08 ? 255 : 0;

        // parse l1 and r1
        state.leftShoulder = data[6] == 0x01 ? 1 : 0;
        state.rightShoulder = data[6] == 0x02 ? 1 : 0;
    }

    return state;
}

void ControllerParser::updateXInputState(PVIGEM_CLIENT client, PVIGEM_TARGET target, const ControllerState &state)
{
    XUSB_REPORT report{};

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