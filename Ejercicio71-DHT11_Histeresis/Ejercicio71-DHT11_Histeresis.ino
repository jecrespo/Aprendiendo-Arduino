#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 4

DHT dht(DHTPIN, DHTTYPE);

const float baselineTemp = 20.0;
const int ledPin = 13;      // pin that the LED is attached to
float threshold;
const float histeresis = 0.5;  //Valor de la histeresis
boolean alarma;        //1 = ALARMA, 0 = NO ALARMA

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communications:
  Serial.begin(9600);

  dht.begin();

  float temperature = dht.readTemperature();

  if (temperature > baselineTemp) {
    alarma = 1;
    threshold = baselineTemp - histeresis;
    digitalWrite(ledPin, HIGH);
  }
  else {
    alarma = 0;
    threshold = baselineTemp + histeresis;
    digitalWrite(ledPin, LOW);
  }
}

void loop() {
  // read the value of the potentiometer:
  float temperature = dht.readTemperature();

  // if the analog value is high enough, turn on the LED:
  if (temperature > threshold) {
    if (alarma == 0) {
      digitalWrite(ledPin, HIGH);
      Serial.println("ALARMA!!!!!!!!!!!!!!");
      alarma = 1;
      threshold = baselineTemp - histeresis;
    }
  } else {
    if (alarma == 1) {
      digitalWrite(ledPin, LOW);
      Serial.println("ALARMA RECUPERADA");
      alarma = 0;
      threshold = baselineTemp + histeresis;
    }
  }

  // print values
  Serial.print(temperature);
  Serial.print(" ");
  Serial.print(threshold);
  Serial.print(" ");
  Serial.println(alarma * 1024);
  delay(50);
  delay(50);
}
