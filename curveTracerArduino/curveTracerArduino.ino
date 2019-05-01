#include <SPI.h>
#include <MCP492X.h>


#define PIN_SPI_CHIP_SELECT_DAC_TOP 10 // Or any pin you'd like to use
#define PIN_SPI_CHIP_SELECT_DAC_BOT 9 // Or any pin you'd like to use
#define MODEPIN 3

MCP492X myDacTop(PIN_SPI_CHIP_SELECT_DAC_TOP);
MCP492X myDacBot(PIN_SPI_CHIP_SELECT_DAC_BOT);

bool odd = false;
bool buffered = true;
bool gain = true;
bool active = true;
bool MOSFETmode = false; 
unsigned int value_base = 10; 
unsigned int value_collect = 2048; 
float VCCr = 470;
float HCSresistor = 47000;
int MOSFETinc = 82;
int BJTinc = 256;
int baseIncSelect;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int sensorValue;
  pinMode(MODEPIN, INPUT);

  myDacTop.begin();
  myDacBot.begin();
  sendEndOfCurve();                                                                                                                                               
}


void sendEndOfCurve()
{
  Serial.println("L");
}

void sendEndOfGraph(bool MOSFETmode)
{
  Serial.print("E");
  Serial.print(",");
  Serial.println(MOSFETmode);
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

void sendBaseAndEndCurve(float baseCurrent, bool MOSFETmode)
{
  Serial.print("L");
  Serial.print(",");
  Serial.print(baseCurrent);
  Serial.print(",");
  Serial.println(MOSFETmode);
}

void makeCurve(float value_base, float maxBase, float baseIncSelect)
{
  for(value_base; value_base < maxBase; value_base = value_base + baseIncSelect)
  {
    for(int value_collect = 0; value_collect < 4095; value_collect = value_collect + 39)
    {
      myDacTop.analogWrite(odd, buffered, gain, active, value_base);
      myDacBot.analogWrite(odd, buffered, gain, active, value_collect);
      double VCE = convertReadingToVoltage(analogRead(A0));
      double VCC = convertReadingToVoltage(analogRead(A1));
      double ICE = (VCC-VCE)*1000000/VCCr; //Puts current in uA
      //debugPrint(VCE, ICE);
      sendOverSerial(VCE, ICE);
      delay(20);
    }
    delay(10);
    double baseSendValue;
    if(MOSFETmode)
    {
      baseSendValue = convertDACreadingToVoltage(value_base);
    }
    else
    {
      baseSendValue = convertToBaseCurrent(value_base);
    }
    sendBaseAndEndCurve(baseSendValue, MOSFETmode);
  }
}

void loop() 
{
  MOSFETmode = digitalRead(MODEPIN);
  sendEndOfGraph(MOSFETmode);
  if(MOSFETmode)
  {
    makeCurve(0, 1, MOSFETinc);
    makeCurve(820, 4096, MOSFETinc);  
  }
  else
  {
    makeCurve(0, 4096, BJTinc);
  }
  delay(50);
 }
