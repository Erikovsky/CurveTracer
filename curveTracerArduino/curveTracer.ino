#define BASE_STEPS 5 //amount of base currents OR gate voltages
#define VCC_STEPS 5 //amount of VCC steps
#define DAC_SIZE 8 //How many bits the dac is

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  establishContact();
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

void sendDataPoint(int x, int y)
{
  Serial.write(x);
  Serial.write(y);
}

void sendEndOfCurve()
{
  Serial.write('E');
}

void loop() {
 /* int totalSteps = pow(2,DAC_SIZE);
  int baseStepSize = totalSteps/BASE_STEPS;
  int vccStepSize = totalSteps/VCC_STEPS;

  int maxVoltage = 9;
  for(int vcc = 0; vcc < totalSteps; vcc=vcc+vccStepSize)
  {
    for(int base = 0; base < totalSteps; base=base+baseStepSize)
    {
      //This is where the reads would go
      sendDataPoint(base,vcc);
      delay(100);
    }
  }*/
  for(int x = 0; x < 250; x=x+10)
  {
    for(int y = 0; y < 250; y=y+10)
    {
      sendDataPoint(x,y);
      delay(100);
    }
  }
  sendEndOfCurve();
}
