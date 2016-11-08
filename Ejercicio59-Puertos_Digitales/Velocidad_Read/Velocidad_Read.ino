#define LECTURAS 100000L

unsigned long tiempo = 0;

void setup() {
  Serial.begin(9600);
  pinMode(9, INPUT_PULLUP);
}

void loop() {
  long lecturas = 0;
  tiempo = micros();
  for (long i = 0; i < LECTURAS; i++) {
    lecturas += digitalRead(9);
  }
  tiempo = micros() - tiempo;
  Serial.println("Usando digitalRead() -->");
  Serial.println("Se han leido un total de " + (String)lecturas + " HIGH");
  Serial.println("La lectura ha costado " + (String)tiempo + " us");

  lecturas = 0;
  tiempo = micros();
  for (long i = 0; i < LECTURAS; i++) {
    lecturas += (boolean)(PINB & (1 << PB1));
  }
  tiempo = micros() - tiempo;
  Serial.println("Usando PINx -->");
  Serial.println("Se han leido un total de " + (String)lecturas + " HIGH");
  Serial.println("La lectura ha costado " + (String)tiempo + " us");

  delay(5000);
}
