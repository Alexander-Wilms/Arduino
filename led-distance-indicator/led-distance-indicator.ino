// HC-SR04
// Connect VCC to 5V

#define Trig 3
#define Echo 2
float distance;
float distance_cm;
int pulselength;
int t1, t2;
int delta_t;

char color = 0;
int rgb[3];

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
  distance_cm = abs(pulselength) / 10000. * 343.5 / 2.;

  Serial.print(distance);
  Serial.println(" m");

  distance_cm = constrain(distance_cm,0,30);
  analogWrite(9,255-map(distance_cm, 0, 30, 0, 255));
  analogWrite(10,map(distance_cm, 0, 30, 0, 255));
  
  t2 = millis();
  delta_t = t2 - t1;
  // Serial.print("delta_t: ");
  // Serial.println(delta_t);

  // delta_t is at most ca. 180
  delay(200-delta_t);
}
