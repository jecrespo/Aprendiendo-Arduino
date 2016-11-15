#include "Wire.h"
extern "C" {
    #include "utility/twi.h"
}
void scanI2CBus(byte from_addr, byte to_addr, void(*callback)(byte address, byte result) )
{
  byte rc;
  byte data = 0;
  for( byte addr = from_addr; addr <= to_addr; addr++ ) {
    rc = twi_writeTo(addr, &data, 0, 1, 0);
    callback( addr, rc );
  }
}
void scanFunc( byte addr, byte result ) {
  Serial.print("addr: ");
  Serial.print(addr,DEC);
  Serial.print( (result==0) ? " Encontrado!":"     ");
  Serial.print( (addr%4) ? "\t":"\n");
}


const byte start_address = 8;
const byte end_address = 119;
 
void setup()
{
    Wire.begin();
    Serial.begin(9600);
    Serial.print("Escaneando bus I2C...");
    scanI2CBus( start_address, end_address, scanFunc );
    Serial.println("\nTerminado");
}
 
void loop()
{
    delay(1000);
}
