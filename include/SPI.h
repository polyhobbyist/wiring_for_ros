#ifndef ARDUINO_SPI_H
#define ARDUINO_SPI_H

#define SPI_CLOCK_DIV32 0

#define MSBFIRST 0

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2


class SPISettings
{
    public:
    SPISettings()
    {

    }
    
    SPISettings(uint32_t, uint8_t, uint8_t)
    {

    }


};


class SPIClass
{
    public:
    static void begin()
    {

    }

    static void setClockDivider(uint16_t)
    {

    }

    static void setBitOrder(uint8_t)
    {

    }

    static void setDataMode(uint8_t)
    {
        
    
    }
    static uint8_t transfer(uint8_t)
    {
        return 0;

    }

    static void beginTransaction(SPISettings&)
    {

    }

    static void endTransaction()
    {

    }

};
extern SPIClass SPI;

#endif