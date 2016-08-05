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

//Versión diseñada para Arduino WiFi Shield https://www.arduino.cc/en/Main/ArduinoWiFiShield
//Se usa la WiFi Library https://www.arduino.cc/en/Reference/WiFi

#include <WiFi.h>

//Configuración WiFi
char ssid[] = "yourNetwork";    //SSID of your network
char pass[] = "secretPassword"; //password of your WPA Network
int status = WL_IDLE_STATUS;    // the Wifi radio's status
IPAddress ip;                   // the IP address of your shield

WiFiServer server(80);

boolean led_state = LOW;

void setup()
{
  Serial.begin(9600);
  //Inicializar Wifi
  Serial.println("inicializando WiFi...");
  WiFi.begin(ssid);

  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }
  // if you are connected, print out info about the connection:
  else {
    //print the local IP address
    ip = WiFi.localIP();
    Serial.print("IP asignada: ");
    Serial.println(ip);
  }

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  led_state = LOW;

  //Comienzo a escuchar cliente Wifi
  server.begin();
}

void loop()
{
  String recibido = ""; //Buffer

  //Si hay in cliente entrante, hay caracteres por leer
  WiFiClient client = server.available();
  if (client) {
    while (server.available()) {
      recibido += char(client.read());
      //Serial.println(client.read());  //Para hacer debug
    }
    Serial.println("He recibido por WiFi: ");
    Serial.println(recibido);

    if (recibido.startsWith("GET / HTTP/1.1")) {
      Serial.println("------> Muestro web");
      muestraWeb(client);
    }
    else if (recibido.startsWith("POST / HTTP/1.1")) {
      Serial.println("------> Cambio estado LED");
      led_state = !led_state;
      digitalWrite(13, led_state);  //cambio el estado del led
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

void muestraWeb(WiFiClient client) {
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
