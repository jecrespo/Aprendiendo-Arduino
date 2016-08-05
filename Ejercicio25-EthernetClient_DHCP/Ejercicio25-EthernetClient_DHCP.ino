#include <Ethernet.h>
#include <SPI.h>


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY  //Sustituir YY por el numero de MAC correcto
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
  Serial.begin(9600);
  String cadenaLeida = "";
  // start the Ethernet connection:
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

