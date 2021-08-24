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

Adafruit_MCP3008::~Adafruit_MCP3008() {
  if (spi_dev)
    delete spi_dev;
}

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
  _cs = cs;
  if (spi_dev)
    delete spi_dev;
  spi_dev = new Adafruit_SPIDevice(cs, MCP3008_SPI_FREQ, MCP3008_SPI_ORDER,
                                   MCP3008_SPI_MODE, theSPI);
  return spi_dev->begin();
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
  _cs = cs;
  spi_dev = new Adafruit_SPIDevice(cs, sck, miso, mosi, MCP3008_SPI_FREQ,
                                   MCP3008_SPI_ORDER, MCP3008_SPI_MODE);
  return spi_dev->begin();
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
  // see datasheet sec 6.1
  buffer[0] = 0x01;
  buffer[1] = ((differential ? 0 : 1) << 7) | (channel << 4);
  spi_dev->beginTransaction();
  digitalWrite(_cs, LOW);
  spi_dev->transfer(buffer, 3);
  digitalWrite(_cs, HIGH);
  spi_dev->endTransaction();
  return (((uint16_t)(buffer[1] & 0x07)) << 8) | buffer[2];
}
