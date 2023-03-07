#include "Arduino.h"
#include <thread>
#include <chrono>
#include <gpiod.h>

Stream Serial;

#if defined(__aarch64__)
// assume Pi
const static uint8_t s_map[] = 
{
    0,
    0,
    0,
    0,
    4,
    5,
    6,
    0,
    0,
    0,
    0,
    0,
    0,
    13,
    0,
    0,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27
};

const char *s_gpioChip = "gpiochip0";

#elif defined(__x86_64__)
// assume not pi, not risc-v
#else
const static uint8_t s_map[] = 
{
    0,
    0,
    0,
    0,
    39,
    42,
    43,
    0,
    0,
    0,
    0,
    0,
    0,
    44,
    0,
    0,
    34,
    0,
    37,
    38,
    35,
    36,
    32,
    33,
    110,
    0,
    113,
    118
};
const char *s_gpioChip = "gpiochip0";

#endif 

void delay(uint32_t ms)
{
    std::this_thread::sleep_for (std::chrono::milliseconds(ms));
}


void pinMode(uint16_t, uint16_t)
{

}

void digitalWrite(uint16_t pin, uint16_t state)
{
    if (pin < sizeof(s_map) && s_map[pin] != 0)
    {
        gpiod_chip* chip = gpiod_chip_open_by_name(s_gpioChip);
        if (chip)
        {
            // todo: cache?
            gpiod_line* line = gpiod_chip_get_line(chip, s_map[pin]);
            gpiod_line_request_output(line, "wiring_for_ros", 0);
            gpiod_line_set_value(line, state);
            gpiod_line_release(line);
            gpiod_chip_close(chip);
        }
    }
}
