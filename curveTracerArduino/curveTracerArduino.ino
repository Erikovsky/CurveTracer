#define BASE_STEPS 5 //amount of base currents OR gate voltages
#define VCC_STEPS 5 //amount of VCC steps
#define DAC_SIZE 8 //How many bits the dac is
int sensorValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sendEndOfGraph();
}


void sendDataPoint(int x, int y)
{
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
}

void sendEndOfCurve()
{
  Serial.println("L");
}

void sendEndOfGraph()
{
  Serial.println("E");
}

void loop() {
 int totalSteps = pow(2,DAC_SIZE);
  int baseStepSize = totalSteps/BASE_STEPS;
  int vccStepSize = totalSteps/VCC_STEPS;

  int maxVoltage = 9;
  for(int vcc = 0; vcc < 200; vcc=vcc+2)
  {
    sensorValue = analogRead(A0);
    sendDataPoint(vcc,sensorValue);
    delay(100);
  }
  sendEndOfCurve();
}
