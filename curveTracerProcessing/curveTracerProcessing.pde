//Electronics I Final Project
//Tim Roche, Fowad Sohail, Nick Kabala

import processing.serial.*;

Serial myPort;                       // The serial port
String data;
float[] values = new float[2];
float xpos, ypos;
float xprev, yprev = 0;
float sizeX = 1000;
float sizeY = 1000;
float offsetX = (int)(sizeX*0.1);
float offsetY = (int)(sizeY*0.1);
float sizeYComp = sizeY-offsetY;
float sizeXComp = sizeX-offsetX;
float maxY = 10000;
float maxX = 5;

int dataPoint = 0;
boolean newData = false;
boolean firstRun = true;
boolean clearGraph = false;

void generateCleanGraph()
{
  background(255);
  stroke(0);
  strokeWeight(3);
  line(offsetX,offsetY,offsetX,sizeY-offsetY);
  line(offsetX,sizeY-offsetY,sizeX-offsetX,sizeY-offsetY);
}

void drawLineInBounds(float xp,float yp,float x,float y)
{
  //map(value, fromLow, fromHigh, toLow, toHigh)
  xp = map(xp, 0, maxX, offsetX, sizeXComp);
  yp = map(yp, 0, maxY, offsetY, sizeYComp);
  x = map(x, 0, maxX, offsetX, sizeXComp);
  y = map(y, 0, maxY, offsetY, sizeYComp);
  float invert_yp = sizeY - yp;
  float invert_y = sizeY - y;
  stroke(96, 154, 247);
  strokeWeight(2);
  line(xp,invert_yp,x,invert_y);
}

void setup() {
  size(1000, 1000);  // Stage size
  smooth();  
  noStroke();      // No border on the next thing drawn
  printArray(Serial.list());
  generateCleanGraph();
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw() 
{
  try
  {
    if(newData)
    {
      if(!firstRun)
      {
        stroke(0);
        strokeWeight(2);
        drawLineInBounds(xprev, yprev, values[0], values[1]);
        newData = false;
      }
      else
      {
        firstRun = false;
      }
      xprev = values[0];
      yprev = values[1];
    }
    if(clearGraph)
    {
      generateCleanGraph();
      clearGraph = false;
    }
  }
  catch(RuntimeException e) {
    // only if there is an error:
    e.printStackTrace();
  }   
}

void serialEvent(Serial myPort) 
{
  try{
    data = myPort.readStringUntil('\n');
    data = trim(data);
    println(data);
    if(data == null)
    {
      return;
    }
    if(data.equals("E"))
    {
      clearGraph = true;
      println("END");
    }
    if((data.equals("L"))||(data.equals("E")))
    {
      firstRun = true;
      println("Reseting Run!!!!");  
    }
    else 
    {
        values = float(splitTokens(data, ",")); // delimiter can be comma space or tab
        if (values.length == 2) 
        {
          //println(values[0] + "\t" + values[1]);  
          newData = true;
        }
    }  
  }  
  catch(RuntimeException e) {
    // only if there is an error:
    e.printStackTrace();
  }
   
}
