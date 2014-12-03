/* Version 0.1 beta */
/////
// TODO: Comprobar tiempos de loop, tamaño en flash y uso de memoria con la opcion de debug y verbose
/////

#define DEBUG 0
#define VERBOSE 0

void setup(){
  Serial.println(inicializa());
}
void loop(){
    Serial.println(compruebaTemperatura());
  #if DEBUG
    imprimeErrores();
  #endif
}

String inicializa() {
	String inicio = "Inicializando...";
	return inicio;
}

float compruebaTemperatura() {
	//Leer temperatura
	//Hacer acciones
	#if DEBUG
	Serial.println("Tarea finalizada");
	#endif
}

void imprimeErrores(){
	Serial.println("Errores...");
}