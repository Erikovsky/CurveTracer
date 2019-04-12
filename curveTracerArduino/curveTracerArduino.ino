#include <SPI.h>
#include <MCP492X.h>

#define PIN_SPI_CHIP_SELECT_DAC_TOP 10 // Or any pin you'd like to use
#define PIN_SPI_CHIP_SELECT_DAC_BOT 9 // Or any pin you'd like to use

MCP492X myDacTop(PIN_SPI_CHIP_SELECT_DAC_TOP);
MCP492X myDacBot(PIN_SPI_CHIP_SELECT_DAC_BOT);

bool odd = false;
bool buffered = true;
bool gain = true;
bool active = true;
unsigned int value_base = 10; 
unsigned int value_collect = 2048; 
float VCCr = 470;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sendEndOfGraph();
  int sensorValue;


  myDacTop.begin();
  myDacBot.begin();
}


void sendEndOfCurve()
{
  Serial.println("L");
}

void sendEndOfGraph()
{
  Serial.println();
  Serial.println("E");
}

void mapToPins(int input)
{
  byte bottom = ((byte)input);
  for(int bitNum = 0; bitNum < 8; ++bitNum)
  {
    digitalWrite(bitNum+2, bitRead(bottom,bitNum));
  }
}

double convertReadingToVoltage(int reading)
{
  double readDoub = (double)reading;
  double output = (readDoub/1023)*5;
  return(output);
}

void debugPrint(float VCE, float ICE)
{
    Serial.print("VOLTAGE: ");
    Serial.print(VCE);
    Serial.println(" Volts");
    Serial.print("CURRENT: ");
    Serial.print(ICE);
    Serial.println(" uA");
}

void sendOverSerial(float VCE, float ICE)
{
  Serial.print(VCE);
  Serial.print(",");
  Serial.println(ICE);
}

void loop() 
{
  for(int value_base = 0; value_base < 4096; value_base = value_base + 256)
  {
    for(int value_collect = 0; value_collect < 4096; value_collect = value_collect + 32)
    {
      myDacTop.analogWrite(odd, buffered, gain, active, value_base);
      myDacBot.analogWrite(odd, buffered, gain, active, value_collect);
      float VCE = convertReadingToVoltage(analogRead(A0));
      float VCC = convertReadingToVoltage(analogRead(A1));
      float ICE = (VCC-VCE)*1000000/VCCr;
      //debugPrint(VCE, ICE);
      sendOverSerial(VCE, ICE);
      delay(20);
    }
    delay(50);
    sendEndOfCurve();
  }
}
