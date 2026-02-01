#include "EmergencyButton.h"
#include "BroadcastSocket.h"
#include "Toolbox.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <chrono>

#define __str(s) _str(s)
#define _str(s) #s

std::string portArgument, intervalArgument;

void passArguments(int argc, char** argv)
{
    if(argc <= 1)
        return;
   
    std::string* nextargument = nullptr;

    for(int i = 1; i < argc; i++)
    {
        std::string argument = argv[i];

        if(nextargument != nullptr)
        {
            *nextargument = argument;
            nextargument = nullptr;
            continue;
        }

        if(argument == "-p" || argument == "--port")
            nextargument = &portArgument;
        else if(argument == "-i" || argument == "--interval")
            nextargument = &intervalArgument;
        else
            error("Unknown argument \"" << argument << "\"");
    }
}

int strToInt(const std::string& str, int defaultval = 0)
{
    if(str.empty())
        return defaultval;

    int ret = 0;
    try 
    {
        ret = std::stoi(str);
    }
    catch(const std::invalid_argument& e)
    {
        error(str + " is not a number!"); 
        return defaultval;
    }

    return ret; 
}


int main(int argc, char** argv)
{
    passArguments(argc, argv);

    BroadcastSocket socket(__str(EMERGENCY_BUTTON_SERVER_SOCKET_INTERFACE), strToInt(portArgument, EMERGENCY_BUTTON_BROADCAST_PORT));
    EmergencyButton button;

    int sleepInterval = strToInt(intervalArgument, EMERGENCY_BUTTON_BROADCAST_INTERVAL_MILLIS);
    
    while(true)
    {
        socket.sendMsg(
            button.update() ? "noemergency" : "emergency"
        );
        
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepInterval));
    }

    return 0;
}
