/*!
 *  @file Adafruit_MCP3008.cpp
 *
 *  @mainpage Adafruit MCP3008 - 8-Channel 10-Bit ADC
 *
 *  @section intro_sec Introduction
 *
 *  This is a library for the MCP3008 - 8-Channel 10-Bit ADC.
 *
 *  Designed specifically to work with the Adafruit MCP3008.
 *
 *  Pick one up today in the adafruit shop!
 *  ------> https://www.adafruit.com/product/856
 *
 *  These breakout use SPI to communicate, 4 ping are required.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  @section license License
 *
 *  MIT license, all text above must be included in any redistribution
 */

#include "Adafruit_MCP3008.h"

#include <SPI.h>

// Initialize for hardware SPI

/*!
 *    @brief  Initialize for hardware SPI
 *    @param  cs
 *            number of CSPIN (Chip Select)
 *    @param  theSPI
 *            optional SPI object
 *    @return true if process is successful
 */
bool Adafruit_MCP3008::begin(uint8_t cs, SPIClass *theSPI) {
  hwSPI = true;

  this->cs = cs;

  pinMode(this->cs, OUTPUT);
  digitalWrite(this->cs, HIGH);
  _spi = theSPI;
  _spi->begin();
  /* SPI.begin(); */

  return true;
}

/*!
 *    @brief  Initialize for software SPI
 *    @param  sck
 *            number of pin used for SCK (Serial Clock)
 *    @param  mosi
 *            number of pin used for MOSI (Master Out Slave In)
 *    @param  miso
 *            number of pin used for MISO (Master In Slave Out)
 *    @param  cs
 *            number of pin used for CS (Chip Select)
 *    @return true if process is successful
 */
bool Adafruit_MCP3008::begin(uint8_t sck, uint8_t mosi, uint8_t miso,
                             uint8_t cs) {
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

  return true;
}

/*!
 *    @brief  Read single ended ADC channel.
 *    @param  channel
 *            channel number
 *    @return -1 if channel < 0 or channel > 7, otherwise ADC (int)
 */
int Adafruit_MCP3008::readADC(uint8_t channel) {
  if ((channel < 0) || (channel > 7))
    return -1;
  return SPIxADC(channel, false);
}

/*!
 *    @brief  Read differential ADC channel.
 *    @param  differential
 *            0: Return channel 0 minus channel 1
 *            1: Return channel 1 minus channel 0
 *            2: Return channel 2 minus channel 3
 *            3: Return channel 3 minus channel 2
 *            4: Return channel 4 minus channel 5
 *            5: Return channel 5 minus channel 4
 *            6: Return channel 6 minus channel 7
 *            7: Return channel 7 minus channel 6
 *    @return -1 if channel < 0 or channel > 7, otherwise ADC difference (int)
 */
int Adafruit_MCP3008::readADCDifference(uint8_t differential) {
  if ((differential < 0) || (differential > 7))
    return -1;
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

  command = ((0x01 << 7) |             // start bit
             (sgldiff << 6) |          // single or differential
             ((channel & 0x07) << 3)); // channel number

  if (hwSPI) {
    byte b0, b1, b2;

    _spi->beginTransaction(
        SPISettings(MCP3008_SPI_MAX, MCP3008_SPI_ORDER, MCP3008_SPI_MODE));
    digitalWrite(cs, LOW);

    b0 = _spi->transfer(command);
    b1 = _spi->transfer(0x00);
    b2 = _spi->transfer(0x00);

    digitalWrite(cs, HIGH);
    _spi->endTransaction();

    return 0x3FF & ((b0 & 0x01) << 9 | (b1 & 0xFF) << 1 | (b2 & 0x80) >> 7);

  } else {

    uint16_t outBuffer, inBuffer = 0;

    digitalWrite(cs, LOW);

    // 5 command bits + 1 null bit + 10 data bits = 16 bits
    outBuffer = command << 8;
    for (int c = 0; c < 16; c++) {
      digitalWrite(mosi, (outBuffer >> (15 - c)) & 0x01);
      digitalWrite(sck, HIGH);
      digitalWrite(sck, LOW);
      inBuffer <<= 1;
      if (digitalRead(miso))
        inBuffer |= 0x01;
    }

    digitalWrite(cs, HIGH);

    return inBuffer & 0x3FF;
  }
}
