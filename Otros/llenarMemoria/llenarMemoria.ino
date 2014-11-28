//Selecciona la opcion a probar
#define GLOBAL 1
#define LOCAL 0
#define STRINGS 0

long numeros [1000]; //un long ocupa 4 bytes
String frase[100];

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup(){
  Serial.begin(9600);
  Serial.println("Empiezo");
  Serial.println(freeRam());
}

void loop(){
  #if LOCAL
  long numeros2 [1000]; //un long ocupa 4 bytes
  #endif
  for (int i=0; i < 5000 ; i++){
    #if GLOBAL
    numeros[i] = i;
    #endif
    #if LOCAL
    numeros2[i] = i;
    #endif
    #if STRINGS
    frase[i] = "aaaaaaaaaa"; //10 bytes
    #endif
    Serial.print("Memoria libre: ");
    Serial.println(freeRam());
    Serial.println(i);
  }
}

