#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdint.h>

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

#define F(x) x

#define PROGMEM

#if __cplusplus < 201703L
typedef uint8_t byte;
#endif

inline uint8_t highByte(uint16_t val)
{
    return (uint8_t)(val >> 8);
}

inline uint8_t lowByte(uint16_t val)
{
    return (uint8_t)(val & 0xFF);
}

inline void pinMode(uint16_t, uint16_t)
{

}

inline void digitalWrite(uint16_t, uint16_t)
{

}


class Stream
{
    public:
    void print(char)
    {

    }

    void println(char)
    {
        
    }

    void println()
    {
        
    }
};

extern Stream Serial;


void delay(uint32_t);

#define pgm_read_byte_near(x) *x
#define __FlashStringHelper void

// some libraries do not do this, so help them out.
using namespace std;

#endif
