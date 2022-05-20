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

    inline void setAddressSize(size_t bytes)
    {
        _i2cDevice.iaddr_bytes = bytes;
    }
    
    void begin();
    void beginTransmission(uint16_t id);
    uint8_t endTransmission(bool end = false);
    bool available();
    uint8_t read();
    void write(uint8_t value);
    void write(uint8_t* value, size_t len);
    uint32_t requestFrom(uint16_t, size_t);
    uint32_t requestFrom(uint16_t id, uint8_t regi, size_t size);

    inline uint32_t requestFrom(uint16_t id, uint8_t regi, int size)
    {
        if (size > 0)
        {
            return requestFrom(id, regi, static_cast<size_t>(size));
        }

        return 0;
    }

    void setClock(uint32_t clock);
};


extern TwoWire Wire;

#endif