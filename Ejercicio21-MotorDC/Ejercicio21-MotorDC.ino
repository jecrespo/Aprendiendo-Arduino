//Ejercicio basado en los ejercicios 9 y 10 del libro Arduino Starter Kit

//const int switchPin = 10;
const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 9;
const int direccionSwitchPin = 4;
Const int onOffSwitchPin = 5;
const int potPin = A0;

int onOffSwitchState = 0;
int previousOnOffSwitchState = 0;
int direccionSwitchState = 0;
int previousDirectionSwitchState = 0;

//const int motorPin = 9;
//int switchState = 0;
//encendido_apagado = 0;

void setup(){
pinMode(motorPin, OUTPUT);
pinMode(switchPin, INPUT);
}

void loop(){
// leo el boton de encendido/apagado del pin 2
switchState = digitalRead(switchPin);
if ((switchState == HIGH)&& !encendido_apagado){	//Si está el motor apagado y leo un pulso
	encendido_apagado = 1;
	digitalWrite(motorPin, HIGH);
}
if (switchState == HIGH)&& encendido_apagado){	//Si está el motor encendido y leo un pulso
	encendido_apagado = 0;
	digitalWrite(motorPin, LOW);
}

}