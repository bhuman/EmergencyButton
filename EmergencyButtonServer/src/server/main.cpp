#include "EmergencyButton.h"
#include "BroadcastSocket.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <chrono>

int main(int argc, char** argv)
{
    EmergencyButton button;
    BroadcastSocket socket;
    
    while(true)
    {
        socket.sendMsg(
            button.update() ? "noemergency" : "emergency"
        );
        
        std::this_thread::sleep_for(std::chrono::milliseconds(EMERGENCY_BUTTON_BROADCAST_INTERVAL_MILLIS));
    }

    return 0;
}