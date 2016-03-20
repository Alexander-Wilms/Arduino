#include <Wire.h>

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
int K = 2;

// Parameter der simulierten Strecke
int t1 = 2;

// Koeffizienten des PI-Reglers
// Mit Simulink PID Tuner bestimmt
float Kp = 0.185457514040232;
float Ki = 1.55640804377475;
// Rechteckregel:
// float c1 = Kp;
// float c2 = Kp-Ki*ta;
// Trapezregel:
float c1 = Kp+(Ki*ta)/2;
float c2 = Kp-(Ki*ta)/2;

void setup() {
  Wire.begin(); // join i2c bus as master
  Serial.begin(9600);
  Serial.println("Master");
}

void loop() {
  // aktuelle Zeit speichern
  t_alt = millis();

  if(millis()<=1000)
  {
    Serial.print(0);
  }
  else if(millis()>1000 && !stepped)
  {
    stepped = true;
  } else {
    w = 64;
  }

  Serial.print(w);

  // Stellgröße messen
  Wire.requestFrom(8, 1);
  while(Wire.available()<1)
    ;

  x = Wire.read();

  Serial.print(", ");
  Serial.print(x);

  // Stellfehler berechnen
  e_alt = e;
  e = w - x;
  // Serial.print(", e ");
  Serial.print(", ");
  Serial.print(e);
  
  // Regler: PI-Algorithmus, um Stellgröße zu berechnen
  y_alt = y;
  y = y_alt + c1*e - c2*e_alt;
  
  // Wenn man nur die Strecke betrachten möchte
  // y = w;

  // Serial.print(", y ");
  Serial.print(", ");
  Serial.println(y);

  // Stellgröße ausgeben  
  Wire.beginTransmission(8);
  Wire.write(y);
  Wire.endTransmission();
   
  // warten, bis TA vergangen ist
  rechenzeit = millis() - t_alt;
  //Serial.println(rechenzeit);
  delay(100-rechenzeit);
}
