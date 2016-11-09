#include <EEPROM.h>
 
void setup(){
  Serial.begin(57600);
 
  e2reader();
}
 
void loop(){
}
 
void e2reader(){
  char buffer[16];
  char valuePrint[4];
  byte value;
  unsigned int address;
  uint8_t trailingSpace = 2;
 
  Serial.print("Dumping "); Serial.print(E2END + 1);
  Serial.println(" bytes from EEPROM.");
  Serial.print("baseAddr ");
  for(int x = 0; x < 2; x++){
    Serial.print(" ");
    for(int y = 0; y < 25; y++)
      Serial.print("=");
  }
 
  // E2END is a macro defined as the last EEPROM address
  // (1023 for ATMEGA328P)
  for(address = 0; address <= E2END; address++){
    // read a byte from the current address of the EEPROM
    value = EEPROM.read(address);
 
    // add space between two sets of 8 bytes
    if(address % 8 == 0)
      Serial.print("  ");
 
    // newline and address for every 16 bytes
    if(address % 16 == 0){
      //print the buffer
      if(address > 0 && address % 16 == 0)
        printASCII(buffer);
 
      sprintf(buffer, "\n 0x%05X: ", address);
      Serial.print(buffer);
 
      //clear the buffer for the next data block
      memset (buffer, 32, 16);
    }
 
    // save the value in temporary storage
    buffer[address%16] = value;
 
    // print the formatted value
    sprintf(valuePrint, " %02X", value);
    Serial.print(valuePrint);
  }
 
  if(address % 16 > 0){
    if(address % 16 < 9)
      trailingSpace += 2;
 
    trailingSpace += (16 - address % 16) * 3;
  }
 
  for(int i = trailingSpace; i > 0; i--)
    Serial.print(" ");
 
  //last line of data and a new line
  printASCII(buffer);
  Serial.println();
}
 
void printASCII(char * buffer){
  for(int i = 0; i < 16; i++){
    if(i == 8)
      Serial.print(" ");
 
    if(buffer[i] > 31 and buffer[i] < 127){
      Serial.print(buffer[i]);
    }else{
      Serial.print(".");
    }
  }
}
