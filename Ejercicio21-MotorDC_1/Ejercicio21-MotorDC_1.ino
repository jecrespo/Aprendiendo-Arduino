//Ejercicio basado en los ejercicios 9 y 10 del libro Arduino Starter Kit
//Version original

//const int switchPin = 10;
const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 9;
const int directionSwitchPin = 4;
const int onOffSwitchStateSwitchPin = 5;
const int potPin = A0;

int onOffSwitchState = 0;
int previousOnOffSwitchState = 0;
int directionSwitchState = 0;
int previousDirectionSwitchState = 0;

int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 1;

//const int motorPin = 11;
//int switchState = 0;
//encendido_apagado = 0;

void setup(){
//pinMode(motorPin, OUTPUT);
//pinMode(switchPin, INPUT);
pinMode(directionSwitchPin, INPUT);
pinMode(onOffSwitchStateSwitchPin, INPUT);
pinMode(controlPin1, OUTPUT);
pinMode(controlPin2, OUTPUT);
pinMode(enablePin, OUTPUT);
digitalWrite(enablePin, LOW);
}

void loop(){
// leo el boton de encendido/apagado del pin 2
/*switchState = digitalRead(switchPin);
if ((switchState == HIGH)&& !encendido_apagado){	//Si está el motor apagado y leo un pulso
	encendido_apagado = 1;
	digitalWrite(motorPin, HIGH);
}
if (switchState == HIGH)&& encendido_apagado){	//Si está el motor encendido y leo un pulso
	encendido_apagado = 0;
	digitalWrite(motorPin, LOW);
}
*/
onOffSwitchState = digitalRead(onOffSwitchStateSwitchPin);
delay(1);
directionSwitchState = digitalRead(directionSwitchPin);
motorSpeed = analogRead(potPin)/4;

if (onOffSwitchState != previousOnOffSwitchState){
	if (onOffSwitchState == HIGH){
		motorEnabled = !motorEnabled;
	}
}

if (directionSwitchState != previousDirectionSwitchState) {
	if (directionSwitchState == HIGH){
		motorDirection = !motorDirection;
	}
}

if (motorDirection == 1) {
	digitalWrite(controlPin1, HIGH);
	digitalWrite(controlPin2, LOW);
}
else {
	digitalWrite(controlPin1,LOW);
	digitalWrite(controlPin2,HIGH);
}

if (motorEnabled == 1){
	analogWrite(enablePin,motorSpeed);
}
else {
	analogWrite(enablePin, 0);
}

previousDirectionSwitchState = directionSwitchState;
previousOnOffSwitchState = onOffSwitchState;

}