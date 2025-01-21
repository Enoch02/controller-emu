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
        // Convert to 16-bit range
        state.leftStickX = (data[3] - 128) * 256;
        // state.leftStickY = (data[4] - 128) * 256;
        state.leftStickY = ((255 - data[4]) - 128) * 256;
        state.rightStickX = (data[1] - 128) * 256;
        state.rightStickY = (data[2] - 128) * 256;

        // analog buttons
        uint8_t analog_buttons = data[6];
        state.leftThumb = analog_buttons == 0x40;
        state.rightThumb = analog_buttons == 0x80;

        if (analog_buttons == 0xc0)
        {
            state.leftThumb = true;
            state.rightThumb = true;
        }

        // parse buttons
        uint8_t buttons = data[5];
        state.a = buttons == 0x4F;
        state.b = buttons == 0x2F;
        state.x = buttons == 0x8F;
        state.y = buttons == 0x1F;

        // parse dpad
        state.dpadUp = buttons == 0x00;
        state.dpadDown = buttons == 0x04;
        state.dpadLeft = buttons == 0x06;
        state.dpadRight = buttons == 0x02;

        // up + left
        if (buttons == 0x07)
        {
            state.dpadUp = true;
            state.dpadLeft = true;
        }

        // up + right
        if (buttons == 0x01)
        {
            state.dpadUp = true;
            state.dpadRight = true;
        }

        // down + left
        if (buttons == 0x05)
        {
            state.dpadDown = true;
            state.dpadLeft = true;
        }

        // down + right
        if (buttons == 0x03)
        {
            state.dpadDown = true;
            state.dpadRight = true;
        }

        // parse start and select
        uint8_t start_select = data[6];
        state.start = start_select == 0x20;
        state.back = start_select == 0x10;

        if (start_select == 0x30)
        {
            state.start = true;
            state.back = true;
        }

        uint8_t triggers = data[6];
        // parse l2 and r2
        state.leftTrigger = triggers == 0x04 ? 255 : 0;
        state.rightTrigger = triggers == 0x08 ? 255 : 0;

        // parse l1 and r1
        state.leftShoulder = triggers == 0x01;
        state.rightShoulder = triggers == 0x02;

        // button combinations need to be mapped apparently
        if (buttons == 0xff)
        {
            state.a = true;
            state.b = true;
            state.x = true;
            state.y = true;
        }

        // buttons 3 and 2
        if (buttons == 0x6f)
        {
            state.a = true;
            state.b = true;
        }

        // buttons 3 and 4
        if (buttons == 0xcf)
        {
            state.a = true;
            state.x = true;
        }

        // buttons 3 and 1
        if (buttons == 0x5f)
        {
            state.a = true;
            state.y = true;
        }

        // buttons 2 + 4 (0xAF)
        if (buttons == 0xaf)
        {
            state.b = true;
            state.x = true;
        }

        // buttons 2 + 1 (0x3F)
        if (buttons == 0x3f)
        {
            state.b = true;
            state.y = true;
        }

        // buttons 4 + 1 (0x9F)
        if (buttons == 0x9f)
        {
            state.x = true;
            state.y = true;
        }

        // buttons 3 + 2 + d (0xEF)
        if (buttons == 0xef)
        {
            state.a = true;
            state.b = true;
            state.x = true;
        }

        // buttons 3 + 2 + 1 (0x7F)
        if (buttons == 0x7f)
        {
            state.a = true;
            state.b = true;
            state.y = true;
        }

        // buttons 3 + 4 + 1 (0xDF)
        if (buttons == 0xdf)
        {
            state.a = true;
            state.x = true;
            state.y = true;
        }

        // buttons 2 + 4 + 1 (0xBF)
        if (buttons == 0xbf)
        {
            state.b = true;
            state.x = true;
            state.y = true;
        }

        // r1 + l1
        if (triggers == 0x03)
        {
            state.rightShoulder = true;
            state.leftShoulder = true;
        }

        // l2 + r2
        if (triggers == 0x0c)
        {
            state.leftTrigger = 255;
            state.rightTrigger = 255;
        }

        // l1 + l2
        if (triggers == 0x05)
        {
            state.leftShoulder = true;
            state.leftTrigger = 255;
        }

        // r1 + r2
        if (triggers == 0x0a)
        {
            state.rightShoulder = true;
            state.rightTrigger = 255;
        }

        // l1 + l2 + r1 + r2
        if (triggers == 0x0f)
        {
            state.leftShoulder = true;
            state.leftTrigger = 255;
            state.rightShoulder = true;
            state.rightTrigger = 255;
        }
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
    if (state.dpadUp)
        report.wButtons |= XUSB_GAMEPAD_DPAD_UP;
    if (state.dpadDown)
        report.wButtons |= XUSB_GAMEPAD_DPAD_DOWN;
    if (state.dpadLeft)
        report.wButtons |= XUSB_GAMEPAD_DPAD_LEFT;
    if (state.dpadRight)
        report.wButtons |= XUSB_GAMEPAD_DPAD_RIGHT;

    // Send report to virtual controller
    vigem_target_x360_update(client, target, report);
}