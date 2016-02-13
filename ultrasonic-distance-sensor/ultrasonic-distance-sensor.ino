// HC-SR04
// Connect VCC to 5V

#define Trig 3
#define Echo 2
float distance;
int pulselength;
int t1, t2;
int delta_t;

void setup() {
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  t1 = millis();
  
  digitalWrite(Trig, HIGH);
  delay(1);
  digitalWrite(Trig, LOW);

  // measure pulse length in microseconds
  pulselength = pulseIn(Echo, HIGH);
  // microseconds to meters
  distance = abs(pulselength) / 1000000. * 343.5 / 2.;

  Serial.print(distance);
  Serial.println(" m");

  t2 = millis();
  delta_t = t2 - t1;
  // Serial.print("delta_t: ");
  // Serial.println(delta_t);

  // delta_t is at most ca. 180
  delay(200-delta_t);
}
