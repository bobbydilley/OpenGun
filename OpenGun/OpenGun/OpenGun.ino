#include "MPU9250.h"
#define M_PI 3.14159265359      
#define dt 0.01 

MPU9250 IMU(Wire,0x68);
int status;

float left_x = 0;
float right_x = 0;
float top_y = 0;
float bottom_y = 0;

float pitch = 1.0;
float roll = 1.0;

void setup() {
  // serial to display data
  Serial.begin(9600);

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  //Serial.println("Move the mag in a figure of 8.");
  //status = IMU.calibrateMag();

  IMU.setMagCalX(1.25, 1.03);
  IMU.setMagCalY(19.07, 0.99);
  IMU.setMagCalZ(-4.06, 0.98);
}

void loop() {
  IMU.readSensor();
  
  float acData[3];
  acData[0] = IMU.getAccelX_mss();
  acData[1] = IMU.getAccelY_mss();
  acData[2] = IMU.getAccelZ_mss();
  
  float gyData[3];
  gyData[0] = IMU.getGyroX_rads();
  gyData[1] = IMU.getGyroY_rads();
  gyData[2] = IMU.getGyroZ_rads();
  
  int compass = getCompassCourse(
      IMU.getAccelX_mss(),
      IMU.getAccelY_mss(),
      IMU.getAccelZ_mss(),
      IMU.getMagX_uT(),
      IMU.getMagY_uT(),
      IMU.getMagZ_uT()
   );

   complementaryFilter(acData, gyData, &pitch, &roll);
   
   Serial.print("Compass: ");
   Serial.print(compass);
   Serial.print("Pitch: ");
   Serial.print(pitch);
   Serial.print("Roll: ");
   Serial.println(roll);
  
  delay(100);
}

int getCompassCourse(int ax, int ay, int az, int cx, int cy, int cz) {
  float xh, yh, ayf, axf;
  ayf = ay / 57.0;
  axf = ax / 57.0;
  xh=cx*cos(ayf)+cy*sin(ayf)*sin(axf)-cz*cos(axf)*sin(ayf);
  yh=cy*cos(axf)+cz*sin(axf);
  int var_compass=atan2((double)yh,(double)xh) * (180 / PI) -90; // angle in degrees
  if (var_compass>0) {
    var_compass=var_compass-360;
  }
  var_compass=360+var_compass;
  return var_compass;
}

void complementaryFilter(float accData[3], float gyrData[3], float *pitch, float *roll) {
    float pitchAcc, rollAcc;               
 
    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *pitch += gyrData[0] * dt; // Angle around the X-axis
    *roll -= gyrData[1] * dt;    // Angle around the Y-axis
 
    // Compensate for drift with accelerometer data if !bullshit
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
    {
        // Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / M_PI;
        *pitch = *pitch * 0.98 + pitchAcc * 0.02;
 
        // Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2f((float)accData[0], (float)accData[2]) * 180 / M_PI;
        *roll = *roll * 0.98 + rollAcc * 0.02;
    }
} 

