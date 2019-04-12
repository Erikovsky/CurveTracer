#include <SPI.h>
#include <MCP492X.h>


#define PIN_SPI_CHIP_SELECT_DAC_TOP 10 // Or any pin you'd like to use
#define PIN_SPI_CHIP_SELECT_DAC_BOT 9 // Or any pin you'd like to use

MCP492X myDacTop(PIN_SPI_CHIP_SELECT_DAC_TOP);
MCP492X myDacBot(PIN_SPI_CHIP_SELECT_DAC_BOT);

void setup() {
  // put your setup code here, to run once:
  myDacTop.begin();
  myDacBot.begin();
}

void loop() {
  bool odd = false;
  bool buffered = true;
  bool gain = true;
  bool active = true;
  unsigned int value_top = 2048; 
  unsigned int value_bottom = 1000; 
  // put your main code here, to run repeatedly:

  // Write any value from 0-4095
  //myDac.analogWrite(2048);
  myDacTop.analogWrite(odd, buffered, gain, active, value_top);
  myDacBot.analogWrite(odd, buffered, gain, active, value_bottom);
}
