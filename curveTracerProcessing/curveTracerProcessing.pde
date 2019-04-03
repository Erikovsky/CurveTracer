import processing.serial.*;

Serial myPort;                       // The serial port
boolean handshake = false;        // Whether we've heard from the microcontroller
int[] serialInArray = new int[2];    // Where we'll put what we receive
int curvePointCount = 0;  // A count of how many bytes we receive
int serialCount;
int xpos, ypos;
int xprev, yprev = 0;
int offsetX = 10;
int offsetY = 10;
int sizeX = 266;
int sizeY = 266;
int curvePoints = 15;
boolean newData = false;

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
  noStroke();      // No border on the next thing drawn


  // Print a list of the serial ports, for debugging purposes:
  printArray(Serial.list());

  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  generateCleanGraph();
}


void draw() {
   if(newData)
   {
     newData = false;
     stroke(0);
     strokeWeight(2);
     drawLineInBounds(xprev,yprev,xpos,ypos);
     xprev = xpos;
     yprev = ypos;
   }
}

void serialEvent(Serial myPort) {
  int inByte = myPort.read();
  if ((handshake == false) && (inByte == 'A')) 
  {
    myPort.clear();         
    handshake = true;  
    myPort.write((byte)curvePoints); 
    xprev = 0;
    yprev = 0;
    return;
  } 
  else
  {
    serialCount = curvePointCount%2;
    serialInArray[serialCount] = inByte;
    if (serialCount != 0) 
    {
      xpos = serialInArray[0];
      ypos = serialInArray[1];
      println(xpos + "\t" + ypos);
      newData = true;
      serialCount = 0;
    }
    curvePointCount++;
  }

}
