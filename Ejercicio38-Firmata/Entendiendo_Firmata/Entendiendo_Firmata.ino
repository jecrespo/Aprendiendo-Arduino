//Example based on https://www.arduino.cc/en/Reference/Firmata example
//To decode the firmata protocol use Bray's terminal from https://sites.google.com/site/terminalbpp/
//More details in https://aprendiendoarduino.wordpress.com/

#include <Firmata.h>

byte analogPin;

void analogWriteCallback(byte pin, int value)
{
  pinMode(pin, OUTPUT);
  analogWrite(pin, value);
}

void setup()
{
  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  Firmata.begin();
  Serial.begin(57600);
}

void loop()
{
  Serial.write(255); // send a byte with the value 255 en hexadecimal FF
  Serial.write(255); // FFFF means the loop init
  while (Firmata.available()) {
    Firmata.processInput();
  }
  for (analogPin = 0; analogPin < TOTAL_ANALOG_PINS; analogPin++) {
    Firmata.sendAnalog(analogPin, analogRead(analogPin));
    Serial.println(analogRead(analogPin));
    delay(1000);
  }
  delay(5000);
}
