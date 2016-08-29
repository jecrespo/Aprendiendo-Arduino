#include <Ethernet.h>
#include <SPI.h>
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };	//Sustituir YY por el numero de MAC correcto

EthernetServer server = EthernetServer(80);

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
  
  server.begin();
  delay(1000);
}

void loop(){
  EthernetClient client = server.available();
  if (client.available()){
    Serial.print((char)client.read());
  }
  if (Serial.available()){
    client.print((char)Serial.read());
  }
}



