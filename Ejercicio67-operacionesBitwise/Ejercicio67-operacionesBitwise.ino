//Pin connected to ST_CP of 74HC595
int latchPin = 7;
//Pin connected to SH_CP of 74HC595
int clockPin = 6;
//Pin connected to DS of 74HC595
int dataPin = 5;
//Pin connected to Compressor 1
int comp1Pin = 2;
//Pin connected to Compressor 2
int comp2Pin = 3;

byte numberToDisplay = B00000001;

boolean sentido = 0;

void setup() {
  Serial.begin(9600);
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(comp1Pin, INPUT);
  pinMode(comp2Pin, INPUT);

}

void loop() {
  Serial.print("Rele encendido: ");
  Serial.println(numberToDisplay, BIN);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
  digitalWrite(latchPin, HIGH);
  Serial.println("Estado compresor 1: " + (String)digitalRead(comp1Pin) + " | Estado compresor 2: " + (String)digitalRead(comp2Pin));
  delay(1000);
  if (sentido) {
    numberToDisplay = numberToDisplay >> 1 ;
    if (numberToDisplay == B00000001) sentido = 0;
  }
  else {
    numberToDisplay = numberToDisplay << 1 ;
    if (numberToDisplay == B00001000) sentido = 1;
  }
  
  
}
