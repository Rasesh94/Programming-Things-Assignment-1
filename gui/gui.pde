import processing.serial.*;
Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port

void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
    size(200,200); //make our canvas 200 x 200 pixels big

}

void draw()
{
  if (mousePressed == true) 
  {                           //if we clicked in the window
   myPort.write('1');         //send a 1
   println("1");   
  } else 
  {                           //otherwise
  myPort.write('0');          //send a 0
  }   
}