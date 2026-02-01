#pragma once
#include <iostream>

#define print(message) (std::cout << getCurrentTime() << message << std::endl)
#define _print(message) (std::cout << getCurrentTime() << message)
#define error(message) (std::cerr << getCurrentTime() << message << std::endl)
#define _error(message) (std::cerr << getCurrentTime() << message)

static clock_t t1 = clock();
static clock_t t2 = clock();

static std::string getCurrentTime()
{
    t2=clock();
    float diff ((float)t2-(float)t1);

    return "[" + std::to_string(diff / CLOCKS_PER_SEC) + "] ";
}
