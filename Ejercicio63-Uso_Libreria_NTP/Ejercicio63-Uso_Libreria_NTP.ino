/*----------------------------------------------------------------------*
 * simple-NTP library example sketch.                                   *
 * Self-adjusting clock from NTP server                                 *
 * aprendiendoarduino by jecrespo May 2016                              *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/

#include <TimeLib.h>        //http://www.arduino.cc/playground/Code/Time
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <simpleNTP.h>

simpleNTP ntp;	//simpleNTP object
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x70, 0xEF};
byte ip[] = {192, 168, 1, 179};

void setup()
{
  Ethernet.begin(mac, ip);
  delay(500);
  Serial.begin(9600);
  Serial.println("Requesting Time...");
  unsigned long utc = ntp.readNTP();	//UTC time
  setTime(utc);
  Serial.println(utc);
}

void loop()
{
	Serial.println(millis());
	Serial.println(now());
  digitalClockDisplay();
	delay(1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(year());
  Serial.print("-");
  Serial.print(month());
  Serial.print("-");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

