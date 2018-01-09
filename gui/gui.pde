import processing.serial.*;

Serial myPort;  // Create object from Serial class

Button W,A,S,D,Room,Complete,Corridor;
  
void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
    size(400,400); //make our canvas 200 x 200 pixels big
  W = new Button("W",100,20,75,25);
  A = new Button("A",25,50,75,25);
  S = new Button("S",125,50,75,25);
  D = new Button("D",100,50,75,25);
}

void draw()
{
    background(300);

   W.Draw();
   A.Draw();
   S.Draw();
   D.Draw();

}
  /*void mousePressed(){
   if (b.MouseIsOver()){
   b.Clicked();
   }
  }*/
 
//button code from tutorial https://hadamlenz.wordpress.com/2014/07/16/building-a-button-in-processing/
class Button {
  String label; // button label
  float x;      // top left corner x position
  float y;      // top left corner y position
  float w;      // width of button
  float h;      // height of button
  
  
  // constructor
  Button(String labelB, float xpos, float ypos, float widthB, float heightB) {
    label = labelB;
    x = xpos;
    y = ypos;
    w = widthB;
    h = heightB;
    
  }
  
  void Draw() {
    smooth();
    fill(255);
    stroke(0);
    rect(x, y, w, h, 10);//draws the rectangle, the last param is the round corners
    fill(0);
    textSize(24); 
    text(label, x+w/2-(textWidth(label)/2), y+h/2+(textAscent()/2)); 
    //all of this just centers the text in the box
    
      if (this.MouseIsOver()){
         if(mousePressed){ myPort.write(EventSelector(label));}}
      
  }
  
  boolean MouseIsOver() {
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      return true;
    }
    return false;
  }
 
  String EventSelector(String button){
  String output = "";  
  switch (button){
            case "W":  output = "W";
                     break;
            case "A":  output = "A";
                     break;
            case "S":  output = "S";
                     break;
            case "D":  output = "D";
                     break;
            case "Complete":  output = "C";
                     break;
            case "Corridor":  output = "Co";
                     break;
            case "Room":  output = "R";
                     break;        
                 
  }
   return output;
  }
  
}


/*void update(int x, int y) {
  if ( overCircle(circleX, circleY, circleSize) ) {
    circleOver = true;
    rectOver = false;
  } else if ( overRect(rectX, rectY, rectSize, rectSize) ) {
    rectOver = true;
    circleOver = false;
  } else {
    circleOver = rectOver = false;
  }
}
boolean overCircle(int x, int y, int diameter) {
  float disX = x - mouseX;
  float disY = y - mouseY;
  if (sqrt(sq(disX) + sq(disY)) < diameter/2 ) {
    return true;
  } else {
    return false;
  }
}
void mousePressed() {
  if (circleOver) {
    currentColor = circleColor;
  }
  if (rectOver) {
    currentColor = rectColor;
  }
}*/