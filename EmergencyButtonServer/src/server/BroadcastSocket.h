#pragma once
#include <netinet/in.h>
#include <string>

class BroadcastSocket
{
private:
    struct sockaddr_in oBroadcastAddr;
    int iSock;
    int iEnable = 1;
    int iAddrLength;
  
public:
    BroadcastSocket(const std::string& interface, int port);

    bool sendMsg(const std::string& msg);
};
