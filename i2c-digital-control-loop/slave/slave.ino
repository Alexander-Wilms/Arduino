
#include <Wire.h>
char c;

// Abtastzeit
float ta = 0.1;
int K = 2;
int y = 0;
int x = 0;
int x_alt = 0;

int t_alt;
int rechenzeit;

// Parameter der simulierten Strecke
int t1 = 2;

void setup() {
  Wire.begin(8);                // join i2c bus as slave with address 8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);
  Serial.println("Slave");
}

void loop() {
  // aktuelle Zeit speichern
  t_alt = millis();
  
  // simulierte Strecke: PT1-Algorithmus
  x_alt = x;
  x = (t1/(t1+ta))*x_alt+K*(ta/(t1+ta))*y;
  
  // warten, bis TA vergangen ist
  rechenzeit = millis() - t_alt;
  //Serial.println(rechenzeit);
  delay(100-rechenzeit);
}

void receiveEvent(int bytes)
{
  while(Wire.available() != 0)
  {
      y = Wire.read();
      Serial.print("Received: ");
      Serial.println(y);
  }
}

void requestEvent()
{
  Serial.print("Send: ");
  Serial.println(x);
  Wire.write(x);
}
