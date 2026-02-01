#include "Serial.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <sys/ioctl.h>
#include "Toolbox.h"

Serial::Serial()
  : iFileDescriptor(-1),
    bConnected(false)
{

}

bool Serial::open(std::string port)
{
    this->iFileDescriptor = ::open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC | O_NDELAY);
    if(this->iFileDescriptor < 0)
    {
        error("Failed to open " << port << ": " << strerror(errno));
        return false;
    }
    
    
    if(tcgetattr(iFileDescriptor, &tty) != 0) {
        error("Failed to get tty: " << strerror(errno));
        return false;
    }


    cfmakeraw(&tty);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_iflag &= ~(IXOFF | IXANY);

    tty.c_cc[VMIN] = 0; 
    tty.c_cc[VTIME] = 0;

    if(tcsetattr(iFileDescriptor, TCSANOW, &tty) != 0)
    {
        error("Error from tcsetattr: " << strerror(errno));
        return false;
    }

    bConnected = true;
    return true;
}

void Serial::close()
{
    bConnected = false;
    ::close(iFileDescriptor);
}

size_t Serial::read(const char* buffer, size_t size)
{
    size_t readsize = ::read(iFileDescriptor, (void*)buffer, size);
    ioctl(iFileDescriptor, TCFLSH, 2); // flush serial port buffer
    return readsize;
}

size_t Serial::write(const char* buffer, size_t size)
{
    return ::write(iFileDescriptor, (void*)buffer, size);
}

bool Serial::setBaudrate(int baudrate)
{
    cfsetospeed(&tty, baudrate);
    cfsetispeed(&tty, baudrate);

    if(tcsetattr(iFileDescriptor, TCSANOW, &tty) != 0)
    {
        error("Error from tcsetattr: " << strerror(errno));
        return false;
    }
    return true;
}

bool Serial::isConnected()
{
    return bConnected;
}
