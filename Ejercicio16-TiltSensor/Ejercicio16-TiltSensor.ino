/* Hagamos un sistema autonomo que detecte cambios 
de inclinación y emita un sonido el piezo.
*/
 
#define NOTE_C6  1047

// these constants won't change:
const int PinSensor = 2;      // Sensor de inclinación en el pin 2
const int soundOutput = A0; // the piezo is connected to analog pin 0

void setup() {
 pinMode(PinSensor, INPUT_PULLUP); 
 Serial.begin(9600);       // use the serial port
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  if (digitalRead(PinSensor)){
    Serial.println("Sonido!!!!!!!");
    tone(soundOutput,NOTE_C6);
  }
  else
    noTone(soundOutput);

  delay(100);
}
