// Connections:
// SI7021   Arduino
// VIN      3.3V
// GND      GND
// SCL      SCL
// SDA      SDA

#define si7021_address 0x40
#include <Wire.h>
unsigned char MSB, LSB;
unsigned int RH_Code;
int relative_humidity;
unsigned int Temp_Code;
int temperature;
int t1;
int t2;
int delta_t;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  t1 = millis();
  
  // START and slave address
  Wire.beginTransmission(si7021_address);
  // WRITE and byte to be written
  Wire.write(0xE5);
  Wire.endTransmission();

  // Repeated START
  Wire.requestFrom(si7021_address, 2);
  while(Wire.available()<2)
    ;

  // READ and ACK
  MSB = Wire.read();
  // READ and ACK
  LSB = Wire.read();

  /*Serial.println("MSB and LSB: ");
  Serial.print(MSB);
  Serial.print(" ");
  Serial.println(LSB);*/

  RH_Code = MSB << 8 | LSB;
  /*Serial.println("RH_Code: ");
  Serial.println(RH_Code);*/
  
  relative_humidity = ((125.*RH_Code)/65536.)-6.;
  Serial.print("Relative humidity: ");
  Serial.println(relative_humidity);

  // START and slave address
  Wire.beginTransmission(si7021_address);
  // WRITE and byte to be written
  Wire.write(0xE0);
  Wire.endTransmission();

  // Repeated START
  Wire.requestFrom(si7021_address, 2);
  while(Wire.available()<2)
    ;

  // READ and ACK
  MSB = Wire.read();
  // READ and ACK
  LSB = Wire.read();

  /*Serial.println("MSB and LSB: ");
  Serial.print(MSB);
  Serial.print(" ");
  Serial.println(LSB);*/

  Temp_Code = MSB << 8 | LSB;
  /*Serial.println("Temp_Code: ");
  Serial.println(Temp_Code);*/
  
  temperature = ((175.72*Temp_Code)/65536.)-46.85;
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.println();

  t2 = millis();
  delta_t = t2 - t1;
  
  // 1 measurement per second
  delay(1000-delta_t);
}
