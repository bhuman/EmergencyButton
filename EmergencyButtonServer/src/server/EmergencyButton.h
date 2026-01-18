#pragma once
#include "Serial.h"
#include <functional>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <chrono>

#ifndef EMERGENCY_BUTTON_CRITICAL_CONNECTION_LOST_MILLIS
#define EMERGENCY_BUTTON_CRITICAL_CONNECTION_LOST_MILLIS 2
#endif
#ifndef EMERGENCY_BUTTON_DISCONNECT_MILLIS
#define EMERGENCY_BUTTON_DISCONNECT_MILLIS 500
#endif
#ifndef EMERGENCY_BUTTON_SERIAL_BAUDRATE
#define EMERGENCY_BUTTON_SERIAL_BAUDRATE B9600
#endif

class EmergencyButton
{
private:
    std::chrono::system_clock::time_point tCountdownStart = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point tCountdownEnd = std::chrono::system_clock::now();
    std::function<void()> pEmergencyCallback;
    Serial serial;
    bool bIsConnected;

    std::string check_serial_devices();
    void connect();

public:
    EmergencyButton();

    void update();

    void onEmergency(std::function<void()> callback);
};