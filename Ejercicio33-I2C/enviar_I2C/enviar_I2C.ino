/*Tutorial Arduino -I2C
Autor:Toni Ruiz Sastre 
www.electroensaimada.com
*/
#include <Wire.h>

void setup()
{
  Wire.begin();//Iniciamos el bus I2C 
}

void loop()
{
  Wire.beginTransmission(0xF5); // Enviamos a la dirección 0xF5 en binario 111 0101
  Wire.write(0x52);          // Enviamos el byte 0x52            
  Wire.endTransmission();    // stop 

  delay(100);
}
