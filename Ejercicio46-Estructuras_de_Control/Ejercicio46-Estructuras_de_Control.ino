/*
  Estructuras de Control
  Menu interactivo
*/

void setup() {
  Serial.begin(9600);
  String titulo = "Menu Interactivo con Arduino";
  for (int i = 0; i < titulo.length(); i++) {
    Serial.print("-");
  }
  Serial.print("\n" + titulo + "\n");
  for (int i = 0; i < titulo.length(); i++) {
    Serial.print("-");
  }
  Serial.println();
}

void loop() {
  int respuesta;
  do {
    respuesta = menu();
    switch (respuesta) {
      case 1:
        Serial.println("Ejecucion de Opcion 1");
        delay(5000);
        break;
      case 2:
        Serial.println("Ejecucion de Opcion 2");
        delay(5000);
        break;
      case 3:
        Serial.println("Ejecucion de Opcion 3");
        delay(5000);
        break;
      case 4:
        Serial.println("Adios...");
        while (true) {
          //se queda aqui el programa
        }
        break;
      default:
        Serial.println("Opcion de menu incorrecta!!!!!");
        break;
    }
    limpia_terminal();
  } while ((respuesta < 1) || (respuesta > 4));
}

int menu() {
  String leido = "";

  Serial.println("Elige opcion a ejecutar:");
  Serial.println("1 - Multiplicar un numero por 15");
  Serial.println("2 - Repetir 10 veces una cadena");
  Serial.println("3 - Repetir \"hola\" hasta pulsar una tecla");
  Serial.println("4 - Fin");

  while (Serial.available() == 0) {
    //Ho hago nada
  }

  do {
    char caracter_leido;
    caracter_leido = Serial.read();
    leido += caracter_leido;
    delay(5);
  }  while (Serial.available() > 0);

  if (leido.toInt() != 0)
    return leido.toInt();
  else return 0;

}

void limpia_terminal() {		//Solo v�lido para usar con putty
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}
