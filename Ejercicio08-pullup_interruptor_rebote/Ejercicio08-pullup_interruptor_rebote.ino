int estado_anterior;
int estado_boton; //Estado real del boton sin rebotes
unsigned long lastDebounceTime = 0;  // ultimo momento que el pin ha cambiado
unsigned long debounceDelay = 50;    // tiempo de rebote, aumentar si hay rebotes

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); //Boton
  pinMode(10, OUTPUT);  //Led
  estado_anterior = digitalRead(2); //Inicializo con el estado del boton al iniciar programa
  estado_boton = estado_anterior;
}

void loop() {
  int sensorVal = digitalRead(2);
  Serial.println(sensorVal);
  if (sensorVal != estado_anterior) {
    lastDebounceTime = millis();  //momento en que detecto un cambio de estado del boton
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    //Cuando ha pasado el tiempo con una señal estable
    if (sensorVal != estado_boton) {
      estado_boton = sensorVal;
      if (estado_boton == HIGH) { //Detecto flanco ascendente, poner LOW para detectar flanco descendente
        int estado_led = digitalRead(10);
        digitalWrite(10, !estado_led);
      }
    }
  }
  estado_anterior = sensorVal;  //Sin esto no funciona, actualizo para el siguente loop
}
