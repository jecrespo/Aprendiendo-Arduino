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

#define PIN_RX 2
#define PIN_TX 3

String inputString = "";         // donde recibo los datos del softwareSerial
String outputString = "";		 // donde mando los datos al softwareSerial
String inputStringDec = "";		//string entrante con los datos en decimal
unsigned long previousmillis = 0;

void setup() {
  Serial.begin(9600);
  //Por defecto el ultimo puerto inicializado esta escuchando, como solo usamos uno, no es ncesario usar la funcion listen() de software serial
  mySerial.begin (4800);	//probar a poner velocidades diferentes en cada punto.
  // reserve 200 bytes for the inputString:
  outputString.reserve(200);	//Maximo de 200 caracteres que podemos trabajar con este programa
}

void loop()
{
  int tiempoLoop = millis() - previousmillis;
  previousmillis = millis();
  if (tiempoLoop > 250) {	//como los loops pueden ser largos, imprimo por pantalla si es mayor de 250 ms.
    Serial.print("Tiempo de Loop: ");
    Serial.println(tiempoLoop);
  }

  inputString = "";	//inicalizo los String en cada loop
  outputString = "";
  inputStringDec = "";

  //////////////////// Leo del serie en cada loop el buffer completo
  if (mySerial.available()) {
    digitalWrite(PIN_RX,HIGH);  //Enciendo el led de recepción de paquetes por puerto SW serial
    while (mySerial.available()) {	//Primero en el loop miro lo que entra por el serie conectado al otro Arduino
      //leo el nuevo caracter
      char inChar = (char)mySerial.read();
      delay(20);
      //if (inChar == '\n') break;	//salgo del while si detecto un salto de linea si solo quisiera leer lineas
      inputString += inChar;
      inputStringDec += int(inChar);
      inputStringDec += '-';
      if (inputString.length() > 100) break;  //Salgo del bucle e imprimo si tengo muchos caracteres, por ejemplo infinito
    }
    digitalWrite(PIN_RX,LOW);
    Serial.print("Recibido por puerto serie ");
    Serial.print(inputString.length());
    Serial.print(" caracteres.\nTexto: ");
    Serial.println(inputString);	//Escribo en consola todo lo recibido.
    Serial.print("Leido en base DEC: ");
    Serial.println(inputStringDec);
  }

  /////////////////// Leo de la consola en cada loop el buffer completo
  if (Serial.available()) {
    while (Serial.available()) {
      char outChar = (char)Serial.read();
      delay(5);
      //if (outChar == '\n') break;	//salgo del while si detecto un salto de l�nea.
      outputString += outChar;
    }

    Serial.print("He escrito por consola: ");
    Serial.print(outputString.length());
    Serial.print(" caracteres.\nTexto: ");
    Serial.println(outputString);	//Escribo en consola todo lo recibido.

    if (outputString == "repite") {
      String repiteString = "";
      Serial.println("Cuantas veces repito?");
      while (Serial.available() == 0) {
      }	// No hago nada hasta que no respondo a la pregunta
      while (Serial.available()) {		//Espero una segunda cadena
        char outChar = (char)Serial.read();
        delay(5);
        repiteString += outChar;
      }
      Serial.println(repiteString);
      int repito = repiteString.toInt();
      if (repito <= 0) {
        Serial.println("No es un valor correcto, repito 10 veces..");
        repito = 10;
      }

      digitalWrite(PIN_TX,HIGH);  //Enciendo el led de transmisión de paquetes por puerto SW serial
	  for (int i = 0; i < repito ; i++) {	//repito 10 veces la segunda cadena
        mySerial.print(outputString);
      }
	  digitalWrite(PIN_TX,LOW);
    }
    else if (outputString == "ataca") {
      String atacaString = "";
      Serial.println("Que cadena quieres repetir 100 veces?");
      while (Serial.available() == 0) {
      }	// No hago nada hasta que no respondo a la pregunta
      while (Serial.available()) {		//Espero una segunda cadena
        char outChar = (char)Serial.read();
        delay(5);
        atacaString += outChar;
      }
      Serial.println(atacaString);
	  digitalWrite(PIN_TX,HIGH);
      for (int i = 0; i < 100; i++) {
        mySerial.print(atacaString);
      }
	  digitalWrite(PIN_TX,LOW);
    }
    else if (outputString == "infinito") {
	digitalWrite(PIN_TX,HIGH);
      while (true) {
        mySerial.println("bucle infinito");
      }
    }
    else {
	digitalWrite(PIN_TX,HIGH);
	mySerial.print(outputString);
	digitalWrite(PIN_TX,LOW);
  }
}
}
