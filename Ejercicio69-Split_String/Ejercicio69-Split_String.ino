String strTest = "123,456,789,abc";

void setup() {
  Serial.begin(9600);
  Serial.println("Longitud del String: " + (String)strTest.length());
  int i = num_ocurrencias(strTest, ",");
  Serial.println("Número de ocurrencias: " + (String)i);
  String dividido[i + 1];
  split(strTest, ",", dividido);
  Serial.println("Resultado de la división es: ");
  for (int j = 0; j < i + 1 ; j++) {
    Serial.println(dividido[j]);
  }
}

void loop() {

}

void split (String str, String delimitador, String* resultado) {
  int tam = num_ocurrencias(str, delimitador);
  Serial.println("Se va a dividir en " + (String)(tam + 1) + " trozos");
  int indice = 0;
  for (int i = 0; i < tam + 1; i++) {
    resultado[i] = str.substring(indice, str.indexOf(delimitador, indice + 1));
    Serial.println("Trozo " + (String)(i + 1) + " desde " + indice + " hasta " + String(str.indexOf(delimitador, indice + 1)) + " es " + resultado[i]);
    indice = str.indexOf(delimitador, indice + 1) + 1;
  }
}

int num_ocurrencias (String str, String delimitador) {
  int ocurrencias = 0;
  int indice = str.indexOf(delimitador);
  while (indice >= 0) {
    ocurrencias++;
    Serial.println("Ocurrencia " + (String)ocurrencias + " en la posición " + (String)indice);
    indice = str.indexOf(delimitador, indice + 1);
  }
  return ocurrencias;
}
