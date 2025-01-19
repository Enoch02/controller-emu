#ifndef CONTROLLER_PARSER_H
#define CONTROLLER_PARSER_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ViGEm/Client.h>
#include <vector>
#include "ControllerState.h"

class ControllerParser
{
  public:
    static ControllerState ParseRawData(const std::vector<uint8_t> &data);

    static void UpdateXInputState(PVIGEM_CLIENT client, PVIGEM_TARGET target, const ControllerState &state);
};

#endif // CONTROLLER_PARSER_H