/*
  Sketch para Arduino MKR1000
  Sonda de temperatura DS18B20
*/

#include <WiFi101.h>
#include <OneWire.h>
#include "Timer.h"

#define SONDAPIN 6
#define NUM_FRIGORIFICO 3 //sonda roja ROM = 28 C9 FF 26 0 0 80 2B
#define NUM_CONGELADOR 2  //sonda azul ROM = 28 D7 E7 26 0 0 80 21
#define TIEMPOGRABAR 600000 //en milisegundos
#define TIMEOUT 5000 //en milisegundos

// codigo de: https://github.com/jecrespo/aprendiendoarduino-servicios/blob/master/arduino_code/data_logger_temperatura_DS18B20_MKR1000/data_logger_temperatura_DS18B20_MKR1000.ino
//char url[] = "www.aprendiendoarduino.com";
char url[] = "www.bar.org";
//char ssid[] = "AndroidAP4628"; //  your network SSID (name)
//char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)

char ssid[] = "MOVISTAR_4CEO_EXT2";

char pass[] = "12345678901234567890123456";

int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;


OneWire  ds(SONDAPIN);  //(a 4.7K resistor is necessary)
WiFiClient client;
Timer t;

float temp_frigorifico;
float temp_congelador;
byte addr_frigorifico[8] = {0x28, 0xC9, 0xFF, 0x26, 0x0, 0x0, 0x80, 0x2B};
byte addr_congelador[8] = {0x28, 0xD7, 0xE7, 0x26, 0x0, 0x0, 0x80, 0x21};

void setup()
{

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, keyIndex, pass);

    // wait 10 seconds for connection:
    delay(1000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  t.every(TIEMPOGRABAR, grabaDatos);
}

void loop()
{
  if ( WiFi.status() != WL_CONNECTED) {
    while (WiFi.begin(ssid, keyIndex, pass) != WL_CONNECTED) {
      // unsuccessful, retry in 4 seconds
      Serial.println("failed, wifi no conectada ");
      delay(4000);
      Serial.println("retrying ... ");
    }
  }
  t.update();
}

void grabaDatos() {
  Serial.println();
  Serial.println("  ---------->>>>> leyendo temperatura... ");
  LeeSondas();

  if (temp_frigorifico == 999)
    Serial.println("Error lectura sonda " + String(NUM_FRIGORIFICO));
  else
    GrabaTemperatura(temp_frigorifico, NUM_FRIGORIFICO);

  if (temp_congelador == 999)
    Serial.println("Error lectura sonda " + String(NUM_CONGELADOR));
  else
    GrabaTemperatura(temp_congelador, NUM_CONGELADOR);
  Serial.println();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void GrabaTemperatura(float temperatura, int numero_sonda) {
  String webString = "";

  Serial.println("connecting to server...");

  if (client.connect(url, 80)) {
    Serial.println("connected. Sonda " + (String)numero_sonda);
    client.print("GET /temperatura_camaras/grabaDatos.php?arduino=" + (String)numero_sonda + "&dato=");
    client.print(temperatura);
    client.println(" HTTP/1.1");
    client.println("Host: www.bar.org");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed. Sonda " + (String)numero_sonda);
    return;
  }

  unsigned long timestamp = millis();
  while (client.available() == 0) {
    //espero respuesta del servidor
    if ((millis() - timestamp) > TIMEOUT) {
      Serial.println("conexion timeout. Sonda " + (String)numero_sonda);
      client.stop();
      return;
    }
  }

  if (client.available()) {
    Serial.println("Respuesta del Servidor---->");
    while (client.available()) {
      char c = client.read();
      webString += c;
    }
    Serial.println(webString);
    if (webString.indexOf("GRABADOS") > 0) Serial.println("Datos guardados correctamente. Sonda " + (String)numero_sonda);
    else Serial.println("Error al guardar los datos. Sonda " + (String)numero_sonda);

    client.stop();
  }
  client.stop();
}

void LeeSondas() {

  if (OneWire::crc8(addr_frigorifico, 7) != addr_frigorifico[7]) {
    Serial.println("CRC is not valid!");
    temp_frigorifico = 999;
  }
  else {
    byte present = 0;
    byte data[12];
    float celsius;

    ds.reset();
    ds.select(addr_frigorifico);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    delay(1000);     // maybe 750ms is enough, maybe not

    present = ds.reset();
    ds.select(addr_frigorifico);
    ds.write(0xBE);         // Read Scratchpad

    Serial.print("  Data = ");
    Serial.print(present, HEX);
    Serial.print(" ");
    for (int i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.print(" CRC=");
    Serial.print(OneWire::crc8(data, 8), HEX);
    Serial.println();

    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
    celsius = (float)raw / 16.0;
    temp_frigorifico = celsius;
    Serial.print("  Temperatura Frigorifico = ");
    Serial.print(celsius);
    Serial.println(" Celsius.");
  }

  if (OneWire::crc8(addr_congelador, 7) != addr_congelador[7]) {
    Serial.println("CRC is not valid!");
    temp_congelador = 999;
  }
  else {
    byte present = 0;
    byte data[12];
    float celsius;

    ds.reset();
    ds.select(addr_congelador);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    delay(1000);     // maybe 750ms is enough, maybe not

    present = ds.reset();
    ds.select(addr_congelador);
    ds.write(0xBE);         // Read Scratchpad

    Serial.print("  Data = ");
    Serial.print(present, HEX);
    Serial.print(" ");
    for (int i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.print(" CRC=");
    Serial.print(OneWire::crc8(data, 8), HEX);
    Serial.println();

    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
    celsius = (float)raw / 16.0;
    temp_congelador = celsius;
    Serial.print("  Temperatura Congelador = ");
    Serial.print(celsius);
    Serial.println(" Celsius.");
  }
}
