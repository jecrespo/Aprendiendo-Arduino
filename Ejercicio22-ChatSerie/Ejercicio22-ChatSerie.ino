/* Ejercicio22-ChatSerie para el curso #aprendiendoarduino
  No todos los pines soportam interrupciones de cambio
  The circuit:
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)
*/

// Ojo a las limitaciones y temas de interrupciones!!!
// http://arduino.cc/en/Reference/SoftwareSerial
// Basado en: http://www.instructables.com/id/Simple-Chat-Program-with-Arduino-Uno/?ALLSTEPS

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Chat Program...");
  mySerial.begin (4800);	//probar a poner velocidades diferentes en cada punto.
  delay(1000);
  mySerial.println("Hola, acabo de iniciar el chat");
}
void loop() {	//en cada loop leo un caracter si hay en alguno de los bufferes
  if (mySerial.available()) {
    Serial.print((char)mySerial.read());
  }
  if (Serial.available()) {
    mySerial.print((char)Serial.read());
  }
}