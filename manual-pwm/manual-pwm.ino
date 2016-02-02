/*
 Fade

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 The analogWrite() function uses PWM, so if
 you want to change the pin you're using, be
 sure to use another PWM capable pin. On most
 Arduino, the PWM pins are identified with 
 a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

 This example code is in the public domain.
 */

int led = 13;           // the pin the LED is attached to
int j = 0;
float duty = 0;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);
  delay(20.*duty);
  digitalWrite(led, LOW);
  delay(20.*(1-duty));

  duty = (sin(j*3.1415/180)+1)/2;
  j++;
}
