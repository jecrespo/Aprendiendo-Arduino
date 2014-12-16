/*Tutorial Arduino -I2C
Autor:Toni Ruiz Sastre 
www.electroensaimada.com
*/


#include <Wire.h>

void setup()
{
  Wire.begin(0xF5);                // Nos ponemos la direccion 0xF5
  Wire.onReceive(receiveEvent); // Configuramos la recepcion como una interrupcion/evento
  Serial.begin(9600);           // Configuramos el serial
}

void loop()
{
  delay(100);
}


//Interrupcion de dato recibido i2c
void receiveEvent(int howMany)
{
    while( Wire.available()>0) // Leemos todo el buffer
  {
    byte rec = Wire.read(); // almacenamos el byte
    Serial.print(rec,HEX);         // Lo mostramos por pantalla
  }
  Serial.println();
    
}
