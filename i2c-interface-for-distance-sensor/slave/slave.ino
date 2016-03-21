// HC-SR04
// Connect VCC to 5V

#define Trig 3
#define Echo 2
int distance;
int pulselength;
int t1, t2;
int delta_t;

#include <Wire.h>
int x;
char c;

void setup() {
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
  Serial.println("Slave");
}

void loop() {
  delay(100);
}

void receiveEvent(int bytes)
{
  while(Wire.available() != 0)
  {
      x = Wire.read();
      Serial.print("Received: ");
      Serial.println(x);
  }
  if(x == 't')
  {
    digitalWrite(Trig, HIGH);
    delay(1);
    digitalWrite(Trig, LOW);
  
    // measure pulse length in microseconds
    pulselength = pulseIn(Echo, HIGH);
    // microseconds to meters
    distance = abs(pulselength) / 10000. * 343.5 / 2.;
  }
}

void requestEvent()
{
  Serial.print("Send: ");
  Serial.println(distance);
  Wire.write(distance);
}
