/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

char color = 0;
int rgb[3];

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  if (Serial.available() > 0)
  {
    color = Serial.read();
  }

  int val = analogRead(A0);
  switch(color){
    case 'r':
      analogWrite(9,255-map(val, 0, 1023, 0, 255));
      break;
    case 'g':
      analogWrite(10,255-map(val, 0, 1023, 0, 255));
      break;
    case 'b':
      analogWrite(11,255-map(val, 0, 1023, 0, 255));
      break;
  }
}
