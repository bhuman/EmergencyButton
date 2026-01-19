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
    BroadcastSocket();

    bool sendMsg(const std::string& msg);
};