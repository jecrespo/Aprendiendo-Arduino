
void setup(){
  Serial.begin(9600);
  Serial.write(0xFE);
  Serial.write(0x00);
  Serial.print("Inicio");
  delay(1000);
}


void loop() {
  Serial.write(0xFE);
  Serial.write(0x00);
  Serial.print("Tiempo: ");
  Serial.write(0xFE);
  Serial.write(192);
  Serial.print(millis());
  delay(1000);
  
}
