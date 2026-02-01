#include "EmergencyButton.h"
#include <iostream>
#include <thread>
#include <sys/stat.h>
#include "Toolbox.h"

EmergencyButton::EmergencyButton()
  : bIsConnected(false)
  , bReceivedEmergencySignal(false)
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
        print("Trying to connect to button");
        std::this_thread::sleep_for(1s);
    }
    
    if(!serial.open(port))
        goto connectstart;
        
    print("Connected to button!");
    bReceivedEmergencySignal = false;
    serial.setBaudrate(EMERGENCY_BUTTON_SERIAL_BAUDRATE);
}

bool EmergencyButton::update()
{
    uint8_t data = 0;

    //Receiving anything?
    if(serial.read((const char*)&data, 1) > 0)
    {
        tCountdownStart = std::chrono::system_clock::now();

        //Button has been pressed
        if(data == 'y')
        {
            if(!bReceivedEmergencySignal)
                print("Emergency button pushed down");
            bReceivedEmergencySignal = true;
            return false;
        }
        else if(data == 'n')
        {
            if(bReceivedEmergencySignal)
                print("Emergency button pulled up");
            bReceivedEmergencySignal = false;
        }
    }
    else
    {
        tCountdownEnd = std::chrono::system_clock::now();
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(tCountdownEnd - tCountdownStart);
    if(elapsed.count() > EMERGENCY_BUTTON_DISCONNECT_MILLIS)
    {
        print("Connection lost after " << elapsed.count() << "ms , reconnecting..");
        serial.close();
        connect();
    }
    else if(elapsed.count() > EMERGENCY_BUTTON_CRITICAL_CONNECTION_LOST_MILLIS)
    {
        print("Button has probably timed out after " << elapsed.count() << "ms, sending emergency signal just in case..");
        return false;
    }
    
    return true;
}
