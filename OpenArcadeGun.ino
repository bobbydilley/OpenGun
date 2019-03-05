/*
  OpenArcadeGun Version 0.1
  This project aims to write firmware for the Arduino Uno to track an infrared cameras pointing position towards a screen to allow the user to play arcade games.
*/

const int referencePoints = 10; // The amount of points around the screen, clockwise, 0,0 being top left.

void setup() {
  Serial.begin(9600);
}

void loop() {
    
}

void getQuadFromPoints(double x[], double y[]) {
  for(int i = 0 ; i < referencePoints ; i++) {
    Serial.println(x[i]);
    Serial.println(y[i]);
  }
}



double transform(double Qx[], double Qy[], double x, double y) {
  return 0.0;
}
