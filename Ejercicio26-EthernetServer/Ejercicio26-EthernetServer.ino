#include <SPI.h>
#include <Ethernet.h>

// network configuration.  gateway and subnet are optional.

 // the media access control (ethernet hardware) address for the shield:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
//the IP address for the shield:
byte ip[] = { 192, 168, 1, 179 };    
// the router's gateway address:
byte gateway[] = { 192, 168, 1, 1 };
// the subnet:
byte subnet[] = { 255, 255, 255, 0 };

// telnet defaults to port 23
EthernetServer server = EthernetServer(80);

void setup()
{
  Serial.begin(9600);
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);

  // start listening for clients
  server.begin();
}

void loop()
{
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client > 0) {
    client.println("HTTP/1.0 200K");
    client.println();
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    while (client.available() > 0){
    char inChar = client.read();
    client.write(inChar);
    Serial.print(inChar);
    //if (inChar == '\n') {
      //client.stop();
    //}
    }
    client.print("He leido por el puerto analogico: ");
    client.println(analogRead(A0));
    client.println();
    client.stop();
  }
}
