#include <Wire.h>
#include <ArduinoWiFi.h>
/*
on your borwser, you type http://<IP>/arduino/webserver/ or http://<hostname>.local/arduino/webserver/

http://labs.arduino.org/WebServer

modified by www.aprendiendoarduino.com

At least http://<IP>/arduino/web must be typed in browser for ESP send request to 328P and process
*/

boolean led_state = LOW;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Inicializando..."));
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  led_state = LOW;

  Wifi.begin();
  Wifi.println(F("WebServer Server is up"));
}
void loop() {

  while (Wifi.available()) {
    process(Wifi);
  }
  delay(50);
}
void process(WifiData client) {
  // read the command
  String command = "";
  
  while (Wifi.available()) {
    command = command + (char)client.read();  //read complete command to analyze
  }
  
  Serial.println(command);

  if (command == "/arduino/webserver/on") {
    digitalWrite(13, HIGH);
    led_state = HIGH;
  }
  if (command == "/arduino/webserver/off") {
    digitalWrite(13, LOW);
    led_state = LOW;
  }
  
  WebServer(client);  //Always send web

}
void WebServer(WifiData client) {
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close"));
  client.println();
  client.println(F("<!DOCTYPE html>"));
  client.println(F("<html>"));
  client.println(F("<head><style>"));
  if (led_state == HIGH) client.println(F("div {background-color: yellow; border: 1px solid black; padding: 5px; margin: 5px; width: 50px; border-radius: 20px; text-align: center; font-weight:bold;}"));
  else client.println(F("div {background-color: grey; border: 1px solid black; padding: 5px; margin: 5px; width: 50px; border-radius: 20px; text-align: center; font-weight:bold;}"));
  client.println(F("</style></head>"));
  client.println(F("<body>"));
  client.println(F("<div>"));
  if (led_state == HIGH) client.println(F("LED ON"));
  else client.println(F("LED OFF"));
  client.println(F("</div>"));
  if (led_state == HIGH) client.println(F("<form action=\"/arduino/webserver/off\" method=\"get\">"));
  else client.println(F("<form action=\"/arduino/webserver/on\" method=\"get\">"));
  if (led_state == HIGH) client.println(F("<input type=\"submit\" value=\"Switch OFF\" />"));
  else client.println(F("<input type=\"submit\" value=\"Switch ON\" />"));
  client.println(F("</form>"));
  client.println(F("</body>"));
  client.println(F("</html>"));
  client.print(DELIMITER); // very important to end the communication !!!
}
