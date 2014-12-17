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
String GET = "GET / HTTP/1.1";
String POST = "POST /enciendeled HTTP/1.1";

void setup()
{
  Serial.begin(9600);
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);

  // start listening for clients
  server.begin();
}

void loop()
{
  int led = digitalRead(8);
  recibido = ""; //Vacio el Buffer
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client) {
    while (server.available()) {
      recibido += char(client.read());
      //Serial.println(client.read());
    }
    Serial.println("He recibido por ethernet: ");
    Serial.println(recibido);

    if (recibido.startsWith("GET")){
      // Despues de leer la peticion, devuelvo la web embebida
      // read bytes from the incoming client and write them back
      // to any clients connected to the server:
      client.println("HTTP/1.0 200K");
      client.println();
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<body>");
      if (led == HIGH) client.println("<p>Apaga un Led de Arduino</p>");
      else client.println("<p>Enciende un Led de Arduino</p>");
      client.println("<form action=\"\" method=\"post\">");
      if (led == HIGH) client.println("<input type=\"submit\" value=\"Apaga Led\" />");
      else client.println("<input type=\"submit\" value=\"Enciende Led\" />");
      client.println("</form>");
      client.println("</body>");
      client.println("</html>");
      client.println();
      client.stop();
      client.flush();
    }
    else if (recibido.startsWith("POST")){
      digitalWrite(8,!led);
      client.println("HTTP/1.0 200K");
      client.println();
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<body>");
      if (led == HIGH) client.println("<p>LED APAGADO!</p>");
      else client.println("<p>LED ENCENDIDO!</p>");
      client.println("<a href=\"\">Volver</a>");
      client.println("</body>");
      client.println("</html>");
      client.println();
      client.stop();
      client.flush();
    }
    else{
      client.println("HTTP/1.0 200K");
      client.println();
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<body>");
      client.println("<p>ACCESO DENEGADO!</p>");
      client.println("</body>");
      client.println("</html>");
      client.println();
      client.stop();
      client.flush();;
    }
  }
}






