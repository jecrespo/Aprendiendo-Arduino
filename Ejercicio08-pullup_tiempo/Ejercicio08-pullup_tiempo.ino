unsigned long tiempo_loop;

void setup(){
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT); 
  tiempo_loop = millis();
}

void loop(){
  Serial.print("Tiempo Loop = ");
  Serial.println(millis() - tiempo_loop);
  tiempo_loop = millis();
  int sensorVal = digitalRead(2);
  Serial.println(sensorVal);
  if (sensorVal == HIGH) {
    digitalWrite(13, LOW);
  } 
  else {
    digitalWrite(13, HIGH);
	Serial.println("Pulsado...");
  }
}
