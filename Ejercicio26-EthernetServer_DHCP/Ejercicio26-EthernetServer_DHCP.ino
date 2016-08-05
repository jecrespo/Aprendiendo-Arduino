#include <SPI.h>
#include <Ethernet.h>

// network configuration.  gateway and subnet are optional.

// the media access control (ethernet hardware) address for the shield:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY };  //Sustituir YY por el numero de MAC correcto

// WEB defaults to port 80
EthernetServer server = EthernetServer(80);

void setup()
{
  Serial.begin(9600);

  // initialize the ethernet device
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

  // start listening for clients
  server.begin();
}

void loop()
{
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client > 0) {
    server.println("HTTP/1.0 200K");
    server.println();
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    while (server.available() > 0) {
      char inChar = client.read();
      client.write(inChar);
      Serial.print(inChar);
      //if (inChar == '\n') {
      //client.stop();
      //}
    }
    server.print("He leido por el puerto analogico: ");
    server.println(analogRead(A0));
    server.println();
    client.stop();
  }
}
