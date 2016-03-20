
#include <Wire.h>
int x;
char c;
void setup() {
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
}

void requestEvent()
{
  Serial.print("Send: ");
  Serial.println(x*2);
  Wire.write(x*2);
}
