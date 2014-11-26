void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH); 
  enciendo();
  delay(1000);  
  digitalWrite(13, LOW);    
  apago();
  delay(1000);  
}
