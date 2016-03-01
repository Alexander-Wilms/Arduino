int x_out = 128;
int y[3] = {0};
int x_in[3] = {0};
// sampling rate of the ADC, value taken from https://sites.google.com/site/measuringstuff/the-arduino
// float TA = 0.04;
float TA = 0.04;
// time constant of the PT1, results in a rather smooth plot: T1=1
int T1 = 3;
int t = 0;

void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  x_out = 127 * (sin(t/180.*2) + 1);
  Serial.print(x_out);
  Serial.print(",");
  
  analogWrite(5, x_out);
  
  // read the input on analog pin 0:
  x_in[0] = analogRead(A0);
  Serial.print(x_in[0]/4);
  Serial.print(",");
  
  // smooth values using a PT1, useful when observing PWM output
  y[0] = (int)(T1/(T1+TA)*y[1]+TA/(T1+TA)*x_in[0]);

  // print out the value you read:
  Serial.println(y[0]/4);

  y[2]=y[1];
  y[1]=y[0];
  x_in[2]=x_in[1];
  x_in[1]=x_in[0];

  delay(1);        // delay in between reads for stability
  t++;
}
