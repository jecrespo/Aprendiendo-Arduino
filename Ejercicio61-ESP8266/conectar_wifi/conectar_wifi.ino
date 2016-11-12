#include <SoftwareSerial.h>
SoftwareSerial BT1(3, 2); // RX | TX

void setup()
{ Serial.begin(9600);
  BT1.begin(9600);
  //Serial.println("Version");
  //BT1.println("AT+GMR");
  //respuesta();
  //delay(500);
  //Serial.println("Factory reset");
  //BT1.println("AT+RESTORE");
  //respuesta();
  //delay(500);
  //Serial.println("Velocidad 9600");
  //BT1.println("AT+UART_DEF=9600,8,1,0,0");
  //respuesta();
  //delay(500);
  //BT1.begin(9600);
  Serial.println("modo cliente");
  BT1.println("AT+CWMODE_DEF=1");
  respuesta();
  delay(500);
  Serial.println("conectar wifi");
  BT1.println("AT+CWJAP=\"AndroidAP4628\",\"esp8266wifi\"");
  respuesta();
  delay(500);
  Serial.println("comprobar la IP asignada");
  BT1.println("AT+CIFSR");
  respuesta();
  delay(500);
}

void loop()
{

}

void respuesta() {
  String respuesta = "";
  boolean ok = 1;
  while (BT1.available() == 0) {
    //nada
  }
  do {
    if (BT1.available() > 0) {
      char caracter_leido = BT1.read();
      respuesta = respuesta + caracter_leido;
      Serial.print(caracter_leido);
      if (respuesta.endsWith("OK")) ok = 0;
    }
  }  while (ok || (BT1.available() == 0));
  Serial.println();
}

