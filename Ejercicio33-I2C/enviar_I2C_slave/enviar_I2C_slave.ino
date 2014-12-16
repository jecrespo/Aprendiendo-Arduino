/*Tutorial Arduino -I2C
Autor:Toni Ruiz Sastre 
www.electroensaimada.com
*/


#include <Wire.h>

unsigned char result=0;

void setup()
{
  Wire.begin(0xF5);                // Address 0xf5
  Wire.onRequest(requestEvent); // Activamos evento de peticion
    Wire.onReceive(receiveEvent);//Activamos evento de lectura.
}

void loop()
{
  delay(100);
}

//Evento de peticion se activa cuando un master nos pide que le enviemos algun dato
void requestEvent()
{
  Wire.write(result); // Enviamos el resultado del calculo
                      
}

//Evento de recpcion
void receiveEvent(int howMany)
{
  unsigned char sum1,sum2;
  while( Wire.available()) //Leemos hasta que no haya datos. Teoricamente son 2.
  {
     sum1 = Wire.read(); //Leemos los dos bytes
     sum2 = Wire.read();
     result=sum1+sum2;//Sumamos
  }

}
