#pragma once
#include <string>
#include <termios.h>

class Serial
{
private:
    int iFileDescriptor;
    bool bConnected;
    struct termios tty;

public:
    Serial();

    bool open(std::string port);
    void close();
    size_t read(const char* buffer, size_t size);
    size_t write(const char* buffer, size_t size);

    bool setBaudrate(int baudrate);

    bool isConnected();
};