#include <Ethernet.h>
#include <SPI.h>
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 
  192, 168, 1, 180 };
byte DNS[] = {
  8,8,8,8};
byte gateway[] = {
  192, 168, 1, 1};
byte subnet[] = {
  255,255,255,0};

EthernetServer server = EthernetServer(80);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip, DNS, gateway, subnet);
  server.begin();
  delay(1000);
}

void loop(){
  EthernetClient client = server.available();
  if (client.available()){
    Serial.print((char)client.read());
  }
  if (Serial.available()){
    client.print((char)Serial.read());
  }
}



