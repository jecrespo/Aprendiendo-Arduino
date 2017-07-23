/*
    Test simple para comprobar funcionamiento de un coche con
    dos motores DC y un  driver L298
*/

#define PIN_IN1 11
#define PIN_IN2 10
#define PIN_IN3 9
#define PIN_IN4 6

void setup() {
  Serial.begin(9600);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
}

void loop() {
  char comando = '0';

  Serial.println(F("\n------------------------"));
  Serial.println(F("MENU"));
  Serial.println(F("Selecciona acción ejecutar"));
  Serial.println(F("F - Forward"));
  Serial.println(F("B - Backward"));
  Serial.println(F("R - Right"));
  Serial.println(F("L - Left"));
  Serial.println(F("S - Stop"));

  while (Serial.available() == 0) {
    //Nada
  }

  if (Serial.available() > 0) {
    comando = Serial.read();
    Serial.print("\nHe Leido la instrucción: " + (String)comando);
  }

  if (comando == 'F') { //Forward
    Serial.println(comando);
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  }
  else if (comando == 'B') { //Backward
    Serial.println(comando);
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
  }
  else if (comando == 'R') { //Right
    Serial.println(comando);
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  }
  else if (comando == 'L') { //Left
    Serial.println(comando);
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
  }
  else if (comando == 'S') { //Stop
    Serial.println(comando);
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
  }
  else
    Serial.println("Comando incorrecto!!!");
}
