// Example by Tom Igoe
// Modified for http://www.DFRobot.com by Lumi, Jan. 2014

/*
   This code should show one colored blob for each detected IR source (max four) at the relative position to the camera.
*/

import processing.serial.*;

int lf = 10;    // Linefeed in ASCII
String myString = null;
Serial myPort;  // The serial port

void setup() {
  // List all the available serial ports
  println(Serial.list()[1]);
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[1], 38400);
  myPort.clear();
  // Throw out the first reading, in case we started reading 
  // in the middle of a string from the sender.
  myString = myPort.readStringUntil(lf);
  myString = null;
  size(800,800);
  //frameRate(30);
}

void draw() {
  background(77);
  //while (myPort.available() > 0) {
    myString = myPort.readStringUntil(lf);
    if (myString != null) {
      
      int[] output = {0,0,0,0,0,0,0,0,0,0};
      
      
      while(output[0] == 0 && output[9] == 0) {
        output = int (split(myString, ','));
      }
      
      println(myString); // display the incoming string
         
      int xx = output[0];
      int yy = output[1];
      
      int ww = output[2];
      int zz = output[3];
      
      int xxx = output[4];
      int yyy = output[5];
      
      int www = output[6];
      int zzz = output[7];
      
      int aa = output[8];
      int bb = output[9];

      ellipseMode(RADIUS);  // Set ellipseMode to RADIUS
      fill(255, 0, 0);  // Set fill to white
      ellipse(xx, yy, 20, 20);
      
      ellipseMode(RADIUS);  // Set ellipseMode to RADIUS
      fill(0, 255, 0);  // Set fill to white
      ellipse(ww, zz, 20, 20);
      
      ellipseMode(RADIUS);  // Set ellipseMode to RADIUS
      fill(0, 0, 255);  // Set fill to white
      ellipse(xxx, yyy, 20, 20);
      
      ellipseMode(RADIUS);  // Set ellipseMode to RADIUS
      fill(255);  // Set fill to white
      ellipse(www, zzz, 20, 20);
      
      ellipseMode(RADIUS);  // Set ellipseMode to RADIUS
      fill(255,255,0);  // Set fill to white
      ellipse(aa, bb, 10, 10);

    }
}
