//Electronics I Final Project
//Tim Roche, Fowad Sohail, Nick Kabala

import processing.serial.*;

Serial myPort;                       // The serial port
String data;
int[] values = new int[2];
int xpos, ypos;
int xprev, yprev = 0;
int offsetX = 10;
int offsetY = 10;
int sizeX = 266;
int sizeY = 266;
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

void drawLineInBounds(int xp,int yp,int x,int y)
{
  int invert_yp = sizeY - yp;
  int invert_y = sizeY - y;
  stroke(96, 154, 247);
  strokeWeight(2);
  line(xp+offsetX,invert_yp-offsetY,x+offsetX,invert_y-offsetY);
}

void setup() {
  size(266, 266);  // Stage size
  smooth();  
  noStroke();      // No border on the next thing drawn
  printArray(Serial.list());
  generateCleanGraph();
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw() 
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
        values = int(splitTokens(data, ",")); // delimiter can be comma space or tab
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
