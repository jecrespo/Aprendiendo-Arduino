//Ejercicio basado en ejemplo 3 del libro incluido en el Arduino Starter Kit
//TODO - Añadir histéresis en los cambios de estado. 
const int sensorPin = A0;
const float baselineTemp = 20.0;

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
  
  if(temperature < baselineTemp){
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }
  else if (temperature >= baselineTemp+2 && temperature < baselineTemp+4){
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }
   else if (temperature >= baselineTemp+4 && temperature < baselineTemp+6){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }
   else if (temperature >= baselineTemp+6){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
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
