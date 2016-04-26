#include <SoftwareSerial.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

SoftwareSerial mySerial(9, 8);

void setup() {

  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("Ejemplo Reloj Display");
  Serial.println("-------------------");

  mySerial.begin(9600);
  delay(500);
}

void loop() {
  tmElements_t tm;
  
  mySerial.write(0xFE);   //command flag
  mySerial.write(0x01);   //clear command.

  if (RTC.read(tm)) {
    mySerial.write(0xFE);
    mySerial.write(128);  //First Line
    mySerial.print((int)tm.Day);
    mySerial.print("/");
    mySerial.print((int)tm.Month);
    mySerial.print("/");
    mySerial.print((int)tmYearToCalendar(tm.Year));

    mySerial.write(0xFE);
    mySerial.write(192);  //Second Line
    print2digits(tm.Hour);
    mySerial.print(":");
    print2digits(tm.Minute);
    mySerial.print(":");
    print2digits(tm.Second);

    Serial.print("Time = ");
    Serial.print(tm.Hour);
    Serial.write(':');
    Serial.print(tm.Minute);
    Serial.write(':');
    Serial.print(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();

    Serial.println(millis());
    Serial.println(now());
    Serial.println(RTC.get());
    Serial.println(makeTime(tm));
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    mySerial.print('0');
  }
  mySerial.print(number);
}
