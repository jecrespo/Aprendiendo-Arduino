//Ejercicio basado en ejemplo 3 del libro incluido en el Arduino Starter Kit
//Se añade histeresis

const int sensorPin = A0;
const float baselineTemp = 20.0;
int estado = 0;	//inicializo, pero para hacerlo bien debo inicializarlo en el setup leyendo la temperatura. En este caso lo trato en el else.

void setup(){
  Serial.begin(9600);
  for (int pinNumber = 2; pinNumber<5; pinNumber++){
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void loop(){
  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal/1024.0)*5.0;
  float temperature = (voltage - 0.5)*100;
  
  // Estado 0 cuando la temperatura es menor que 20 o si vengo del estado 1 cuando es menor que 21
  if((temperature < baselineTemp)||((estado == 1)&&(temperature < baselineTemp+1))){
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
	estado = 0;
  }
  //Estado 1 cuando llego a 22 del estado 0, me mantengo hasta 24 o si vengo de estado 2 vuelvo cuando bajo a 23
  else if (((estado == 0) && (temperature >= baselineTemp+2)) || ((estado == 2)&&(temperature < baselineTemp+3))){
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
	estado = 1;
  }
  //Estado 2 cuando llego a 24 del estado 1, me mantengo hasta 26 y vuelvo cuando bajo a 25
   else if (((estado == 1) && (temperature >= baselineTemp+4)) || ((estado == 3)&&(temperature < baselineTemp+5))){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
	estado = 2;
  }
  //Estado 3 cuando la temperatura es mayor que 26
   else if (temperature >= baselineTemp+6){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
	estado = 3;
  }
  else {	//Para tratar el resto de casos, por ejemplo cuando hay saltos muy grandes de temperatura o en el primer loop
	//Condiciones con if, elseif 
  }
  delay(1);
  
  if (Serial.available() > 0){
    char caracterLeido = Serial.read();
    if (caracterLeido == 'c' || caracterLeido == 'C'){
      Serial.print("Sensor Value: ");
      Serial.print(sensorVal);
      Serial.print(", Volts: ");
      Serial.print(voltage);
      Serial.print(", degrees C: ");
      Serial.println(temperature);
    }
    else if (caracterLeido == 'f' || caracterLeido == 'F'){
      Serial.print("Sensor Value: ");
      Serial.print(sensorVal);
      Serial.print(", Volts: ");
      Serial.print(voltage);
      Serial.print(", degrees F: ");
      Serial.println(temperature*1.8+32.0);
    }
    else Serial.println("Caracter incorrecto... c o C para celsius y f o F para fahrenheit");
}
}
