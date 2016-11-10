/*
  Web impresa:
  <!DOCTYPE html>
  <html>
  <body>
  <p>LED APAGADO</p>
  <form action="" method="post">
  <input type="submit" value="Enciende Led" />
  </form>
  </body>
  </html>
*/

#include <SPI.h>
#include <Ethernet.h>

//Configuración Ethernet
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAA
};
//the IP address for the shield:
byte ip[] = {
  10, 22, 72, 30
};
// the router's gateway address:
byte gateway[] = {
  10, 22, 72, 1
};
// the subnet:
byte subnet[] = {
  255, 255, 255, 192
};

EthernetServer server = EthernetServer(80);

boolean led_state = LOW;

void setup()
{
  Serial.begin(9600);
  //Inicializar Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  led_state = LOW;

  //Comienzo a escuchar cliente ethernet
  server.begin();
}

void loop()
{
  String recibido = ""; //Buffer

  //Si hay in cliente entrante, hay caracteres por leer
  EthernetClient client = server.available();
  if (client) {
    while (client.available()) {
      recibido += char(client.read());
      //Serial.println(client.read());  //Para hacer debug
    }
    Serial.println("He recibido por ethernet: ");
    Serial.println(recibido);

    if (recibido.startsWith("GET / HTTP/1.1")) {
      Serial.println("------> Muestro web");
      muestraWeb(client);
    }
    else if (recibido.startsWith("POST / HTTP/1.1")) {
      Serial.println("------> Cambio estado LED");
      led_state = !led_state;
      digitalWrite(8, led_state);  //cambio el estado del led
      muestraWeb(client);
    }
    else {
      client.println("HTTP/1.0 200K");
      client.println();
      client.println();
      client.stop();
      client.flush();
    }
  }
}

void muestraWeb(EthernetClient client) {
  client.println("HTTP/1.0 200K");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<body>");
  if (led_state == HIGH) client.println("<p>LED ENCENDIDO</p>");
  else client.println("<p>LED APAGADO</p>");
  client.println("<form action=\"\" method=\"post\">");
  if (led_state == HIGH) client.println("<input type=\"submit\" value=\"Apaga Led\" />");
  else client.println("<input type=\"submit\" value=\"Enciende Led\" />");
  client.println("</form>");
  client.println("</body>");
  client.println("</html>");
  client.println();
  client.stop();
  client.flush();
}
