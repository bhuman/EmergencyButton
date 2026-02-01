#pragma once
#include "Serial.h"
#include <cstring>
#include <iostream>
#include <cstdint>
#include <chrono>

class EmergencyButton
{
private:
    std::chrono::system_clock::time_point tCountdownStart = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point tCountdownEnd = std::chrono::system_clock::now();
    Serial serial;
    bool bIsConnected;
    bool bReceivedEmergencySignal;

    std::string check_serial_devices();
    void connect();

public:
    EmergencyButton();

    bool update();
};
