
const int ledPin = 9;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  byte brightness;
  String cadenaLeida;
  int dimmer;
  Serial.print("------------------------------------\r\nIntroduce un valor entre 0 y 100 para encender el led ---->>>>  ");
  while (Serial.available() == 0){
    //Ho hago nada
  }
  do{
    char caracter_leido = Serial.read();
    cadenaLeida += caracter_leido;
    delay(5);
  }  while (Serial.available() > 0);
  Serial.print("He Leido la cadena: ");
  Serial.println(cadenaLeida);
  
  //Necesario para poder leer un 0
  if (cadenaLeida == "0")
    dimmer = 0;
  else{
    dimmer = cadenaLeida.toInt();
    (dimmer == 0)?dimmer = -1:dimmer = dimmer; //operador ternario
  }

  //Con operador ternario
  //(cadenaLeida == '0')?dimmer = 0:dimmer = cadenaLeida.toInt();
  
  if (dimmer >= 0) {
	if (dimmer <= 100){
		dimmer = map(dimmer, 0, 100, 0, 255);
		Serial.print("El valor a aplicar al LED es: ");
		Serial.println(dimmer);
		analogWrite(ledPin, dimmer);
	}
	else Serial.println("El valor es muy alto, solo es valido un numero entre 0 y 100");
  }
  else Serial.println("No has introducido un numero");
    
}
