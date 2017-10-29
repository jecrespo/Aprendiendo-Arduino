#include <LowPower.h> //https://github.com/rocketscream/Low-Power

// Usar pin 2 como pin para despertar y dormir
const int wakeUpPin = 2;

void setup() {
  pinMode(wakeUpPin, INPUT_PULLUP);  //uso resistencia interna pullup
  Serial.begin(9600);

}

void loop() {
  Serial.println("loop " + String(millis()));
  
  if (digitalRead(wakeUpPin) == LOW) { //detecta boton pulsado, conectado a input_pullup
    int tiempo_rebote = 0;
    while (digitalRead(wakeUpPin) == LOW) { //pequeño bucle para evitar rebotes espero al menos 150 ms pulsado para entrar en modo sleep
      delay(10);
      tiempo_rebote += 10;
      if (digitalRead(wakeUpPin) == HIGH) { //al liberar el botón
        break;
      }
    }

    if (tiempo_rebote > 150) { //Solo entro si el pin está a HIGH y ha estado es ese estado más de 150 ms para evitar rebotes botón
      delay(100);
      Serial.println("Entro en modo sleep");
      delay(100);
      // Permite despertar al poner el pin en LOW
      attachInterrupt(0, wakeUp, LOW);

      // ENTRO EN MODO SLEEP CON MODULOS DE ANALOGICO Y BROWN OUT DESHABILITADOS
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

      // deshabilito la interrupcion externa en el pin de wakeup
      detachInterrupt(0);
      Serial.println("Salgo modo sleep");
    }
  }
}

void wakeUp()
{
  // función para manejar la interrupción del wakeup
}
