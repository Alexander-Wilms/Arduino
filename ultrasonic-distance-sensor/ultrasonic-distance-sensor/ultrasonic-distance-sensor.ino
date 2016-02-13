// HC-SR04
// Connect VCC to 5V

#define Trig 3
#define Echo 2
float distance;
int pulselength;

void setup() {
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(Trig, HIGH);
  delay(1);
  digitalWrite(Trig, LOW);

  pulselength = pulseIn(Echo, HIGH);
  // ms to cm
  distance = pulselength / 29 / 2;

  Serial.println(pulselength);
  Serial.println(distance);

  delay(1000);
}
