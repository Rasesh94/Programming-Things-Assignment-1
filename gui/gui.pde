/* =========================================================
 * ====                   WARNING                        ===
 * =========================================================
 * The code in this tab has been generated from the GUI form
 * designer and care should be taken when editing this file.
 * Only add/edit code inside the event handlers i.e. only
 * use lines between the matching comment tags. e.g.

 void myBtnEvents(GButton button) { //_CODE_:button1:12356:
     // It is safe to enter your event code here  
 } //_CODE_:button1:12356:
 
 * Do not rename this tab!
 * =========================================================
 */

public void button1_click1(GButton source, GEvent event) { //_CODE_:button1:939351:
  println("button1 - GButton >> GEvent." + event + " @ " + millis());

} //_CODE_:button1:939351:

synchronized public void win_draw1(PApplet appc, GWinData data) { //_CODE_:main:328568:
  appc.background(230);
} //_CODE_:main:328568:

public void textarea1_change1(GTextArea source, GEvent event) { //_CODE_:txtIncoming:409478:
  println("txtIncoming - GTextArea >> GEvent." + myString + " @ " + millis());
} //_CODE_:txtIncoming:409478:

public void W_click1(GButton source, GEvent event) { //_CODE_:W:256229:
 myPort.write("W");
   txtIncoming.appendText("Sent to Robot: " + "Manually moving forward.");
} //_CODE_:W:256229:

public void A_click1(GButton source, GEvent event) { //_CODE_:A:747617:
 myPort.write("A");
  txtIncoming.appendText("Sent to Robot: " + "Manually moving left.");
} //_CODE_:A:747617:

public void S_click1(GButton source, GEvent event) { //_CODE_:S:290439:
  myPort.write("S");
   txtIncoming.appendText("Sent to Robot: " + "Manually moving back.");
} //_CODE_:S:290439:

public void D_click1(GButton source, GEvent event) { //_CODE_:D:324130:
 myPort.write("D");
  txtIncoming.appendText("Sent to Robot: " + "Manually moving right.");
} //_CODE_:D:324130:

public void btnComplete_click(GButton source, GEvent event) { //_CODE_:btnComplete:200569:
  myPort.write("C");
  txtIncoming.appendText("Sent to Robot: " + "Corner Complete.");
} //_CODE_:btnComplete:200569:

public void btnRoom_click(GButton source, GEvent event) { //_CODE_:btnRoom:583555:
    myPort.write("R");
  txtIncoming.appendText("Sent to Robot: " + "Outside Room.");
} //_CODE_:btnRoom:583555:

public void btnScan_click(GButton source, GEvent event) { //_CODE_:btnScan:446119:
      myPort.write("STOP");
  txtIncoming.appendText("Sent to Robot: " + "Stop Robot.");
} //_CODE_:btnScan:446119:

public void btnAuto_click(GButton source, GEvent event) { //_CODE_:btnAuto:764531:
    myPort.write("S");
  txtIncoming.appendText("Sent to Robot: " + "Auto navigation initiated.");
} //_CODE_:btnAuto:764531:

public void btnStop_click(GButton source, GEvent event) { //_CODE_:btnStop:521789:
      myPort.write("stop");
  txtIncoming.appendText("Sent to Robot: " + "Stop Robot.");
} //_CODE_:btnStop:521789:

public void button2_click1(GButton source, GEvent event) { //_CODE_:button2:560600:
  println("button2 - GButton >> GEvent." + event + " @ " + millis());
} //_CODE_:button2:560600:

public void btnLeft(GButton source, GEvent event) { //_CODE_:btnRight:203449:
  println("btnRight - GButton >> GEvent." + event + " @ " + millis());
} //_CODE_:btnRight:203449:

public void textarea1_change2(GTextArea source, GEvent event) { //_CODE_:txtCurrentLocation:331829:
  println("txtCurrentLocation - GTextArea >> GEvent." + event + " @ " + millis());
} //_CODE_:txtCurrentLocation:331829:



// Create all the GUI controls. 
// autogenerated do not edit
public void createGUI(){
  G4P.messagesEnabled(false);
  G4P.setGlobalColorScheme(GCScheme.BLUE_SCHEME);
  G4P.setCursor(ARROW);
  surface.setTitle("Sketch Window");
  button1 = new GButton(this, 110, -60, 80, 30);
  button1.setText("W");
  button1.addEventHandler(this, "button1_click1");
  main = GWindow.getWindow(this, "Search and Rescue Robot", 0, 0, 500, 300, JAVA2D);
  main.noLoop();
  main.addDrawHandler(this, "win_draw1");
  txtIncoming = new GTextArea(main, 33, 47, 434, 70, G4P.SCROLLBARS_VERTICAL_ONLY);
  txtIncoming.setOpaque(true);
  txtIncoming.addEventHandler(this, "textarea1_change1");
  W = new GButton(main, 230, 141, 80, 30);
  W.setText("W");
  W.addEventHandler(this, "W_click1");
  A = new GButton(main, 150, 180, 80, 30);
  A.setText("A");
  A.addEventHandler(this, "A_click1");
  S = new GButton(main, 230, 180, 80, 30);
  S.setText("S");
  S.addEventHandler(this, "S_click1");
  D = new GButton(main, 310, 180, 80, 30);
  D.setText("D");
  D.addEventHandler(this, "D_click1");
  sketchPad1 = new GSketchPad(main, 262, 122, 80, 60);
  btnComplete = new GButton(main, 30, 220, 160, 50);
  btnComplete.setText("Complete");
  btnComplete.setTextBold();
  btnComplete.addEventHandler(this, "btnComplete_click");
  btnRoom = new GButton(main, 330, 220, 142, 41);
  btnRoom.setText("Room");
  btnRoom.setTextBold();
  btnRoom.addEventHandler(this, "btnRoom_click");
  btnScan = new GButton(main, 220, 220, 80, 50);
  btnScan.setText("Scan");
  btnScan.addEventHandler(this, "btnScan_click");
  btnAuto = new GButton(main, 380, 139, 80, 30);
  btnAuto.setText("Corridor");
  btnAuto.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  btnAuto.addEventHandler(this, "btnAuto_click");
  btnStop = new GButton(main, 60, 140, 80, 30);
  btnStop.setText("STOP");
  btnStop.setLocalColorScheme(GCScheme.RED_SCHEME);
  btnStop.addEventHandler(this, "btnStop_click");
  label1 = new GLabel(main, 20, 30, 80, 20);
  label1.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label1.setText("GUI");
  label1.setOpaque(false);
  button2 = new GButton(main, 330, 265, 56, 30);
  button2.setText("Left");
  button2.addEventHandler(this, "button2_click1");
  btnRight = new GButton(main, 406, 266, 66, 30);
  btnRight.setText("Right");
  btnRight.addEventHandler(this, "btnLeft");
  txtCurrentLocation = new GTextArea(main, 200, 11, 270, 29, G4P.SCROLLBARS_NONE);
  txtCurrentLocation.setOpaque(true);
  txtCurrentLocation.addEventHandler(this, "textarea1_change2");
  main.loop();
}

// Variable declarations 
// autogenerated do not edit
GButton button1; 
GWindow main;
GTextArea txtIncoming; 
GButton W; 
GButton A; 
GButton S; 
GButton D; 
GSketchPad sketchPad1; 
GButton btnComplete; 
GButton btnRoom; 
GButton btnScan; 
GButton btnAuto; 
GButton btnStop; 
GLabel label1; 
GButton button2; 
GButton btnRight; 
GTextArea txtCurrentLocation; 