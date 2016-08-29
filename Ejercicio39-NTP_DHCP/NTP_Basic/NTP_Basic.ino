//Ardunino NTP Basic Client
//Based on https://github.com/PaulStoffregen/Time TimeNTP example
//BAsed on https://www.arduino.cc/en/Tutorial/UdpNtpClient
//NTP Protocol https://en.wikipedia.org/wiki/Network_Time_Protocol

//----------------------- LIBRARIES ------------------------//
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Time.h>   // Version 1.4. https://github.com/PaulStoffregen/Time. More info: https://www.pjrc.com/teensy/td_libs_Time.html

//----------------------- NETWORK CONFIGURATION ------------------------//
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x70, 0xYY};	//Sustituir YY por el numero de MAC correcto
char serverName[] = "0.es.pool.ntp.org";

EthernetUDP Udp;
unsigned int localPort = 8888;      // local port to listen for UDP packets

//----------------------- NTP VARS ------------------------//
const int NTP_PACKET_SIZE = 48;  //NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
const long timeZoneOffset = 3600;   //GMT+1 TimeZone

const String weekdays[] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup() {
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
  
  Udp.begin(localPort);
  setSyncProvider(getTimeAndDate);    // set the external time provider
  setSyncInterval(60);          // set the number of seconds between re-sync
}

void loop() {
  now();  //Reads the current time, as a time_t number. Needed for setSyncInterval() function
}

//--------------------- FUNCTIONS --------------------------//
void sendNTPpacket(char * serverName) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp
  Udp.beginPacket(serverName, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

unsigned long getTimeAndDate() {
  sendNTPpacket(serverName);
  delay(1000);
  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    Serial.println("Response from NTP Server. seconds from 1/1/1990 (32 bits)");
    Serial.print("BIN --> ");
    Serial.println(highWord << 16 | lowWord, BIN);
    Serial.print("DEC --> ");
    Serial.println(highWord << 16 | lowWord);
    unsigned long epoch = highWord << 16 | lowWord;  //seconds from 1/1/1990 (32 bits) = packetBuffer[40]+packetBuffer[41]+packetBuffer[42]+packetBuffer[43]
    epoch = epoch - 2208988800UL + timeZoneOffset;  //Unix time + timezone
    setTime(epoch);
    Serial.print("Unix timestamp: ");
    Serial.println(epoch);
    Serial.print("Updated timestamp: ");
    Serial.print(year());
    Serial.print("/");
    if (month() < 10) Serial.print("0");
    Serial.print(month());
    Serial.print("/");
    if (day() < 10) Serial.print("0");
    Serial.print(day());
    Serial.print(" ");
    if (hour() < 10) Serial.print("0");
    Serial.print(hour());
    Serial.print(":");
    if (minute() < 10) Serial.print("0");
    Serial.print(minute());
    Serial.print(":");
    if (second() < 10) Serial.print("0");
    Serial.println(second());
    Serial.print("Week Day: ");
    Serial.println(weekdays[weekday()]);
    return epoch;
  }
  else {
    Serial.println("NTP Server not responding!");
    return 0;
  }
}
