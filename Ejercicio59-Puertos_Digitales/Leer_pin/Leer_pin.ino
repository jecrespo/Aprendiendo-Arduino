void setup() {
  Serial.begin(9600);
  DDRB = B11111101; //Configura pin 1 de PORTB como entrada y el resto salida PIN 9
  PORTB = B00000110; // pongo a pullup el pin 9 y escribo a HIGH el pin 10
}

void loop() {
  boolean my_var = 0; //Variable para guardar la información leída en PORTD
  uint8_t my_var_int = 0;
  my_var = (PINB & (1 << PB1)); /* Lee el pin 1 de PORTD y lo coloca en la variable. */
  my_var_int = (PINB & (1 << PB1)); /* Lee el pin 1 de PORTD y lo coloca en la variable. */
  Serial.println("Selecionar pines:");
  Serial.println(1 << PB1, BIN);  //Pin 9
  Serial.println(1 << PB2, BIN);  //Pin 10
  Serial.println("Estado puerto B:");
  Serial.println(PINB, BIN);
  Serial.println("Valor del pin seleccionado en boolean y entero");
  Serial.println(my_var, BIN);
  Serial.println(my_var_int, BIN);
  Serial.println("-------");
  delay(1000);
}
