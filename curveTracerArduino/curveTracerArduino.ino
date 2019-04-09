#define BASE_STEPS 5 //amount of base currents OR gate voltages
#define VCC_STEPS 5 //amount of VCC steps
#define DAC_SIZE 8 //How many bits the dac is
#define CRESISTOR 470;
int sensorValue;


void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
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

double readVoltage()
{
  double VCE = convertReadingToVoltage(analogRead(A0));
  return (VCE); 
}

double readCurrent()
{
  int diff = analogRead(A0) - analogRead(A1);
  double Vresistor = convertReadingToVoltage(diff);
  double IC = Vresistor/470.0;
  return(IC);
}

void loop() {
 /*int totalSteps = pow(2,DAC_SIZE);
  int baseStepSize = totalSteps/BASE_STEPS;
  int vccStepSize = totalSteps/VCC_STEPS;

  int maxVoltage = 9;
  for(int vcc = 0; vcc < 200; vcc=vcc+2)
  {
    mapToPins(254);
    sensorValue = analogRead(A0);
    sendDataPoint(vcc,sensorValue);
    delay(100);
  }
  sendEndOfCurve();*/
  mapToPins(180);
 // Serial.println(analogRead(A0));
  
  Serial.print("VOLTAE: ");
  Serial.println(readVoltage());
  Serial.print("CURRENT: ");
  Serial.println(readCurrent());
  Serial.println("------");
  delay(10);
  
}
