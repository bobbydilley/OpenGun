
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_9Axis_MotionApps41.h"
MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL

#define INTERRUPT_PIN 2 
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float yaw, pitch, roll;

int left_button_pin = 9; // Left button
int right_button_pin = 10; // right button
int leftClickFlag = 0;
const int sensitivity = 30;
float vertZero, horzZero;
float vertValue, horzValue;  // Stores current analog output of each axis

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() 
{
  Wire.begin();                                // join I2C bus (I2Cdev library doesn't do this automatically)
  Serial.begin(115200);                       // initialize serial communication
  while (!Serial);                            // wait for Leonardo enumeration, others continue immediately
  pinMode(INTERRUPT_PIN, INPUT);
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
   if (devStatus == 0) 
   {
      mpu.setDMPEnabled(true);                // turn on the DMP, now that it's ready
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();
      dmpReady = true;                        // set our DMP Ready flag so the main loop() function knows it's okay to use it
      packetSize = mpu.dmpGetFIFOPacketSize();      // get expected DMP packet size for later comparison
  } 
  else 
  {                                          // ERROR!        1 = initial memory load failed         2 = DMP configuration updates failed        (if it's going to break, usually the code will be 1)
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
  }
  pinMode(LED_PIN, OUTPUT);                 // configure LED for output
  pinMode(left_button_pin, INPUT);
  pinMode(right_button_pin, INPUT);
  yaw = 0.0;
  pitch = 0.0;
  roll = 0.0;
  vertZero = 0;
  horzZero = 0;
  Serial.println("Init okay");
}

uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };


// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {

    if (!dmpReady) return;                                                    // if programming failed, don't try to do anything
    mpuInterrupt = true;
    fifoCount = mpu.getFIFOCount();                                           // get current FIFO count 
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();        // wait for correct available data length, should be a VERY short wait
        mpu.getFIFOBytes(fifoBuffer, packetSize);                             // read a packet from FIFO
        fifoCount -= packetSize;  
        

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        yaw = ypr[1] /PI * 180;
        pitch = ypr[2] /PI * 180;
        roll = ypr[0] /PI * 180;
        //Serial.print(yaw);
        //Serial.print("\t");
        //Serial.print(pitch);
        //Serial.print("\t");
        //Serial.println(roll);

        mpu.dmpGetEuler(euler, &q);
        //Serial.print("\t");
        //Serial.print(euler[0] * 180/M_PI);
        //Serial.print("\t");
        //Serial.print(euler[1] * 180/M_PI);
        //Serial.print("\t");
        //Serial.println(euler[2] * 180/M_PI);

 
        teapotPacket[2] = fifoBuffer[0];
        teapotPacket[3] = fifoBuffer[1];
        teapotPacket[4] = fifoBuffer[4];
        teapotPacket[5] = fifoBuffer[5];
        teapotPacket[6] = fifoBuffer[8];
        teapotPacket[7] = fifoBuffer[9];
        teapotPacket[8] = fifoBuffer[12];
        teapotPacket[9] = fifoBuffer[13];
        Serial.write(teapotPacket, 14);
        teapotPacket[11]++; // packetCount, loops at 0xFF on purpose


        
        blinkState = !blinkState;                                             // blink LED to indicate activity
        vertValue = yaw - vertZero;
        horzValue = roll - horzZero;
        vertZero = yaw;
        horzZero = roll;   
        if (vertValue != 0) {
          //Serial.println(horzValue * sensitivity); 
          //Serial.print(", ");
          //Serial.println(vertValue * sensitivity);    
        }
        if (horzValue != 0) {
          //Serial.print(horzValue * sensitivity); 
          //Serial.print(", ");
          //Serial.println(vertValue * sensitivity);                                     // move mouse on x axis
        }
   
}
