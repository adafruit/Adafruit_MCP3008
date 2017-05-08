# Adafruit_MCP3008
Adruino Library for MCP3008 8-Channel 10-Bit ADC with SPI interface.
http://www.adafruit.com/products/856

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
