//Demostración retrasos de librería timer vs mstimer2
//
//by aprendiendoarduino 2016
//
//Beerware license: Free for any and all purposes, but if you find it

#include "Timer.h"                     //http://github.com/JChristensen/Timer
#include <MsTimer2.h>
Timer t;                               //instantiate the timer object

void setup()
{
  t.every(500, funcion1);
  MsTimer2::set(500, funcion2); // 500ms period
  MsTimer2::start();
  Serial.begin(250000);
}

void loop(void)
{
  t.update();
  Serial.println("************************ LOOP **********************");
  Serial.print("Tiempo de loop: ");
  Serial.println(millis());
  delay (2000);
}

void funcion1() {
  Serial.print("++++++++++ejecuto función1 cada 500ms y es el tiempo ");
  Serial.println(millis());
  delay(100); //simulo que la función tarda 100 ms
  //Esta función acumula un retraso de 100ms cada vez que se ejecuta
}

void funcion2() {
  Serial.print("----------ejecuto función2 cada 500ms y es el tiempo ");
  Serial.println(millis());
  delay(200); //simulo que la función tarda 200 ms, pero no se ejecuta ¿porque? https://www.arduino.cc/en/Reference/AttachInterrupt
}
