#pragma once
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#ifndef EMERGENCY_BUTTON_BROADCAST_PORT
#define EMERGENCY_BUTTON_BROADCAST_PORT 910
#endif

class BroadcastSocket
{
private:
    int sock;
    struct sockaddr_in broadcast_addr;
    struct sockaddr_in server_addr;
    int addr_len;
    int count;
    int ret;
  
public:
    BroadcastSocket();

    void sendPing();
};