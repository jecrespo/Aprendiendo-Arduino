#include <Ethernet.h>
#include <SPI.h>
#include "Timer.h"


byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 
  192, 168, 1, 179 };
byte DNS[] = {
  8,8,8,8};
byte gateway[] = {
  192, 168, 1, 1};
byte subnet[] = {
  255,255,255,0};
byte raspberry[] = {
  192,168,1,169}; 

EthernetClient client;
Timer t;
String webString = "";

void setup()
{
  Ethernet.begin(mac, ip, DNS, gateway, subnet);
  Serial.begin(9600);
  delay(1000);
  t.every(5000,grabaDatos);
}

void loop()
{
  webString = "";
  t.update();
  if (client.available()) {
    Serial.println("Respuesta del Servidor---->");
    while (client.available()){
      char c = client.read();
      webString += c;
    }
    Serial.println(webString);
    if (webString.endsWith("GRABADOS") == true) Serial.println("Datos guardados correctamente");
    else Serial.println("Error al guardar los datos");
    
   client.stop();
  }
}

void grabaDatos(){
  Serial.println("leyendo temperatura... ");
  int sensorVal = analogRead(A0);
  float voltage = (sensorVal/1024.0)*5.0;
  float temperature = (voltage - 0.5)*100;
  Serial.print("Leido: ");
  Serial.print(temperature);
  Serial.println(" grados");
  
  Serial.println("connecting to server...");
  if (client.connect(raspberry, 80)) {
    Serial.println("connected");
    client.print("GET /grabaDatos.php?arduino=99&temperatura=");
    client.print(temperature);
    client.println(" HTTP/1.1");
    client.println("Host: arduino");
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.println("connection failed");
  }
}


