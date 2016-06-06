/*
  String vs string
*/

String stringOne, stringTwo;
char cadenaOne[] = {'t', 'h', 'i', 's', '\0'};
char cadenaTwo[] = "that";

void setup() {
  Serial.begin(9600);
  stringOne = String("this");
  stringTwo = String("that");
}

void loop() {
  //Dos Strings Iguales//
  //Strings
  if (stringOne == "this") {
    Serial.println("StringOne == \"this\"");
  }

  //cadenas
  if (cadenaOne == "this") {  //Al compilar de un warning
    Serial.println("cadenaOne == \"this\"");
  }
  else Serial.println("Esta comparacion no funciona: cadenaOne == \"this\"");

  if (cadenaTwo == "that") {
    Serial.println("cadenaTwo == \"that\"");
  }
  else Serial.println("Esta comparacion no funciona: cadenaTwo == \"that\"");

  const char cadena[5] = "this"; //probar a poner \0 al final
  if (!strcmp (cadenaOne, cadena)) {
    Serial.println("Esta comparacion si funciona con strcmp()");
  }
  else Serial.println(strcmp (cadenaOne, cadena));

  Serial.println();

  //Dos Strings Distintos//
  //Strings
  if (stringOne != stringTwo) {
    Serial.println(stringOne + " =! " + stringTwo);
  }

  //cadenas
  if (cadenaOne != cadenaTwo) {  //Ojo no estamos comparando las cadenas de caracteres sino los punteros
    // Serial.println(cadenaOne + " =! " + cadenaTwo);  //Error de compilacion
  }

  if (strcmp (cadenaOne, cadenaTwo)) {
    Serial.print("Resultado de la comparacion de cadenas = ");
    Serial.println(strcmp (cadenaOne, cadenaTwo));
    char resultado[30] = "";
    strcat(resultado, cadenaOne);
    strcat(resultado, " =! ") ;
    strcat(resultado, cadenaTwo);
    Serial.println(resultado);
  }
  else {
    Serial.println("Las cadenas cadenaOne y cadenaTwo son iguales");
    char resultado[30] = "";
    strcat(resultado, cadenaOne);
    strcat(resultado, " = ") ;
    strcat(resultado, cadenaTwo);
    Serial.println(resultado);
  }


  while (true) {
    //no hago nada
  }
}

