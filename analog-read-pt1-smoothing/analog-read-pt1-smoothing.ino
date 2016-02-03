/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

int smoothedvalue = 0;
// sampling rate of the ADC, value taken from https://sites.google.com/site/measuringstuff/the-arduino
float TA = 0.04;
// time constant of the PT1, results in a rather smooth plot
int T1 = 1;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println(0);Serial.println(600); 
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);

  // smooth values using a PT1, useful when observing PWM output
  smoothedvalue = (int)(T1/(T1+TA)*smoothedvalue+TA/(T1+TA)*sensorValue);
  
  // print out the value you read:
  Serial.println(smoothedvalue);
  delay(1);        // delay in between reads for stability
}
