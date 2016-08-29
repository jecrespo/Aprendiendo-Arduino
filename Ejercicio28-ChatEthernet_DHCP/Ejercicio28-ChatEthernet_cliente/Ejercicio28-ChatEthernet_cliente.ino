#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY
};	//Sustituir YY por el numero de MAC correcto
byte server[] = {
  192, 168, x, x
};	//Poner IP asignada al server

EthernetClient client;

void setup() {
  Serial.begin(9600);

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

  Serial.println("connecting...");
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("Estoy conectado");
  } else {
    Serial.println("connection failed");
  }
}
void loop() {
  if (client.available()) {
    Serial.print((char)client.read());
  }
  if (Serial.available()) {
    client.print((char)Serial.read());
  }
}



