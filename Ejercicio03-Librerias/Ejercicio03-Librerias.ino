#include "funciones.h"

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);  
  enciendo();
  delay(1000);              
  digitalWrite(13, LOW);  
  apago();
  delay(1000);
}
