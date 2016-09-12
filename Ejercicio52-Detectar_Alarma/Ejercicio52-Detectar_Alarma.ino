const int analogPin = A0;   // pin that the sensor is attached to
const int ledPin = 13;      // pin that the LED is attached to
const int threshold = 400;  // an arbitrary threshold level that's in the range of the analog input
boolean alarma;				//1 = ALARMA, 0 = NO ALARMA

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communications:
  Serial.begin(9600);
  int analogValue = analogRead(analogPin);	//Inicializo el valor de la alarma
  if (analogValue > threshold) {
    alarma = 1;
  }
  else alarma = 0;
}

void loop() {
  // read the value of the potentiometer:
  int analogValue = analogRead(analogPin);	//En cada loop compruebo el valor de la entrada analógica

  // if the analog value is high enough, turn on the LED:
  if (analogValue > threshold) {
    if (alarma == 0) {
      digitalWrite(ledPin, HIGH);
      Serial.println("ALARMA!!!!!!!!!!!!!!");
      alarma = 1;
    }
  } else {
    if (alarma == 1){
      digitalWrite(ledPin, LOW);
      Serial.println("ALARMA RECUPERADA");
      alarma = 0;
    }
  }

  // print the analog value:
  Serial.println(analogValue);
  delay(500);        // delay para leer mejor la salida

}
