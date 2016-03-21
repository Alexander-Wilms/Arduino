/*
An Arduino code example for interfacing with the HMC5883 3-Axis Digital Compass

by: Jordan McConnell
SparkFun Electronics
created on: 6/30/11
license: OSHW 1.0, http://freedomdefined.org/OSHW

Analog input 4 I2C SDA
Analog input 5 I2C SCL
*/

#include <Wire.h> //I2C Arduino Library

#define address 0x1E //0011110b, I2C 7bit address of HMC5883

int aX,aY,aZ;

void setup(){
  //Initialize Serial and I2C communications
  Serial.begin(115200);
  Wire.begin();

  // http://forum.arduino.cc/index.php?topic=223345.0

  //Bypass Mode
  Wire.beginTransmission(0x68);
  Wire.write(0x37);
  Wire.write(0x02);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x68);
  Wire.write(0x6A);
  Wire.write(0x00);
  Wire.endTransmission();
  
  //Disable Sleep Mode
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void loop(){
 
  int x,y,z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
 

//Read data from each axis, 2 registers per axis
  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
 
  aX = (aX + x) / 2;
  aY = (aY + y) / 2;
  aZ = (aZ + z) / 2;
 
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(aX);
  Serial.print("  y: ");
  Serial.print(aY);
  Serial.print("  z: ");
  Serial.println(aZ);
 
  delay(250);
}

