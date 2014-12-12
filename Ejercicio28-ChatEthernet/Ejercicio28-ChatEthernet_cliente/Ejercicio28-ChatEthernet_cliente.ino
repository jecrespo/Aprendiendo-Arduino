#include <Ethernet.h>
#include <SPI.h>
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
byte server[] = {
  192, 168, 1, 180 };

EthernetClient client;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip, DNS, gateway, subnet);
  delay(1000);
  Serial.println("connecting...");
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("Estoy conectado");
  } else {
    Serial.println("connection failed");
  }
}
void loop(){
  if (client.available()){
    Serial.print((char)client.read());
  }
  if (Serial.available()){
    client.print((char)Serial.read());
  }
}



