/***************************************************
Simple example of reading the MCP3008 analog input channels and printing
them all out.

Author: Carter Nelson
License: Public Domain
****************************************************/

#include <Adafruit_MCP3008.h>

Adafruit_MCP3008 adc;

int count = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("MCP3008 simple test.");

  // Hardware SPI (specify CS, use any available digital)
  // Can use defaults if available
  // ex: UNO (SS=10) or Huzzah (SS=15)
  adc.begin();
  // Specify if needed or to override default
  // ex: Feather 32u4 (SS=17) or M0 (SS=16), default SS not broken out
  //adc.begin(10);  

  // Software SPI (specify all, use any available digital)
  //          sck, mosi, miso, cs
  //adc.begin(13,    11,   12, 10);  
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
