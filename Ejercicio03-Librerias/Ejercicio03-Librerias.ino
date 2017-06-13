#include "funciones.h"

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);  
  enciendo();
  delay(1000);              
  digitalWrite(led, LOW);  
  apago();
  delay(1000);
}
