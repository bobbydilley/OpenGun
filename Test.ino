// Wii Remote IR sensor  test sample code  by kako http://www.kako.com
// modified output for Wii-BlobTrack program by RobotFreak http://www.letsmakerobots.com/user/1433
// modified for http://DFRobot.com by Lumi, Jan. 2014

#include <Wire.h>

int IRsensorAddress = 0xB0;
//int IRsensorAddress = 0x58;
int slaveAddress;

boolean ledState = false;
byte data_buf[16];
int i;

int Ix[4];
int Iy[4];
int s;

void Write_2bytes(byte d1, byte d2)
{
    Wire.beginTransmission(slaveAddress);
    Wire.write(d1); Wire.write(d2);
    Wire.endTransmission();
}

void setup()
{
    slaveAddress = IRsensorAddress >> 1;   // This results in 0x21 as the address to pass to TWI
    Serial.begin(38400);
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    digitalWrite(3, LOW);
    digitalWrite(5, LOW);

    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);

    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    
    
    Wire.begin();
    // IR sensor initialize
    Write_2bytes(0x30,0x01); delay(10);
    Write_2bytes(0x30,0x08); delay(10);
    Write_2bytes(0x06,0x90); delay(10);
    Write_2bytes(0x08,0xC0); delay(10);
    Write_2bytes(0x1A,0x40); delay(10);
    Write_2bytes(0x33,0x33); delay(10);
    delay(100);
}
void loop()
{
  int x = 0;
  int y = 0;

  digitalWrite(13, LOW);
  digitalWrite(2, HIGH);
  delay(10);
  readSingle(&x, &y);
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");

  digitalWrite(2, LOW);
  digitalWrite(4, HIGH);
  delay(10);
  readSingle(&x, &y);
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");

  digitalWrite(4, LOW);
  digitalWrite(12, HIGH);
  delay(10);
  readSingle(&x, &y);
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");

  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  delay(10);
  readSingle(&x, &y);
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.println();
  
}

void readSingle(int* x, int* y) {
   //IR sensor read
      Wire.beginTransmission(slaveAddress);
      Wire.write(0x36);
      Wire.endTransmission();
  
      Wire.requestFrom(slaveAddress, 16);        // Request the 2 byte heading (MSB comes first)
      for (i=0;i<16;i++) { data_buf[i]=0; }
      i=0;
      while(Wire.available() && i < 16) { 
          data_buf[i] = Wire.read();
          i++;
      }
  
      int mainX = data_buf[1];
      int mainY = data_buf[2];
      s   = data_buf[3];
      mainX += (s & 0x30) <<4;
      mainY += (s & 0xC0) <<2;
      
      *x = mainX;
      *y = mainY;
}

