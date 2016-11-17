const int LEDPin = 13;
const int intPin = 2;
volatile int frecuencia = 1000;
volatile unsigned long tiempo_interrupcion = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
  pinMode(intPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(intPin), blink, RISING);
}

void loop() {
  //esta parte es para emular la salida
  digitalWrite(LEDPin, HIGH);
  delay(frecuencia);
  digitalWrite(LEDPin, LOW);
  delay(frecuencia);

  if (tiempo_interrupcion > 0) {
    Serial.println("interrupcion lanzada");
    if ((millis() - tiempo_interrupcion) > 5000) {
      frecuencia = 1000;
      tiempo_interrupcion = 0;
      Serial.println("Vuelvo a estado normal");
    }
  }

}

void blink() {
  frecuencia = 100;
  tiempo_interrupcion = millis();
}
