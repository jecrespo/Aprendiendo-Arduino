
/*Tutorial Arduino -I2C
Autor:Toni Ruiz Sastre 
www.electroensaimada.com
*/

#include <Wire.h>

void setup()
{
  Wire.begin();        //Iniciamos el bus I2C 
  Serial.begin(9600);  //Iniciamos el Serial
}

void loop()
{
   unsigned char result;
   
   
   Wire.beginTransmission(0xF5); // Enviamos a la dirección 0xF5 en binario 111 0101
  Wire.write(5);         //Enviamos un 5           
  Wire.write(7);          //Enviamos un 7
  Wire.endTransmission(); //Terminamos la trama    
delayMicroseconds(100);//Para poder distinguir en el osciloscopio el envio y la recepcion
  Wire.requestFrom(0xF5, 1);    // Pedimos 1 bytes al slave 0xf5

  while(Wire.available())    // Mientras tengamos datos.
  { 
    result = Wire.read(); // Leemos el resultado
         
  }
Serial.print("5 + 7 =");Serial.println(result);
  delay(500);
}
