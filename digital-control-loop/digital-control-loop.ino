int w = 0;
int wOutPin = 5;
int e = 0;
int e_alt = 0;
int y = 0;
int y_alt = 0;
int x = 0;
int x_alt = 0;
int wPin = A0;
int yPin = 3; // Pin muss PWM unterstützen
int xPin = A1;
int t_alt;
int rechenzeit;
int c1 = 5;
int c2 = 3;

void setup() {
  Serial.begin(9600);
  pinMode(wPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, OUTPUT);
  pinMode(wOutPin, OUTPUT);
  analogWrite(wOutPin, 100);
}

void loop() {
  // aktuelle Zeit speichern
  t_alt = millis();

  // Sollgröße messen
  // w = analogRead(wPin);
  // Sollgröße vorgeben
  if(millis()>1000)
    w = 1000;
  //Serial.print("w ");
  Serial.print(w);

  // Stellgröße messen
  // x = analogRead(xPin);
  // Drift verhindern
  if(x<5)
    x = 0;
  // simulierte Strecke: PT1-Algorithmus
  x_alt = x;
  x = 0.95*x_alt+0.05*y;
  //Serial.print(" x ");
  Serial.print(",");
  Serial.print(x);

  // Stellfehler berechnen
  e_alt = e;
  e = w - x;

  // Regler: PI-Algorithmus, um Stellgröße zu berechnen
  // ACHTUNG: DAC kann nur positive Werte für y ausgeben!
  y_alt = y;
  y = y_alt + c1*e - c2*e_alt;
  // Wenn man nur die Strecke betrachten möchte
  // y = e;

  //Serial.print(" y ");
  Serial.print(",");
  Serial.println(y/10);

  // Stellgröße ausgeben
  analogWrite(yPin, y);

  // warten, bis TA vergangen ist
  rechenzeit = millis() - t_alt;
  //Serial.println(rechenzeit);
  delay(100-rechenzeit);
}
