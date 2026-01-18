#include "EmergencyButton.h"
#include "BroadcastSocket.h"

int main(int argc, char** argv)
{
    EmergencyButton button;
    button.onEmergency([]() {
        std::cout << "emergency" << std::endl;
    });

    BroadcastSocket socket;
    
    while(true)
    {
        button.update();
    }

    return 0;
}