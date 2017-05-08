/***************************************************
Simple example of reading the MCP3008 analog input channels and printing
them all out.

Author: Carter Nelson
License: Public Domain
****************************************************/

#include <Adafruit_MCP3008.h>

// Hardware SPI
Adafruit_MCP3008 adc;

// Software SPI (sck, mosi, miso, cs);
//Adafruit_MCP3008 adc(14, 13, 12, 15);

int count = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("MCP3008 simple test.");
}

void loop() {
  // 0: Return channel 0 minus channel 1
  // 1: Return channel 1 minus channel 0
  // 2: Return channel 2 minus channel 3
  // 3: Return channel 3 minus channel 2
  // 4: Return channel 4 minus channel 5
  // 5: Return channel 5 minus channel 4
  // 6: Return channel 6 minus channel 7
  // 7: Return channel 7 minus channel 6
  Serial.print(adc.readADCDifference(0));

  Serial.print("["); Serial.print(count); Serial.println("]");
  count++;
  
  delay(1000);
}
