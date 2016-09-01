/* Version 0.2 */
/////
// Comprobar tama�o en flash al compilar.
/////

#define DEBUG 0		//para hacer debug
#define VERBOSE 0	//saca m�s informaci�n
#define ERRORES 0	//Provoco errores
unsigned long tiempo;

#if DEBUG	//Esto har� un consumo alto de memoria
unsigned long datos[50];
#endif

#if ERRORES
serial.println(errores)
ajs qweqwe;
int variable = "error"
#endif

void setup(){
  Serial.begin(9600);
  Serial.println(inicializa());
  Serial.println(freeRam());
#if DEBUG  
  for (int i=0; i<50; i++){
    datos[i] = i;
  }
  Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");  //Ocupa memoria en flash
#endif
}
void loop(){
  Serial.println(freeRam());
#if VERBOSE
  Serial.print("Tiempo de loop: ");
  Serial.print(millis()-tiempo);
  Serial.println(" ms");
  tiempo = millis();
#endif
  float temperatura = compruebaTemperatura();
#if DEBUG	//Decido si saco errores por serie o no y retrasa el programa para que vaya m�s lento
  imprimeErrores(temperatura);
  retrasaPrograma();
#endif
}

String inicializa() {
  String inicio = "Inicializando...";
  return inicio;
}

float compruebaTemperatura() {
  float temperatura = analogRead(A0)*5/1024;
#if VERBOSE
  Serial.println("Tarea de lectura de temperatura finalizada...");
#endif
  return temperatura;
}

void imprimeErrores(float temp){
  if ((temp > 4)||(temp <1)){
    Serial.println("Error de temperatura!!!!!!!");
  }
}

void retrasaPrograma(){
  delay(1000);
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

