// Connections:
// SI7021   Arduino
// VIN      3.3V
// GND      GND
// SCL      SCL
// SDA      SDA

 // Connections:
 // BMP180   Arduino
 // VCC      3.3V
 // GND      GND
 // SCL      SCL
 // SDA      SDA

#define si7021_address 0x40
#include <Wire.h>
unsigned char MSB, LSB;
unsigned int RH_Code;
int relative_humidity;
unsigned int Temp_Code;
float temperature;
int t1;
int t2;
int delta_t;

// I2C address of bmp180
#define bmp180_address 0x77
// oversampling setting
#define oss 0
// pressure at sea level
#define p0 101325

// calibration coefficients (2 bytes each)
short ac1;
short ac2;
short ac3;
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1;
short b2;
short mb;
short mc;
short md;

// b5 is calculated in calcTrueTemp(), this variable is also used in calcTruePress()
long b5;

float pressure;
float altitude;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  // read calibration data from the EEPROM of the bMP180
  calibrate();
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
  Serial.print(relative_humidity);
  Serial.println(" %");

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
  Serial.print(temperature);
  Serial.println(" deg C");

  // read uncompensated pressure value
  pressure = readUncompPress();

  // calculate true pressure (in Pa)
  pressure = calcTruePress(pressure);

  // display pressure value
  Serial.print("Air pressure: ");
  Serial.print(pressure/100);
  Serial.println(" hPa");
  Serial.println();
  
  t2 = millis();
  delta_t = t2 - t1;

  // 1 measurement per second
  delay(1000-delta_t);
}


// read 1 byte from the bmp180 at 'address'
char bmp180Readbyte(unsigned char address)
{
  unsigned char data;

  // send address of the slave device we want to talk to 
  Wire.beginTransmission(bmp180_address);
  // queue 1 byte for transmission
  // it's the memory address we want to read from
  Wire.write(address);
  // transmit queued byte
  Wire.endTransmission();

  // request 1 byte from slave device
  Wire.requestFrom(bmp180_address, 1);
  // wait for byte to be received from the slave
  while(!Wire.available())
    ;

  // return received byte
  return Wire.read();
}

// read 2 bytes from the bmp180
int bmp180Read2bytes(unsigned char address)
{
  unsigned char MSB, LSB;

  Wire.beginTransmission(bmp180_address);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(bmp180_address, 2);
  // wait for 2 bytes to be received
  while(Wire.available()<2)
    ;

  // store the queued bytes
  MSB = Wire.read();
  LSB = Wire.read();

  return (int) MSB<<8 | LSB;
}

void calibrate()
{
  // read uncompensatedTemperature EEPROM registers, 16 bit, MSB first
  // variable names must be lowercase, otherwise the compiler thinks it's a hex value
  ac1 = bmp180Read2bytes(0xAA);
  ac2 = bmp180Read2bytes(0xac);
  ac3 = bmp180Read2bytes(0xAE);
  ac4 = bmp180Read2bytes(0xb0);
  ac5 = bmp180Read2bytes(0xb2);
  ac6 = bmp180Read2bytes(0xb4);
  b1  = bmp180Read2bytes(0xb6);
  b2  = bmp180Read2bytes(0xb8);
  mb  = bmp180Read2bytes(0xbA);
  mc  = bmp180Read2bytes(0xbC);
  md  = bmp180Read2bytes(0xbE);
}

long readUncompTemp()
{
  long uncompensatedTemperature;

  // write 0x2E into register 0xF4
  Wire.beginTransmission(bmp180_address);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // wait at least 4.5ms
  delay(5);

  // read registers 0xF6 (MSB) and 0xF7 (LSB)
  uncompensatedTemperature = bmp180Read2bytes(0xF6);
  return uncompensatedTemperature;
}

long readUncompPress()
{
  unsigned char MSB, LSB, XLSB;
  long uncompensatedPressure = oss;

  Wire.beginTransmission(bmp180_address);
  Wire.write(0xF4);
  Wire.write(0x34);
  Wire.endTransmission();

  // wait for conversion, delay time dependent on oss
  delay(5);

  // read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(bmp180_address);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(bmp180_address, 3);

  // wait for the 3 bytes to become available
  while(Wire.available() < 3)
    ;

  MSB = Wire.read();
  LSB = Wire.read();
  XLSB = Wire.read();

  uncompensatedPressure = (((unsigned long) MSB << 16) | ((unsigned long) LSB << 8) | (unsigned long) XLSB) >> (8);

  return uncompensatedPressure;
}

short calcTrueTemp(unsigned int uncompensatedTemperature)
{
  long x1, x2;

  x1 = (((long)uncompensatedTemperature - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);
}

long calcTruePress(unsigned long uncompensatedPressure)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;

  // Calculate b3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<oss) + 2)>>2;

  // Calculate b4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(uncompensatedPressure - b3) * (50000>>oss));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  return p;
}
