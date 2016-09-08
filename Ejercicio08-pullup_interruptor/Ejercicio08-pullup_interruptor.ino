int estado_anterior;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); //Boton
  pinMode(10, OUTPUT);  //Led
  estado_anterior = digitalRead(2); //Inicializo con el estado del boton al iniciar programa
}

void loop() {
  int sensorVal = digitalRead(2);
  Serial.println(sensorVal);
  if (sensorVal != estado_anterior) {
    if (sensorVal == HIGH) { //Detecto flanco ascendente, poner LOW para detectar flanco descendente
      int estado_led = digitalRead(10);
      digitalWrite(10, !estado_led);
    }
  }
  estado_anterior = sensorVal;  //Sin esto no funciona, actualizo para el siguente loop
}
