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
float HCSresistor = 47000;

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

double convertReadingToVoltage(int reading)
{
  double readDoub = (double)reading;
  double output = (readDoub/1023)*5;
  return(output);
}

double convertDACreadingToVoltage(int reading)
{ 
  double readDoub = (double)reading;
  double output = (readDoub/4096)*5;
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

double convertToBaseCurrent(int value_base)
{
  double baseVoltage = convertDACreadingToVoltage(value_base);
  double baseCurrent = (baseVoltage*1000000)/HCSresistor;
  return(baseCurrent);
}

void sendBaseCurrentAndEndCurve(float baseCurrent)
{
  Serial.print("L");
  Serial.print(",");
  Serial.println(baseCurrent);
}


void loop() 
{
  for(int value_base = 0; value_base < 4096; value_base = value_base + 256)
  {
    for(int value_collect = 0; value_collect < 4095; value_collect = value_collect + 39)
    {
      myDacTop.analogWrite(odd, buffered, gain, active, value_base);
      myDacBot.analogWrite(odd, buffered, gain, active, value_collect);
      double VCE = convertReadingToVoltage(analogRead(A0));
      double VCC = convertReadingToVoltage(analogRead(A1));
      double ICE = (VCC-VCE)*1000000/VCCr;
      //debugPrint(VCE, ICE);
      sendOverSerial(VCE, ICE);
      delay(20);
    }
    delay(10);
    double base_current = convertToBaseCurrent(value_base);
    sendBaseCurrentAndEndCurve(base_current);
    delay(50);
  }
}
