/*
  OpenArcadeGun Version 0.1
  This project aims to write firmware for the Arduino Uno to track an infrared cameras pointing position towards a screen to allow the user to play arcade games.
  This attempts to simulate the popular Sega I/O Gun boards
  Requirements:
    10 x IR LEDs
     1 x DFRobot I2C IR Positional Camera

  Theroy:
    0. The 10 LEDs are placed around the screen in the following pattern:
      x  x  x  x
      x        x
      x  x  x  x
    1. Each LED is flashed in turn, and its position recorded on the camera while its on.
    2. Now that we have the position of each 10 LEDs around the screen, we can attempt to compute the screens size/warp/position from the information we have
    3. Once we have the position of the screen quad, we can attempt to map it to a regular 1x1 square
    4. If we apply this same mapping to the middle of the camera 0.5, then we get the position on the screen that the camera is pointing at.
*/

// Constants:
const int referencePoints = 10; // The amount of points around the screen, clockwise, 0,0 being top left.
const int pointDelayMs = 100;   // The time to wait for before measuring the position of the sensor

// Variables:
int points[referencePoints][2]; // The position of the points around the screen
int quad[4][2]; // The point sof the quad

void setup() {
  Serial.begin(9600);

  /* Setup the LEDs for output */
  for(int i = 2 ; i < referencePoints + 2 ; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

/* Reset all points back to -1 to signal they haven't been found */
void clearPoints() {
  for(int i = 0 ; i < referencePoints ; i++) {
    points[i][0] = -1;
    points[i][1] = -1;
  }
  quad[0][0] = -1;
  quad[0][1] = -1;
  quad[1][0] = -1;
  quad[1][1] = -1;
  quad[2][0] = -1;
  quad[2][1] = -1;
  quad[3][0] = -1;
  quad[3][1] = -1;
}

void loop() {
  clearPoints();
  grabPointData();
  getQuadFromPoints();
  double Qx[4];
  double Qy[4];
  for(int i = 0 ; i < 4 ; i++) {
    Qx[i] = (double) quad[i][0];
    Qy[i] = (double) quad[i][1];
  }
  double screenPoints[2];
  transform(Qx, Qy, 0.5, 0.5, screenPoints);
  sendScreenPoints(screenPoints[0], screenPoints[1]);
}

/* Send the points we beleive the screen to be at through the serial, scaled to 255 */
void sendScreenPoints(double x, double y) {
  Serial.print(x * (double) 255);
  Serial.print(",");
  Serial.println(y * (double) 255);
}

/* Attempt to get the surrounding quad from all the points in different ways*/
void getQuadFromPoints() {
  // Check if we have the entire square
  if(points[0][0] != -1 && points[2][0] != -1 && points[5][0] != -1 && points[7][0] != -1) {
    quad[0][0] = points[0][0];
    quad[0][1] = points[0][1];
    quad[1][0] = points[2][0];
    quad[1][1] = points[2][1];
    quad[2][0] = points[5][0];
    quad[2][1] = points[5][1];
    quad[3][0] = points[7][0];
    quad[3][1] = points[7][1];
    return;
  }

  // Check if we have the left sub square

  // Check if we have the middle sub square vertical

  // Check if we have the right sub square

  // Check if we have the top sub square

  // Check if we have the middle horizontal sub square

  // Check if we have the bottom sub square

  // Check if we have the top left triangle

  // Check if we have the bottom left triangle

  // Check if we have the top right triangle

  // Check if we have the bottom right triangle
}

/* Go through each LED and flash them in turn recording their position */
void grabPointData() {
  for(int i = 2 ; i < referencePoints + 2 ; i++) {
    digitalWrite(i, HIGH);
    delay(pointDelayMs);
    int capturedPoints[2];
    capturePoint(capturedPoints);
    points[i][0] = capturedPoints[0];
    points[i][1] = capturedPoints[1];
    digitalWrite(i, LOW);
  }
}

/* Capture the point from I2C */
void capturePoint(int* capturePoints) {
  capturePoints[0] = 0;
  capturePoints[1] = 0;
}

/* Transform X and Y to a unit square between 0 -> 1, for the warped quad defined in Qx, Qy */
void transform(double Qx[], double Qy[], double x, double y, double* output) {
  double ax = (x - Qx[0]) + (Qx[1] - Qx[0]) * (y - Qy[0]) / (Qy[0] - Qy[1]);
  double a3x = (Qx[3] - Qx[0]) + (Qx[1] - Qx[0]) * (Qy[3] - Qy[0]) / (Qy[0] - Qy[1]);
  double a2x = (Qx[2] - Qx[0]) + (Qx[1] - Qx[0]) * (Qy[2] - Qy[0]) / (Qy[0] - Qy[1]);
  double ay = (y - Qy[0]) + (Qy[3] - Qy[0]) * (x - Qx[0]) / (Qx[0] - Qx[3]);
  double a1y = (Qy[1] - Qy[0]) + (Qy[3] - Qy[0]) * (Qx[1] - Qx[0]) / (Qx[0] - Qx[3]);
  double a2y = (Qy[2] - Qy[0]) + (Qy[3] - Qy[0]) * (Qx[2] - Qx[0]) / (Qx[0] - Qx[3]);
  double bx = x * y - Qx[0] * Qy[0] + (Qx[1] * Qy[1] - Qx[0] * Qy[0]) * (y - Qy[0]) / (Qy[0] - Qy[1]);
  double b3x = Qx[3] * Qy[3] - Qx[0] * Qy[0] + (Qx[1] * Qy[1] - Qx[0] * Qy[0]) * (Qy[3] - Qy[0]) / (Qy[0] - Qy[1]);
  double b2x = Qx[2] * Qy[2] - Qx[0] * Qy[0] + (Qx[1] * Qy[1] - Qx[0] * Qy[0]) * (Qy[2] - Qy[0]) / (Qy[0] - Qy[1]);
  double by = x * y - Qx[0] * Qy[0] + (Qx[3] * Qy[3] - Qx[0] * Qy[0]) * (x - Qx[0]) / (Qx[0] - Qx[3]);
  double b1y = Qx[1] * Qy[1] - Qx[0] * Qy[0] + (Qx[3] * Qy[3] - Qx[0] * Qy[0]) * (Qx[1] - Qx[0]) / (Qx[0] - Qx[3]);
  double b2y = Qx[2] * Qy[2] - Qx[0] * Qy[0] + (Qx[3] * Qy[3] - Qx[0] * Qy[0]) * (Qx[2] - Qx[0]) / (Qx[0] - Qx[3]);
  output[0] = (ax / a3x) + (1.0 - a2x / a3x) * (bx - b3x * ax / a3x) / (b2x - b3x * a2x / a3x);
  output[1] = (ay / a1y) + (1.0 - a2y / a1y) * (by - b1y * ay / a1y) / (b2y - b1y * a2y / a1y);
}
