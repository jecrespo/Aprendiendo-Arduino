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

void setup() {
	Serial.begin(9600);
	//Por defecto el �ltimo puerto inicializado est� escuchando, como solo usamos uno, no es ncesario usar la funci�n listen()
	mySerial.begin (4800);	//probar a poner velocidades diferentes en cada punto.
	// reserve 200 bytes for the inputString:
	outputString.reserve(200);	//M�ximo de 200 caracteres que podemos trabajar con este programa
}

void loop()
{

/*	Versi�n b�sica
	if (mySerial.available())
		Serial.write(mySerial.read());
	if (Serial.available())
		mySerial.write(Serial.read());
*/
	
	inputString = "";	//inicalizo el String
	outputString = "";
	
//////////////////// Leo del serie
	while (mySerial.available()) {	//Primero en el loop miro lo que entra por el serie conectado al otro Arduino
		//leo el nuevo caracter 
		char inChar = (char)mySerial.read();
		//lo a�ado al String 
		inputString += inChar;
	}
	if (inputString.length() > 30) Serial.println("Has recibido un mensaje muy largo");
	else Serial.println(inputString);	//Escribo en consola todo lo recibido.
	
/////////////////// Leo de la consola
	while (Serial.available()){
		char outChar = (char)Serial.read();
		if (outChar == '\n') break;	//salgo del while si detecto un salto de l�nea.
		outputString += outChar;
	}
	
	if (outputString == "repite"){
        outputString = "";
		while (Serial.available()){		//Espero una segunda cadena
			char outChar = (char)Serial.read();
			outputString += outChar;
		}	
		for (int i=0; i<10;i++){	//repito 10 veces la segunda cadena
			mySerial.println(outputString);
		}
    }
    else if (outputString == "ataca"){
		outputString = "";
		while (Serial.available()){		//Espero una segunda cadena
			char outChar = (char)Serial.read();
			outputString += outChar;
		}
		int repeticion = outputString.toInt();
		if (repeticion > 50) {
			for (int i=0; i<repeticion;i++){
				mySerial.println("Esto es un ataque.........................................................!!!!!!!!!!!!!!!!!");
			}
		}
		else mySerial.println("Esto no es un ataque...");
	}
	else if (outputString == "infinito"){
		while(true){
			mySerial.println("bucle infinito");
		}
	}
    else mySerial.println(outputString);

}
