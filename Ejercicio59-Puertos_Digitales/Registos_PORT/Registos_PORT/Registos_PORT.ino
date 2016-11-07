void setup() {
  //PORTD maps to Arduino digital pins 0 to 7
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3,HIGH);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5,HIGH);
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
  digitalWrite(7,HIGH);
  // put your setup code here, to run once:
  Serial.begin(9600);

  
}

void loop() {
  Serial.println("PIND - The Port D Input Pins Register");
  Serial.println(PIND, DEC);
  Serial.println(PIND, HEX);
  Serial.println(PIND, BIN);
  Serial.println("DDRD - The Port D Data Direction Register");
  Serial.println(DDRD, DEC);
  Serial.println(DDRD, HEX);
  Serial.println(DDRD, BIN);
  Serial.println("PORTD - The Port D Data Register");
  Serial.println(PORTD, DEC);
  Serial.println(PORTD, HEX);
  Serial.println(PORTD, BIN);

}
