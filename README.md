Adafruit_MCP3008 [![Build Status](https://github.com/adafruit/Adafruit_MCP3008/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_MCP3008/actions)[![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_MCP3008/html/index.html)
================

<a href="https://www.adafruit.com/product/856"><img src="assets/board.jpg?raw=true" width="500px"></a>

This is the Adafruit MCP3008 - 8-Channel 10-Bit ADC With SPI Interface Library.

Tested for the following boards using pins shown.

MCP3008 | UNO | FEATHER HUZZAH<sup>1</sup> | FEATHER 32u4 | FEATHER M0
:---: | :---: | :---: | :---: | :---:
VDD | 5V | 3V | 3V | 3V 
VREF | 5V | 3V | 3V | 3V
AGND | GND | GND | GND | GND
CLK | 13 | SCK (14) | SCK | SCK
DOUT | 12 | MI (12) | MISO | MISO
DIN | 11 | MO (13) | MOSI | MOSI
CS | 10 | any<sup>2</sup> (15) | any<sup>2</sup> | any<sup>2</sup>
DGND | GND | GND | GND | GND

<sup>1</sup>also works for non-Feather HUZZAH, use (XX) pins

<sup>2</sup>use any available digital pin
