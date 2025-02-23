#ifndef CONTROLLER_STATE_H
#define CONTROLLER_STATE_H
#include <cstdint>

struct ControllerState
{
    // Analog sticks (-32768 to 32767)
    int16_t leftStickX = 0;
    int16_t leftStickY = 0;
    int16_t rightStickX = 0;
    int16_t rightStickY = 0;

    // Triggers (0 to 255)
    uint8_t leftTrigger = 0;
    uint8_t rightTrigger = 0;

    // Buttons (true/false)
    bool dpadUp = false;
    bool dpadDown = false;
    bool dpadLeft = false;
    bool dpadRight = false;
    bool a = false;
    bool b = false;
    bool x = false;
    bool y = false;
    bool leftShoulder = false;
    bool rightShoulder = false;
    bool leftThumb = false;
    bool rightThumb = false;
    bool start = false;
    bool back = false;
};

#endif // CONTROLLER_STATE_H