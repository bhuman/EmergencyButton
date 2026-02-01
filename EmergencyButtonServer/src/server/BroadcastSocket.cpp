#include "BroadcastSocket.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <fcntl.h>
#include "Toolbox.h"

BroadcastSocket::BroadcastSocket(const std::string& interface, int port) 
  : iAddrLength(sizeof(struct sockaddr_in))
{
    _print("Initializing UDP socket.. ");
    iSock = socket(AF_INET, SOCK_DGRAM, 0);
    if(iSock < 0) 
    {
        error("Failed to create socket");
        return;
    }
    print("got " << iSock << " as file descriptor.");

    if(setsockopt(iSock, SOL_SOCKET, SO_BROADCAST, &iEnable, sizeof(iEnable)) < 0)
    {
        error("Failed to make socket broadcast");
        return;
    }

    //Set to non-blocking
    fcntl(iSock, F_SETFL, O_NONBLOCK);

    setsockopt(iSock, SOL_SOCKET, SO_BINDTODEVICE, interface.c_str(), interface.length());

    print("Using port " << port << " on " << interface);
    memset((void*)&oBroadcastAddr, 0, iAddrLength);
    oBroadcastAddr.sin_family = AF_INET;
    //oBroadcastAddr.sin_addr.s_addr = htons(INADDR_ANY);
    oBroadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    oBroadcastAddr.sin_port = htons(port);
}


bool BroadcastSocket::sendMsg(const std::string& msg)
{
    return sendto(iSock, msg.c_str(), msg.length()+1, 0, (struct sockaddr*)&oBroadcastAddr, iAddrLength);
}
