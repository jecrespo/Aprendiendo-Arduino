/* this library is writing by  Cristian Steib.
        steibkhriz@gmail.com
    Designed to work with the GSM Sim800l,maybe work with SIM900L

       This library use SoftwareSerial, you can define de RX and TX pin
       in the header "Sim800l.h" ,by default the pin is RX=10 TX=11..
       be sure that gnd is attached to arduino too.
       You can also change the other preferred RESET_PIN

       Esta libreria usa SoftwareSerial, se pueden cambiar los pines de RX y TX
       en el archivo header, "Sim800l.h", por defecto los pines vienen configurado en
       RX=10 TX=11.
       Tambien se puede cambiar el RESET_PIN por otro que prefiera

      PINOUT:
          _____________________________
         |  ARDUINO UNO >>>   SIM800L  |
          -----------------------------
              GND      >>>   GND
          RX  10       >>>   TX
          TX  11       >>>   RX
         RESET 2       >>>   RST

     POWER SOURCE 4.2V >>> VCC

      Created on: April 20, 2016
          Author: Cristian Steib


*/

#include <Sim800L.h>
#include <SoftwareSerial.h> //is necesary for the library!! 
Sim800L Sim800l;  //to declare the library
String telefono;
String  Mensaje;

bool error; //to catch the response of sendSms


void setup() {
  Serial.begin(9600);
  Serial.println("Formato telefono+-+texto del mensaje+!");
  Sim800l.begin(); // initializate the library.
}

void loop() {

  if (Serial.available())
  {
    String data = Serial.readStringUntil('\n');
    int guion = data.indexOf("-");
    int admiracion = data.indexOf("!");
    telefono = data.substring(0, guion);
    Mensaje = data.substring((guion + 1), admiracion);
    Serial.println("Telefono leido: " + telefono);
    Serial.println("Mensaje leido: " + Mensaje);
    char number[telefono.length()+1];
    char text[Mensaje.length()+1];

    telefono.toCharArray(number, telefono.length()+1);
    Mensaje.toCharArray(text, Mensaje.length()+1);
    Serial.print("Telefono leido convertido: " );
    Serial.println(number);
    Serial.print("Mensaje leido convertido: " );
    Serial.println(text);

    error = Sim800l.sendSms(number, text);
    Serial.println("Respuesta del envio de SMS: " + (String(error)));

  }
}
