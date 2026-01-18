#include "BroadcastSocket.h"
#include <iostream>

BroadcastSocket::BroadcastSocket() 
{  
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) 
  {
    std::cerr << "Failed to create socket" << std::endl;
    return;
  }

  ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &(int){1}, sizeof(int));
  if(ret == -1)
  {
    std::cerr << "setsockopt error" << std::endl;
    return;
  }

  addr_len = sizeof(struct sockaddr_in);

  memset((void*)&broadcast_addr, 0, addr_len);
  broadcast_addr.sin_family = AF_INET;
  broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
  broadcast_addr.sin_port = htons(EMERGENCY_BUTTON_BROADCAST_PORT);
}


void BroadcastSocket::sendPing()
{
    ret = sendto(sock, "test", strlen("test"), 0, (struct sockaddr*) &broadcast_addr, addr_len);
}