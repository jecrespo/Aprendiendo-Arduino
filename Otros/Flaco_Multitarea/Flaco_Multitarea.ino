#define TIEMPO_ENCENDIDO 5000

boolean estado_alarma = 0;
unsigned long tiempo_flanco = 0;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (tiempo_flanco > 0){
    if ((millis() -  tiempo_flanco) > TIEMPO_ENCENDIDO){
      digitalWrite(13, LOW);
      tiempo_flanco = 0;
      Serial.println("Apago Led");
    }
  }
  
  int lectura = digitalRead(7);
  if (lectura == 0) {
    if (estado_alarma == 0) {
      Serial.println("ALARMA!!!!!!");
      digitalWrite(13, HIGH);
      //tiempo_flanco = millis(); //Opcion donde reinicio el contador de de tiempo de encendido
      if (tiempo_flanco == 0) tiempo_flanco = millis();
      estado_alarma = 1;
    }
  }
  else {
    if (estado_alarma == 1) {
      estado_alarma = 0;
      Serial.println("ALARMA RECUPERADA");
    }
  }
}
