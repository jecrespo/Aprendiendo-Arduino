void setup(){
  Serial.begin(9600);
  Serial.println("Comienza...");
}

void loop(){
  Serial.println("Arduino es una plataforma de hardware libre, basada en una placa con un microcontrolador y un entorno de desarrollo, diseñada para facilitar el uso");
  Serial.println("de la electrónica en proyectos multidisciplinares.El hardware consiste en una placa con un microcontrolador Atmel AVR y puertos de entrada/salida.4");
  Serial.println("Los microcontroladores más usados son el Atmega168, Atmega328, Atmega1280, ATmega8 por su sencillez y bajo coste que permiten el desarrollo de");
  Serial.println("múltiples diseños. Por otro lado el software consiste en un entorno de desarrollo que implementa el lenguaje de programación Processing/Wiring");
  Serial.println("y el cargador de arranque que es ejecutado en la placa.Desde octubre de 2012, Arduino se usa también con microcontroladoras CortexM3 de ARM de");
  Serial.println("32 bits,5 que coexistirán con las más limitadas, pero también económicas AVR de 8 bits. ARM y AVR no son plataformas compatibles a nivel binario");
  Serial.println(", pero se pueden programar con el mismo IDE de Arduino y hacerse programas que compilen sin cambios en las dos plataformas. Eso sí, las microcontroladoras");
  Serial.println("CortexM3 usan 3,3V, a diferencia de la mayoría de las placas con AVR que generalmente usan 5V. Sin embargo ya anteriormente se lanzaron placas Arduino");
  Serial.println("con Atmel AVR a 3,3V como la Arduino Fio y existen compatibles de Arduino Nano y Pro como Meduino en que se puede conmutar el voltaje.");
  
  Serial.println(freeRam());
  
  delay(5000);
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
