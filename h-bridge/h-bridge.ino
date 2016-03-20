int speed;
int t = 0;
bool DIR = HIGH;

void setup() {
  digitalWrite(2, HIGH);
}

void loop() {
  speed = 32 * (sin(t/180./4) + 1);
  analogWrite(3,speed);
  digitalWrite(4, DIR);
  digitalWrite(5, !DIR);
  if(speed == 0)
    DIR = !DIR;
  t++;
}
