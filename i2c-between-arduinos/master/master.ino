#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);           // start serial for output
  Serial.println("Master");
}

byte x = 0;
byte y = 0;

void loop() {
  Wire.beginTransmission(8); // transmit to device #
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  
  Serial.print("Send: ");
  Serial.println(x);
    
  Wire.requestFrom(8, 1);
  while(Wire.available()<1)
    ;

  y = Wire.read();
  Serial.print("Received: ");
  Serial.println(y);
   
  x++;
  delay(1000);
}
