#include <SoftwareSerial.h>
SoftwareSerial BT1(10, 11); // RX | TX
void setup()
  { pinMode(8, OUTPUT);        // Al poner en HIGH forzaremos el modo AT
    pinMode(9, OUTPUT);        // cuando se alimente de aqui
    digitalWrite(9, HIGH);
    delay (500) ;              // Espera antes de encender el modulo
    Serial.begin(9600);
    Serial.println("Levantando el modulo HC-05");
    digitalWrite (8, HIGH);    //Enciende el modulo
    Serial.println("Esperando comandos AT:");
    //BT1.begin(38400); //MODO AT (PULSAR BOTON AL REINICIAR)
    BT1.begin(9600);  //MODO BYTE STREAM
  }
 
void loop()
  {  if (BT1.available())
           Serial.write(BT1.read());
     if (Serial.available())
        BT1.write(Serial.read());
  }
