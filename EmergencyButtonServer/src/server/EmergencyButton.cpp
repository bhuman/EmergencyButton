#include "EmergencyButton.h"
#include <iostream>
#include <thread>
#include <sys/stat.h>

EmergencyButton::EmergencyButton()
  : bIsConnected(false)
{
    connect();
}

std::string EmergencyButton::check_serial_devices() 
{
    struct stat buffer;

    for(std::string dev : {"/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyACM2"})
        if(stat (dev.c_str(), &buffer) == 0)
            return dev;
    return "";
}

void EmergencyButton::connect()
{
    using namespace std::chrono_literals;
    std::string port = "";

    connectstart:
    while((port = check_serial_devices()) == "")
    {
        std::cout << "Trying to connect to button" << std::endl;
        std::this_thread::sleep_for(1s);
    }
    
    if(!serial.open(port))
        goto connectstart;
        
    std::cout << "Connected to button!" << std::endl;
    serial.setBaudrate(EMERGENCY_BUTTON_SERIAL_BAUDRATE);
}

bool EmergencyButton::update()
{
    uint8_t data = 0;
    if(serial.read((const char*)&data, 1) > 0)
    {
        tCountdownStart = std::chrono::system_clock::now();
        return false;
    }
    else
    {
        tCountdownEnd = std::chrono::system_clock::now();
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(tCountdownEnd - tCountdownStart);
    if(elapsed.count() > EMERGENCY_BUTTON_DISCONNECT_MILLIS)
    {
        std::cerr << "Connection lost, reconnecting.." << std::endl;
        serial.close();
        connect();
    }
    else if(elapsed.count() > EMERGENCY_BUTTON_CRITICAL_CONNECTION_LOST_MILLIS)
    {
        return false;
    }
    
    return true;
}