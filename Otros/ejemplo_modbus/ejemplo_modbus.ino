int numero = 0;
byte respuesta [8];

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // read from port 1 (mod bus), send to port 0 (serial monitor)
  String trama = "Recibido ---> ";
  //cuando recibo algo del master
  if (Serial1.available()) {
    int i = 0;
    //leo toda la trama y la guardo en respuesta
    while (Serial1.available()) {
      byte inByte = Serial1.read();
      respuesta[i] = inByte; //La respuesta es un echo a la petición para función 0x06
      trama = trama + String(inByte, HEX) + "-";  //Trama completa para imprimir por pantalla
      i++;
      delay(30); //Para dar tiempo a que llegue toda la trama
    }
    numero++; //Numero de petición
    Serial.print("---------- ");
    Serial.print(numero);
    Serial.println(" ----------");
    Serial.println(trama);
    if (respuesta[5] == 0) {
      Serial.println("Apago Led");
      digitalWrite(LED_BUILTIN, LOW);
    }
    else {
      Serial.println("Enciendo Led");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    for (int i = 0; i < sizeof(respuesta); i++) { //repuesta mandada a master
      Serial1.write(respuesta[i]);
      Serial.print(respuesta[i], HEX);
    }
    Serial.println(" ---> Respuesta Enviada");
  }
}
