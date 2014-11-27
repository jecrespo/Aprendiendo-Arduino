
const int ledPin = 9;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  byte brightness;
  if (Serial.available()) {
    brightness = Serial.read();
    Serial.print("He leido por el puerto serie: ");
    Serial.print(brightness);
    Serial.print("\t---->\t");
    Serial.write(brightness);
    Serial.println();
    analogWrite(ledPin, brightness);
    delay(1000);
  }
}

