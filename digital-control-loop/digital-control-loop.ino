int w = 0;
int e = 0;
int e_alt = 0;
int y = 0;
int y_alt = 0;
int x = 0;
int x_alt = 0;
int wOutPin = 5;
int wInPin = A0;
int yOutPin = 6; // Pin muss PWM unterstützen
int xInPin = A1;
int t_alt;
int rechenzeit;

bool stepped = 0;

// Abtastzeit
float ta = 0.1;

// Parameter der simulierten Strecke
int t1 = 2;

// Koeffizienten des PI-Reglers
// Mit Simulink PID Tuner bestimmt
float Kp = 3.37;
float Ki = 1.4;
float c1 = Kp+(Ki*ta)/2;
float c2 = Kp-(Ki*ta)/2;

void setup() {
  Serial.begin(9600);
  pinMode(wInPin, INPUT);
  pinMode(wOutPin, OUTPUT);
  pinMode(xInPin, INPUT);
  pinMode(yOutPin, OUTPUT);
  analogWrite(wOutPin, 100);
}

void loop() {
  // aktuelle Zeit speichern
  t_alt = millis();

  // Sollgröße messen
  // w = analogRead(wInPin);
  // Sollgröße vorgeben
  
//  Serial.print("w ");
  
  if(millis()<=1000)
  {
    Serial.print(0);
  }
  else if(millis()>1000 && !stepped)
  {
    // digitalWrite(wOutPin,HIGH);
    // y ist durch den DAC begrenzt, also darf w nicht allzu groß sein
    // analogWrite(wOutPin,128);
    // w = analogRead(wInPin);
    stepped = true;
  } else {
    w = 1000;//analogRead(wInPin);
  }

  Serial.print(w);

  // Stellgröße messen
  // x = analogRead(xInPin);

  // simulierte Strecke: PT1-Algorithmus
  x_alt = x;
  x = (t1/(t1+ta))*x_alt+(ta/(t1+ta))*y;

//  Serial.print(", x ");
  Serial.print(", ");
  Serial.print(x);

  // Stellfehler berechnen
  e_alt = e;
  e = w - x;
//Serial.print(", e ");
  Serial.print(", ");
  Serial.print(e);

  // ACHTUNG: Der Arduino-eigene DAC kann nur positive Werte für y ausgeben!
  // Dies macht ein direktes Regeln unmöglich
  
  // Regler: PI-Algorithmus, um Stellgröße zu berechnen
  y_alt = y;
  y = y_alt + c1*e - c2*e_alt;
  
  // Wenn man nur die Strecke betrachten möchte
  // y = w;

//  Serial.print(", y ");
  Serial.print(", ");
  Serial.println(y);

  // Stellgröße ausgeben
  analogWrite(yOutPin, y);

  // warten, bis TA vergangen ist
  rechenzeit = millis() - t_alt;
  //Serial.println(rechenzeit);
  delay(100-rechenzeit);
}
