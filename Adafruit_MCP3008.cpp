/***************************************************
  This is an Arduino Library for the MCP3008 - 8-Channel 10-Bit ADC
  With SPI Interface.
  ----> http://www.adafruit.com/products/856
  
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Copyright (c) 2017, Adafruit Industries (adafruit.com)
  MIT License (https://opensource.org/licenses/MIT)
****************************************************/
#include "Adafruit_MCP3008.h"

#include <SPI.h>

// Initialize for hardware SPI
bool Adafruit_MCP3008::begin(uint8_t cs) {
  hwSPI = true;

  this->cs = cs;

  pinMode(this->cs, OUTPUT);
  digitalWrite(this->cs, HIGH);
  SPI.begin();
}

// Initialize for software SPI
bool Adafruit_MCP3008::begin(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs) {
  hwSPI = false;
  
  this->sck = sck;
  this->mosi = mosi;
  this->miso = miso;
  this->cs = cs;
  
  pinMode(this->sck, OUTPUT);
  pinMode(this->mosi, OUTPUT);
  pinMode(this->miso, INPUT);
  pinMode(this->cs, OUTPUT);
  
  digitalWrite(this->sck, LOW);
  digitalWrite(this->mosi, LOW);
  digitalWrite(this->cs, HIGH);
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
  byte command, sgldiff;
  
  if (differential) {
    sgldiff = 0;
  } else {
    sgldiff = 1;
  }
  
  command = ((0x01 << 7) |                 // start bit
             (sgldiff << 6) |              // single or differential
             ((channel & 0x07) << 3) );    // channel number
  
  if (hwSPI) {
    byte b0, b1, b2;
  
    SPI.beginTransaction(SPISettings(MCP3008_SPI_MAX, MCP3008_SPI_ORDER, MCP3008_SPI_MODE));
    digitalWrite(cs, LOW);
  
    b0 = SPI.transfer(command);
    b1 = SPI.transfer(0x00);
    b2 = SPI.transfer(0x00);
  
    digitalWrite(cs, HIGH);
    SPI.endTransaction();
  
    return 0x3FF & ((b0 & 0x01) << 9 |
                    (b1 & 0xFF) << 1 |
                    (b2 & 0x80) >> 7 );
  
  } else {
  
    uint16_t outBuffer, inBuffer = 0;
  
    digitalWrite(cs, LOW);
  
    // 5 command bits + 1 null bit + 10 data bits = 16 bits
    outBuffer = command << 8;
    for (int c=0; c<16; c++) {
      digitalWrite(mosi, (outBuffer >> 15-c) & 0x01);
      digitalWrite(sck, HIGH);
      digitalWrite(sck, LOW);
      inBuffer <<= 1;
      if (digitalRead(miso)) inBuffer |= 0x01;
    }
  
    digitalWrite(cs, HIGH);
  
    return inBuffer & 0x3FF;
  }
}