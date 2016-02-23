// y[2] = y_k-2
int y[3] = {0};
int u[3] = {0};
// sampling rate of the ADC, value taken from https://sites.google.com/site/measuringstuff/the-arduino
float TA = 0.04;
// time constant of the PT1, results in a rather smooth plot: T1=1
int T1 = 1;
int T2 = 5;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  u[0] = analogRead(A0);

  // P with Kp = 1
  // y[0] = u[0];

  // smooth values using a PT1, useful when observing PWM output
  y[0] = (int)(T1/(T1+TA)*y[1]+TA/(T1+TA)*u[0]);

  // PT2
  // y[0]=(u[0]-(T1*T2)*(-y[1]+y[2])-(T1+T2)*(-y[1]))/(T1*T2+(T1+T2)+TA*TA);

  // print out the value you read:
  Serial.println(y[0]);

  y[2]=y[1];
  y[1]=y[0];
  u[2]=u[1];
  u[1]=u[0];

  delay(1);        // delay in between reads for stability
}
