#ifndef CONTROLLER_PARSER_H
#define CONTROLLER_PARSER_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ViGEm/Client.h>
#include <vector>
#include <map>
#include "ControllerState.h"

class ControllerParser
{
  public:
    ControllerParser();

    ControllerState parseRawData(const std::vector<uint8_t> &data);

    void updateXInputState(PVIGEM_CLIENT client, PVIGEM_TARGET target, const ControllerState &state);
};

#endif // CONTROLLER_PARSER_H