/*
  Web impresa:
  <!DOCTYPE html>
  <html>
  <head>
  <title>Barra Desplazamiento</title>
  <script type="text/javascript">
  function sendB() {
    valorB = document.getElementById("B").value;
    url = "http://192.168.240.1/arduino/B/"+valorB;
    var xhttp = new XMLHttpRequest();
	xhttp.open("GET", url, true);
	xhttp.send();
  }
  </script>
  </head>
  <body>
  <p>Barra Desplazamiento</p>
  <input id = "B" type="range" name="B" min="0" max="255" step="1" value="0" onchange="sendB()">
  </body>
  </html>
*/

#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

//Configuración Ethernet
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY  //Sustituir YY por el numero de MAC correcto
};

EthernetServer server = EthernetServer(80);
Servo myservo;

void setup()
{
  Serial.begin(250000);
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

  pinMode(6, OUTPUT);
  analogWrite(6, 0);
  myservo.attach(9);

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
    else if (recibido.startsWith("GET /B/")) {
      Serial.print("------> Cambio a: ");
      String valor = recibido.substring(7, recibido.indexOf(" HTTP/1.1"));
      Serial.println(valor);
      analogWrite(6, valor.toInt());
      myservo.write(valor.toInt());
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
  client.print("<head><title>Barra Desplazamiento</title><script type=\"text/javascript\">");
  client.print("function sendB() {valorB = document.getElementById(\"B\").value;url = \"http://");
  client.print(Ethernet.localIP());
  client.print("/B/\"+valorB;var xhttp = new XMLHttpRequest();xhttp.open(\"GET\", url, true);xhttp.send();}");
  client.print("</script></head>");
  client.println("<body>");
  client.println("<p>Barra Desplazamiento</p>");
  client.println("<input id = \"B\" type=\"range\" name=\"B\" min=\"0\" max=\"255\" step=\"1\" value=\"0\" onchange=\"sendB()\">");
  client.println("</body>");
  client.println("</html>");
  client.println();
  client.stop();
}
