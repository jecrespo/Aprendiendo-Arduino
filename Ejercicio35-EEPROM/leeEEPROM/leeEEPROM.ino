#include <EEPROM.h>

int a = 0;
int value;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  value = EEPROM.read(a);

  Serial.print(a);
  Serial.print("\t");
  Serial.print(value,HEX);
  Serial.println();

  a = a + 1;

  if (a == 1024)
    while(true){
	}

  delay(50);
}
