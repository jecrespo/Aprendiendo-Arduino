#include <Ethernet.h>
#include <SPI.h>
#include "Timer.h"

#define NUM_ARDUINO 5

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x70, 0xYY};	//Sustituir YY por el numero de MAC correcto

byte ip[] = {
  10, 22, 72, 31
};
byte DNS[] = {
  8, 8, 8, 8
};
byte gateway[] = {
  10, 22, 72, 1
};
byte subnet[] = {
  255, 255, 255, 0
};
char url[] = "api.thingspeak.com";

EthernetClient client;
Timer t;
String webString = "";

void setup()
{
  Serial.begin(9600);
  Serial.println("inicializando red...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for (;;)
      ;
  }
  else {
    Serial.print("IP asignada por DHCP: ");
    Serial.println(Ethernet.localIP());
  }
    
  Serial.println("Iniciando datalogger...");
  delay(1000);
  t.every(5000, grabaDatos);
}

void loop()
{
  webString = "";
  t.update();
  if (client.available()) {
    Serial.println("Respuesta del Servidor---->");
    while (client.available()) {
      char c = client.read();
      webString += c;
    }
    Serial.println(webString);
    client.stop();
  }
}

void grabaDatos() {
  Serial.println("leyendo temperatura... ");
  int sensorVal = analogRead(A0);
  float voltage = (sensorVal / 1024.0) * 5.0;
  float temperature = (voltage - 0.5) * 100; //Sonda temperatura kit Arduino
  Serial.print("Temperatura Leida: ");
  Serial.println(temperature);

  Serial.println("connecting to server...");
  if (client.connect(url, 80)) {
    Serial.println("connected");
    client.print("POST /update.json?api_key=DRRINT6CZN93DPJW&amp;field1=");
    client.print(temperature);
    client.println(" HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
}
