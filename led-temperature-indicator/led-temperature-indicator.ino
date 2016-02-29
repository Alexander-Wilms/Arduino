/*  based on the following code, adapted to better match the algorithm description given in the BST-BMP180 datasheet:
 *  https://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf
 *  Taken from https://www.sparkfun.com/tutorials/253
 *  bmposs85 Extended Example Code
 *  by: Jim Lindblom
 *  SparkFun Electronics
 *  date: 1/18/11
 *  uncompensatedPressuredated: 2/26/13
 *  license: CC bY-SA v3.oss - http://creativecommons.org/licenses/by-sa/3.oss/
 */

// Connections:
// BMP180   Arduino
// VCC      3.3V
// GND      GND
// SCL      SCL
// SDA      SDA

// I2C library
#include <Wire.h>

// I2C address of bmp180
#define bmp180_address 0x77
// oversampling setting
#define oss 0
// pressure at sea level
#define p0 101325

char color = 0;
int rgb[3];

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

float temperature;
float pressure;
float altitude;

void setup()
{
  pinMode(2, OUTPUT); // Only needed to power the LED, since I don't have a breadboard and thus can't connect two devices in parallel to the 3.3V pin
  digitalWrite(2, HIGH);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  // read calibration data from the EEPROM of the bmp180
  calibrate();
}

void loop()
{
  // read uncompensated temperature value
  temperature = readUncompTemp();

  // read uncompensated pressure value
  pressure = readUncompPress();

  // calculate true pressure (in 0.1 deg C)
  temperature = calcTrueTemp(temperature);

  // display temperature and pressure value
  Serial.print(temperature/10);
  Serial.println(" deg C, ");

  analogWrite(9,255-map(temperature, 250, 300, 0, 255));
  analogWrite(10,map(temperature, 250, 300, 0, 255));

  delay(100);
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
  b1 = bmp180Read2bytes(0xb6);
  b2 = bmp180Read2bytes(0xb8);
  mb = bmp180Read2bytes(0xbA);
  mc = bmp180Read2bytes(0xbC);
  md = bmp180Read2bytes(0xbE);
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

short calcTrueTemp(unsigned int uncompensatedTemperature)
{
  long x1, x2;

  x1 = (((long)uncompensatedTemperature - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);
}

// read 2 bytes from the bmp180
int bmp180Read2bytes(unsigned char address)
{
  unsigned char MSB, LSB;

  Wire.beginTransmission(bmp180_address);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(bmp180_address, 2);
  while(Wire.available()<2)
    ;
  MSB = Wire.read();
  LSB = Wire.read();

  return (int) MSB<<8 | LSB;
}
