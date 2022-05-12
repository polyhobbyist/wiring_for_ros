#include <memory>
#include <chrono>
#include "rclcpp/rclcpp.hpp"

#include "Arduino.h"
#include <string.h>
#include "Wire.h"

TwoWire Wire;
TwoWire::TwoWire(std::string bus)
    : _bus(bus)
    , _readIndex(0)
{



}

void TwoWire::begin()
{
    if ((_i2cFileDescriptor = i2c_open(_bus.c_str())) == -1) 
    {
        return;
    }

    i2c_init_device(&_i2cDevice);
}


void TwoWire::beginTransmission(uint16_t id)
{
    _i2cDevice.bus = _i2cFileDescriptor;
    _i2cDevice.addr = id;
    _i2cDevice.iaddr_bytes = 1;
    _i2cDevice.page_bytes = 8;


    // If we're beginning a new transmission, but have an old one, purge it now
    if (_writeBuffer.size() > 0)
    {
        endTransmission(true);
    }
}

uint8_t TwoWire::endTransmission(bool sendStop)
{
    uint8_t error = 0;
    uint8_t* buffer = nullptr;
    size_t writeSize = 0;

    if (sendStop)
    {
        uint16_t regi = 0;
        if (_writeBuffer.size() >= _i2cDevice.iaddr_bytes)
        {
            regi = _writeBuffer.data()[0]; // first byte is the full address or the upper byte in a 2 byte address.
            if (_i2cDevice.iaddr_bytes == 2)
            {
                regi = (regi << 8) | _writeBuffer.data()[1]; // second byte is the low address.
            }

            writeSize = _writeBuffer.size() - _i2cDevice.iaddr_bytes;
            buffer = _writeBuffer.data() + _i2cDevice.iaddr_bytes;
#ifdef TRACE_i2c
            std::string trace = "";
            for (size_t i = 0; i < writeSize; i++)
            {
                char buff[100];
                snprintf(buff, sizeof(buff), "0x%02x ", buffer[i]);
                trace.append(buff);
            }
            RCLCPP_INFO(rclcpp::get_logger("i2c"), "Writing to [0x%2x]: [%s] %d bytes", regi, trace.c_str(), writeSize);
#endif
        }

        int ret = i2c_ioctl_write(&_i2cDevice, regi, buffer, writeSize);

        _i2cDevice.flags = 0;
        
        if (ret == -1 || (size_t)ret != writeSize)
        {
            RCLCPP_INFO(rclcpp::get_logger("i2c"), "failed to write: [%d]", ret);
            error = 1;
        }

        _writeBuffer.clear();
    }

    return error;
}

bool TwoWire::available()
{
    return _readIndex < _readBuffer.size();
}

uint8_t TwoWire::read()
{
    if (_readIndex < _readBuffer.size())
    {
        return _readBuffer[_readIndex++];
    }

    return 0;
}

void TwoWire::write(uint8_t value)
{
    _writeBuffer.push_back(value);
}

void TwoWire::write(uint8_t* value, size_t len)
{
    _writeBuffer.insert(_writeBuffer.end(), value, value + len);
}

uint32_t TwoWire::requestFrom(uint16_t id, size_t size)
{
    uint16_t regi = _lastAddress;
    if (_writeBuffer.size() == _i2cDevice.iaddr_bytes)
    {
        regi = _writeBuffer.data()[0]; // first byte is the full address or the upper byte in a 2 byte address.
        if (_i2cDevice.iaddr_bytes == 2)
        {
            regi = regi << 8 | _writeBuffer.data()[1]; // second byte is the low address.
        }
        _writeBuffer.clear();
    }
    else
    {
        // If this was a hold over from a previous batch, purge it
        endTransmission(true);
    }

    _i2cDevice.bus = _i2cFileDescriptor;
    _i2cDevice.addr = id;

    _readIndex = 0;

    _readBuffer.resize(size, 0);

    int ret = i2c_ioctl_read(&_i2cDevice, regi, _readBuffer.data(), size);
    if (ret == -1 || (size_t)ret != size)
    {
        RCLCPP_INFO(rclcpp::get_logger("i2c"), "failed to read status: [%d]", ret);

        // Don't prop error code
        ret = 0;
    }

    _lastAddress = regi + size;
    return (size_t)ret;
}

void TwoWire::setClock(uint32_t)
{
    // TODO

}
