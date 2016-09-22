#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY }; //Sustituir YY por el numero de MAC correcto
byte ip[] = {192, 168, 1, 10}; //the IP
byte gateway[] = {192, 168, 1, 1}; // the router's gateway address
byte subnet[] = {255, 255, 255, 0}; // the subnet

EthernetClient client;

void setup() {
  // start the serial library
  Serial.begin(9600);
  // start the Ethernet connection
  Ethernet.begin(mac, ip, gateway, subnet);

  // print your local IP address:
  Serial.println(Ethernet.localIP());
}

void loop() {
  Serial.println("Now, you can connect to Internet");
  while (true) {
    //nada
  }
}

