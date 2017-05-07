/***************************************************
  This is an Arduino Library for the MCP3008 - 8-Channel 10-Bit ADC
  With SPI Interface.
  ----> http://www.adafruit.com/products/856
 
  TODO: add more banner text
****************************************************/
#include "Adafruit_MCP3008.h"

#include <SPI.h>

// Constructor for hardware SPI
Adafruit_MCP3008::Adafruit_MCP3008(uint8_t cs) {
    chipSelect = cs;
    
    pinMode(chipSelect, OUTPUT);
    digitalWrite(chipSelect, HIGH);
    SPI.begin();
}

// Constructor for software SPI
Adafruit_MCP3008::Adafruit_MCP3008(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss) {
    
}

// Read single ended ADC channel.
int Adafruit_MCP3008::readADC(uint8_t channel) {
    if ((channel < 0) || (channel > 7)) return -1;

    return SPIxADC(channel, false);
}

// Read differential ADC channel.
//      0: Return channel 0 minus channel 1
//      1: Return channel 1 minus channel 0
//      2: Return channel 2 minus channel 3
//      3: Return channel 3 minus channel 2
//      4: Return channel 4 minus channel 5
//      5: Return channel 5 minus channel 4
//      6: Return channel 6 minus channel 7
//      7: Return channel 7 minus channel 6
int Adafruit_MCP3008::readADCDifference(uint8_t differential) {
    if ((differential < 0) || (differential > 7)) return -1;
    
    return SPIxADC(differential, true);    
}

// SPI transfer for ADC read
int Adafruit_MCP3008::SPIxADC(uint8_t channel, bool differential) {
    byte b0, b1, b2, sgldiff;
    
    if (differential) {
        sgldiff = 0;
    } else {
        sgldiff = 1;
    }
    
    SPI.beginTransaction(SPISettings(MCP3008_SPI_MAX, MCP3008_SPI_ORDER, MCP3008_SPI_MODE));
    digitalWrite(chipSelect, LOW);
    
    b0 = SPI.transfer(  (0x01 << 7) |                 // start bit
                        (sgldiff << 6) |              // single or differential
                        ((channel & 0x07) << 3) );    // channel number
    
    b1 = SPI.transfer(0x00);
    b2 = SPI.transfer(0x00);
    
    digitalWrite(chipSelect, HIGH);
    SPI.endTransaction();
    
    return 0x3FF & ((b0 & 0x01) << 9 |
                    (b1 & 0xFF) << 1 |
                    (b2 & 0x80) >> 7 );
}