#include <Ethernet.h>
#include <SPI.h>


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
byte ip[] = {
  192, 168, 1, 179
};
byte server[] = {
  64, 233, 187, 99
}; // Google
byte DNS[] = {
  8, 8, 8, 8
};
byte gateway[] = {
  192, 168, 1, 1
};
byte subnet[] = {
  255, 255, 255, 0
};
byte aemet[] = {
  172, 24, 34, 57
};
byte google[] = { 64, 233, 187, 99 };


EthernetClient client;
String webString = "";
char url[] = "aemet.es";
char api[] = "api.openweathermap.org";

void setup()
{
  String cadenaLeida = "";
  Ethernet.begin(mac, ip, DNS, gateway, subnet);
  Serial.begin(9600);

  Serial.println("inicializando red...");

  delay(1000);
  Serial.println("MENU");
  Serial.println("1 - google");
  Serial.println("2 - API openweathermap");
  Serial.println("3 - aemet");
  while (Serial.available() == 0) {
    //nada
  }
  do {
    char caracter_leido = Serial.read();
    cadenaLeida += caracter_leido;
    delay(5);
  }  while (Serial.available() > 0);
  Serial.print("He Leido la cadena: ");
  Serial.println(cadenaLeida);

  int respuesta = cadenaLeida.toInt();

  switch (respuesta) {
    case 1:
      if (client.connect(google, 80)) {
        Serial.println("connected");
        client.println("GET / HTTP/1.0");
        client.println();
      }
      else {
        Serial.println("connection failed");
      }

      break;
    case 2:
      if (client.connect(api, 80)) {
        Serial.println("connected");
        client.println("GET /data/2.5/weather?q=London&mode=xml HTTP/1.1");
        client.println();
      }
      else {
        Serial.println("connection failed");
      }
      break;
    case 3:
      if (client.connect(url, 80)) {
        Serial.println("connected");
        client.println("GET /xml/municipios/localidad_26089.xml HTTP/1.1");
        client.println();
      }
      else {
        Serial.println("connection failed");
      }
      break;
    default:
      Serial.println("Opcion incorrecta");
      break;
  }

}

void loop()
{
  if (client.available()) {
    char c = client.read();
    webString += c;
    Serial.print(c);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    //Serial.println(webString);
    for (;;)
      ;
  }
}

