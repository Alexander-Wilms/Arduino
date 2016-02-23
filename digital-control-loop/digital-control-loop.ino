int w = 0;
int e = 0;
int e_alt = 0;
int y = 0;
int y_alt = 0;
int x = 0;
int wPin = A0;
int yPin = 3; // Pin muss PWM unterstützen
int xPin = A1;
int t_alt;
int rechenzeit;
int c1 = 5;
int c2 = 3;

void setup() {
  pinMode(wPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, OUTPUT);
}

void loop() {
  // aktuelle Zeit speichern
  t_alt = millis();

  // Sollgröße messen
  w = analogRead(wPin);

  // Stellgröße messen
  x = analogRead(xPin);

  // Stellfehler berechnen
  e_alt = e;
  e = w - x;

  // PI-Algorithmus, um Stellgröße zu berechnen
  y_alt = y;
  y = y_alt + c1*e - c2*e_alt;

  // Stellgröße ausgeben
  analogWrite(yPin, y);

  // warten, bis TA vergangen ist
  rechenzeit = millis() - t_alt;
  delay(1000-rechenzeit);
}
