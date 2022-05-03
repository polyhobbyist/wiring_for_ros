#include "Arduino.h"
#include <thread>
#include <chrono>

Stream Serial;

void delay(uint32_t ms)
{
    std::this_thread::sleep_for (std::chrono::milliseconds(ms));
}
