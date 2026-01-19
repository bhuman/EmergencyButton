#include "BroadcastSocket.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>

BroadcastSocket::BroadcastSocket() 
  : iAddrLength(sizeof(struct sockaddr_in))
{
    iSock = socket(AF_INET, SOCK_DGRAM, 0);
    if(iSock < 0) 
    {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    if(setsockopt(iSock, SOL_SOCKET, SO_BROADCAST, &iEnable, sizeof(iEnable)) < 0)
    {
        std::cerr << "setsockopt error" << std::endl;
        return;
    }


    memset((void*)&oBroadcastAddr, 0, iAddrLength);
    oBroadcastAddr.sin_family = AF_INET;
    oBroadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    oBroadcastAddr.sin_port = htons(EMERGENCY_BUTTON_BROADCAST_PORT);
}


bool BroadcastSocket::sendMsg(const std::string& msg)
{
    return sendto(iSock, msg.c_str(), msg.length(), 0, (struct sockaddr*)&oBroadcastAddr, iAddrLength);
}