/*
Web impresa:
<!DOCTYPE html>
<html>
<body>
  <p>Enciende un Led de Arduino</p>
  <form action="http://192.168.1.179/enciendeled" method="get">
  <input type="submit" value="Enciende Led" />
  </form>
</body>
</html>
*/

#include <SPI.h>
#include <Ethernet.h>

// network configuration.  gateway and subnet are optional.

// the media access control (ethernet hardware) address for the shield:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
//the IP address for the shield:
byte ip[] = { 
  192, 168, 1, 179 };    
// the router's gateway address:
byte gateway[] = { 
  192, 168, 1, 1 };
// the subnet:
byte subnet[] = { 
  255, 255, 255, 0 };

// telnet defaults to port 23
EthernetServer server = EthernetServer(80);
String recibido = "";

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
  if (client) {
    while (server.available()) {
      recibido += char(client.read());
      //Serial.println(client.read());
    }
    Serial.println("He recibido por ethernet: ");
    Serial.println(recibido);
    

    // Despues de ller la petici�n, devuelvo la web embebida
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<body>");
    client.println("<p>Enciende un Led de Arduino</p>");
    client.println("<form action=\"http://192.168.1.179/enciendeled\" method=\"post\">");
    client.println("<input type=\"submit\" value=\"Enciende Led\" />");
    client.println("</form>");
    client.println("</body>");
    client.println("</html>");
    
    client.stop();
    client.flush();
    
    recibido = ""; //Vacio el Buffer
    
    
  }

}





