/* Ejercicio22-ChatSerie para el curso #aprendiendoarduino
 No todos los pines soportam interrupciones de cambio
 The circuit: 
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
 */

// Ojo a las limitaciones y temas de interrupciones!!!
// http://arduino.cc/en/Reference/SoftwareSerial
// Basado en: http://www.instructables.com/id/Simple-Chat-Program-with-Arduino-Uno/?ALLSTEPS


#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

String inputString = "";         // donde recibo los datos del softwareSerial
String outputString = "";		 // donde mando los datos al softwareSerial
String inputStringDec = "";
unsigned long previousmillis = 0;

void setup() {
  Serial.begin(9600);
  //Por defecto el �ltimo puerto inicializado est� escuchando, como solo usamos uno, no es ncesario usar la funci�n listen()
  mySerial.begin (4800);	//probar a poner velocidades diferentes en cada punto.
  // reserve 200 bytes for the inputString:
  outputString.reserve(200);	//M�ximo de 200 caracteres que podemos trabajar con este programa
}

void loop()
{

  int tiempoLoop = millis()-previousmillis;
  previousmillis = millis();
  if (tiempoLoop > 250){
    Serial.print("Tiempo de Loop: ");
    Serial.println(tiempoLoop);
  }

  /*	Versi�n b�sica
   
   #include <SoftwareSerial.h>
   SoftwareSerial mySerial(10, 11); // RX, TX
   
   void setup() {
   Serial.begin(9600);
   mySerial.begin (4800);	//probar a poner velocidades diferentes en cada punto.
   }
   void loop(){
   if (mySerial.available()){
   Serial.print((char)mySerial.read());
   }
   if (Serial.available()){
   mySerial.print((char)Serial.read());
   }
   }
   
   CON LCD
   lcd.autoscroll();	//Para que haga el scroll
   if (mySerial.available()){
   	char inChar = (char)mySerial.read();
   	Serial.write(inChar);
   	lcd.print(inChar);		//Solo con version LCD
   }
   if (Serial.available()){
   	char outChar = (char)Serial.read();
   	mySerial.write(outChar);
   	lcd.print(outChar);	//Solo con version LCD
   }
   */

  inputString = "";	//inicalizo el String
  outputString = "";
  inputStringDec = "";

  //////////////////// Leo del serie
  if (mySerial.available()){
    while (mySerial.available()) {	//Primero en el loop miro lo que entra por el serie conectado al otro Arduino
      //leo el nuevo caracter 
      char inChar = (char)mySerial.read();
      delay(20);
      //if (inChar == '\n') break;	//salgo del while si detecto un salto de l�nea.
      //lo a�ado al String 
      inputString += inChar;
      inputStringDec += int(inChar);
      inputStringDec += '-';
      if (inputString.length() > 100) break;  //Salgo del bucle e imprimo si tengo muchos caracteres, por ejemplo infinito
    }
    //if (inputString.length() > 30) Serial.println("Has recibido un mensaje muy largo");	//Me protejo ante ataques
    //else {
      Serial.print("Leido por puerto serie ");
      Serial.print(inputString.length());
      Serial.print(" caracteres. Texto: ");
      Serial.println(inputString);	//Escribo en consola todo lo recibido.
      Serial.print("Leido en base DEC: ");
      Serial.println(inputStringDec);
    //}
  }

  /////////////////// Leo de la consola
  if (Serial.available()){
    while (Serial.available()){
      char outChar = (char)Serial.read();
      delay(5);
      //if (outChar == '\n') break;	//salgo del while si detecto un salto de l�nea.
      outputString += outChar;
    }

    Serial.print("Leido por consola: ");
    Serial.print(outputString.length());
    Serial.print(" caracteres. Texto: ");
    Serial.println(outputString);	//Escribo en consola todo lo recibido.

    if (outputString == "repite"){
      String repiteString = "";
      Serial.println("Cuantas veces?");
      while (Serial.available() == 0){
      }	// No hago nada hasta que no respondo a la pregunta
      while (Serial.available()){		//Espero una segunda cadena
        char outChar = (char)Serial.read();
        delay(5);
        repiteString += outChar;
      }
      Serial.println(repiteString);
      int repito = repiteString.toInt();
      if (repito <= 0){
        Serial.println("No es un valor correcto, repito 10 veces..");
        repito = 10;
      }

      for (int i=0; i<repito ;i++){	//repito 10 veces la segunda cadena
        mySerial.println(outputString);
      }
    }
    else if (outputString == "ataca"){
      String atacaString = "";
      Serial.println("Que cadena quieres repetir 100 veces?");
      while (Serial.available() == 0){
      }	// No hago nada hasta que no respondo a la pregunta
      while (Serial.available()){		//Espero una segunda cadena
        char outChar = (char)Serial.read();
        delay(5);
        atacaString += outChar;
      }
      Serial.println(atacaString);
      for (int i=0; i<100;i++){
        mySerial.println(atacaString);
      }
    }
    else if (outputString == "infinito"){
      while(true){
        mySerial.println("bucle infinito");
      }
    }
    else mySerial.println(outputString);
  }
}

