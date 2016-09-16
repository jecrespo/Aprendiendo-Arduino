#include <Servo.h>

int posicion = 0;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  if (Serial.available() > 0) {
    String grados = "";
    do {
      grados = grados + (char)Serial.read();
      Serial.println(grados);
      delay(5);
    }
    while (Serial.available() > 0);
    Serial.println(grados);
      posicion = grados.toInt();
      myservo.write(posicion);
  }
}

