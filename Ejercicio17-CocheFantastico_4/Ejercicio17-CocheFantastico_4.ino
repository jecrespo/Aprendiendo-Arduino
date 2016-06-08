/* Knight Rider 4
   --------------

   Based on Knight Rider 2 but change timer received by serial port

*/

int pinArray[] = {2, 3, 4, 5, 6, 7};
int count = 0;
int timer = 100;

void setup() {

  Serial.begin(9600);
  for (count = 0; count < 6; count++) {
    pinMode(pinArray[count], OUTPUT);
  }
}

void loop() {
  for (count = 0; count < 6; count++) {
    if (Serial.available()) timer = lee_timer();
    Serial.println("Valor de timer = " + String(timer));
    digitalWrite(pinArray[count], HIGH);
    Serial.println("Led " + String(count) + " ENCENDIDO");
    delay(timer);
    digitalWrite(pinArray[count], LOW);
    Serial.println("Led " + String(count) + " APAGADO");
    delay(timer);
  }
  for (count = 5; count >= 0; count--) {
    if (Serial.available()) timer = lee_timer();
    Serial.println("Valor de timer = " + String(timer));
    digitalWrite(pinArray[count], HIGH);
    Serial.println("Led " + String(count) + " ENCENDIDO");
    delay(timer);
    digitalWrite(pinArray[count], LOW);
    Serial.println("Led " + String(count) + " APAGADO");
    delay(timer);
  }
}

int lee_timer() {
  String cadena_Leida = "";
  do {
    char caracter_leido = Serial.read();
    cadena_Leida += caracter_leido;
    delay(5);
  }  while (Serial.available() > 0);

  int nuevo_timer = cadena_Leida.toInt();

  if (nuevo_timer >= 10 && nuevo_timer <= 500) {
    Serial.println("Nuevo timer establecido a " + cadena_Leida + "segundos");
    return nuevo_timer;
  }
  else {
    Serial.println("El valor del timer debe estar entre 10 y 500 ms, introduce un valor correcto");
    return timer;
  }
}

