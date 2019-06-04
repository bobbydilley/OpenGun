#include <util/atomic.h>

const byte ledPin = 13;
const byte hsyncPin = 3;
const byte vsyncPin = 2;
const byte gunPin = 8;
const byte gunPowerPin = 7;
volatile int line = 0;
volatile int bob = 0;

unsigned long time1 = 0.0;
unsigned long time2 = 0.0;

unsigned long time1m = 0.0;
unsigned long time2m = 0.0;

volatile unsigned int blob = 0;
volatile unsigned int blob_overflows = 0;

volatile unsigned int gun_blob = 0;
volatile unsigned int gun_blob_overflows = 0;
volatile unsigned int gun_lines = 0;

volatile unsigned int overflows = 0;

volatile unsigned int av_blob = blob;

void setup() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1C = 0;
  TIMSK1 = _BV(TOIE1);  // Enable Interrupt
  
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  
  pinMode(hsyncPin, INPUT_PULLUP);
  pinMode(vsyncPin, INPUT_PULLUP);

  pinMode(gunPowerPin, OUTPUT);
  digitalWrite(gunPowerPin, HIGH);

  pinMode(gunPin, INPUT);   // set Pin as Input (default)
  digitalWrite(gunPin, HIGH);  // enable pullup resistor
  pciSetup(gunPin);
  
  attachInterrupt(digitalPinToInterrupt(hsyncPin), hsync, FALLING);
  attachInterrupt(digitalPinToInterrupt(vsyncPin), vsync, RISING);
}

void loop() {

  unsigned int local_gun_lines;
  unsigned int local_lines = 0;
  unsigned long local_blob_overflows;
  unsigned long local_gun_blob_overflows;
  unsigned long local_gun_blob;
  unsigned long local_blob;
  
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    local_lines = bob;
    local_gun_lines = gun_lines;
    local_blob_overflows = blob_overflows;
    local_gun_blob_overflows = gun_blob_overflows;
    local_gun_blob = gun_blob;
    local_blob = blob;
    
  }
  av_blob = (float) ((float) 0.8 * float(av_blob)) + ((float) 0.2 * (float) local_blob);
  unsigned long ticks = ((((unsigned long) local_blob_overflows) << 16) | (unsigned long) av_blob);
  unsigned long gun_ticks = ((((unsigned long) local_gun_blob_overflows) << 16) | (unsigned long) local_gun_blob);
  unsigned long x = 255.00 - ((float) ((float) gun_ticks / (float) ticks) * (float) 255.00);
  unsigned long y = (float) ((float) gun_lines / (float) local_lines) * (float) 255.00;
  
  Serial.print(gun_ticks);
  Serial.print("/");
  Serial.print(ticks);
  Serial.print(", ");
  Serial.print(gun_lines);
  Serial.print("/");
  Serial.print(local_lines);
  Serial.print(" - ");
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
  
  
  delay(50);
}

void vsync() {
  gun_blob = TCNT1;
      gun_blob_overflows = overflows;
      gun_lines = line;
}

void hsync() {
  TCCR1B = 0;
  blob = TCNT1;
  blob_overflows = overflows;
  TCNT1 = 0;
  overflows = 0;
  line++;
  TCCR1B = 1;
}

ISR(TIM1_OVF_vect)
{
  overflows++;
}//end ISR


ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
{    
    if(digitalRead(gunPin) == 1) {
    
      time2m = time1m;
        time1m = micros();
        bob = line;
        line = 0;
      
    }
    
}

void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}


