/*
  String Numeros
*/
String leido = "";
int numeroLeido;

void setup() {
  Serial.begin(9600);
  Serial.println("Introduce un numero entero:");
  while (Serial.available() == 0) {
    //Ho hago nada
  }

  do {
    char caracter_leido;
    caracter_leido = Serial.read();
    leido += caracter_leido;
    delay(5);
  }  while (Serial.available() > 0);

  Serial.println("He leido la cadena: \"" + leido + "\"");

  Serial.println(leido.toInt());

  if ((leido.toInt() != 0)||(leido =="0")) {
    numeroLeido = leido.toInt();
    Serial.print("Tu numero multiplicado por 15 es: ");
    Serial.println(numeroLeido * 15);
  }
  else Serial.println("No has introducido un numero");
}

void loop() {

  //no hago nada

}

