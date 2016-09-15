#include <avr/wdt.h>

int loops = 0;

void setup()
{
  Serial.begin(9600);
  wdt_disable();
  wdt_enable(WDTO_8S);
  Serial.println("iniciando");
  delay(1000);
}

void loop()
{
  Serial.println("hola " + String(loops));
  delay(1000);
  if (loops > 20) {
    Serial.println("entro en bucle largo");
    for (int i = 0; i < 100; i++) {
      Serial.println(i);
      delay(1000);
    }
  }
  wdt_reset();
  loops++;
}

