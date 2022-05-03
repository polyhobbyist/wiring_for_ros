#ifndef ARDUINO_WIRE_H
#define ARDUINO_WIRE_H
#include "Arduino.h"
#include "i2c/i2c.h"
#include <string>
#include <vector>

class TwoWire
{
    std::string _bus;
    int _i2cFileDescriptor;
    I2CDevice _i2cDevice;

    std::vector<uint8_t> _writeBuffer;
    std::vector<uint8_t> _readBuffer;
    size_t _readIndex;
    uint16_t _lastAddress;


public:
    TwoWire(std::string bus = "/dev/i2c-1");
    
    void begin();
    void beginTransmission(uint16_t id);
    uint8_t endTransmission(bool end = true);
    bool available();
    uint8_t read();
    void write(uint8_t value);
    void write(uint8_t* value, size_t len);
    uint32_t requestFrom(uint16_t, size_t);
    void setClock(uint32_t clock);
};


extern TwoWire Wire;

#endif