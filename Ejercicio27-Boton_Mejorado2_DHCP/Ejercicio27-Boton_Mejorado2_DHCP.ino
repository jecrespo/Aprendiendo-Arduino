/*
  Web impresa:
  <!DOCTYPE html>
  <html>
  <body>
  <p>LED APAGADO</p>
  <form action="" method="post">
  <input type="hidden" name="led" value="on">
  <input type="submit" value="Enciende Led" />
  </form>
  </body>
  </html>
*/

#include <SPI.h>
#include <Ethernet.h>

//Configuración Ethernet
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY  //Sustituir YY por el numero de MAC correcto
};

EthernetServer server = EthernetServer(80);

boolean led_state = LOW;

void setup()
{
  Serial.begin(9600);
  //Inicializar Ethernet
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
    while (server.available()) {
      recibido += char(client.read());
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
      client.stop();
    }
  }
}

void muestraWeb(EthernetClient client) {
  client.println("HTTP/1.0 200K");
  client.println("Content-Type:text/html");
  client.println("Server:Arduino");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<body>");
  if (led_state == HIGH) client.println("<p>LED ENCENDIDO</p>");
  else client.println("<p>LED APAGADO</p>");
  client.println("<form action=\"\" method=\"post\">");
  if (led_state == HIGH)client.println("<input type=\"hidden\" name=\"led\" value=\"off\">");
  else client.println("<input type=\"hidden\" name=\"led\" value=\"on\">");
  if (led_state == HIGH) client.println("<input type=\"submit\" value=\"Apaga Led\" />");
  else client.println("<input type=\"submit\" value=\"Enciende Led\" />");
  client.println("</form>");
  client.println("</body>");
  client.println("</html>");
  client.println();
  client.stop();
}
