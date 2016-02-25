bool high = 0;
int k = 0;
float R = 220000;
float C = 0.000001;
float tau = R*C;

void setup() {
   Serial.begin(9600);
   pinMode(5, OUTPUT);
   pinMode(A0, INPUT);
}

void loop() {
  k = millis()/(4*tau*1000);
  //digitalWrite(5,k%2);
  analogWrite(5,255*(k%2));
  Serial.println(analogRead(A0));
}
