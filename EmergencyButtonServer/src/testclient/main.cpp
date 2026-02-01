#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <iostream>

struct sockaddr_in oClientAddr;
struct sockaddr_in oServerAddr;
unsigned int iAddrLengh = sizeof(struct sockaddr_in);
char buffer[1024];

int main() 
{
    std::cout << "Initializing UDP socket.. ";
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) 
    {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }
    std::cout << "got " << sock << " as file descriptor." << std::endl;
    
    std::cout << "Listening on port " << EMERGENCY_BUTTON_BROADCAST_PORT << std::endl;
    memset((void*)&oServerAddr, 0, iAddrLengh);
    oServerAddr.sin_family = AF_INET;
    oServerAddr.sin_addr.s_addr = htons(INADDR_ANY);
    oServerAddr.sin_port = htons(EMERGENCY_BUTTON_BROADCAST_PORT);

    if(bind(sock, (struct sockaddr*)&oServerAddr, iAddrLengh) < 0) 
    {
        std::cerr << "Socket bind error" << std::endl;
        return -1;
    }


    std::cout << "Waiting for message" << std::endl;
    while(true) 
    {
        recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&oClientAddr, &iAddrLengh);
        std::cout << buffer << std::endl;
    }

    return 0;
}

