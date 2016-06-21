//Ejercicio basado en los ejercicios 9 y 10 del libro Arduino Starter Kit
//Proyecto Motor DC
//Modificaciones realizadas para:
//* Añadir una rampa de arranque y otra de parada cuando se detecte el encendido y el apagado
//* Añadir una rampa de arranque y parada cuando se detecte un cambio de dirección
//* Añadir un boton de emergencia

#define SEGURIDAD 0		//Habilita o deshabilita las rampas de arranque y parada

//CONSTANTES
const int controlPin1 = 2;	//Pin de control conectado al pin7 del puente H
const int controlPin2 = 3;	//Pin de control conectado al pin2 del puente H
const int enablePin = 9;	//Pin activación conectado al pin1 del puente H
const int directionSwitchPin = 4;	//Pin conectado al boton de direccion
const int onOffSwitchStateSwitchPin = 5;	//Pin conectado al boton de envedido/apagado
const int potPin = A0;	//Pin conectado al potenciometro
const int emergenciaPin = 11;	//Pin conectado al botón de emergencia

//VARIABLES
int onOffSwitchState = 1;	//Guardo el estado de encendido/apagado. Valor leido en el loop actual del boton.
int previousOnOffSwitchState = 1;	//Guardo el estado anterior de encendido/apagado. Valor leido en el loop anterior.
int directionSwitchState = 1;	//Guardo el estado de direccion (valores 1 y 0). Valor leido en el loop actual del boton.
int previousDirectionSwitchState = 1;	//Guardo el estado anterior de direccion. Valor leido en el loop anterior.
int motorEnabled = 0;	//Si el motor está activado o no
int motorSpeed = 0;		//Velocidad del motor. Se actualiza en cada loop al leer el potenciometro.
int motorDirection = 1;	//Valor de la dirección del motor
int previousMotorSpeed = 0;	//Para detectar cambios en la velocidad del motor

void setup(){
  Serial.begin(9600);
  pinMode(directionSwitchPin, INPUT_PULLUP);  // Lo hago todo in input pullup y detecto flancos ascendientes
  pinMode(onOffSwitchStateSwitchPin, INPUT_PULLUP);
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(emergenciaPin, INPUT_PULLUP);
  digitalWrite(enablePin, LOW);	//Motor apagado en el setup
  Serial.println("Iniciando...");
}

void loop(){
  if (digitalRead(emergenciaPin) == HIGH){	//PRIORITARIO, leo primero el boton de emergencia, solo ejecuto el control si está a 1
    //Primero leo el estado de los botones y del potenciometro
    onOffSwitchState = digitalRead(onOffSwitchStateSwitchPin);
    delay(1);
    directionSwitchState = digitalRead(directionSwitchPin);
    motorSpeed = analogRead(potPin)/4;
    //Si detecto cambio velocidad. Doy prioridad a esto sobre pulsar el boton. Solo si motor está enabled
    if ((previousMotorSpeed != motorSpeed) && motorEnabled){
#if SEGURIDAD
      rampa(previousMotorSpeed,motorSpeed);
#else
      analogWrite(enablePin, motorSpeed);
#endif
    }
    //Si detecto flanco ascendente en el boton de arranque/parada
    if (onOffSwitchState != previousOnOffSwitchState){
      if (onOffSwitchState == HIGH){
        motorEnabled = !motorEnabled;
        Serial.println("Pulsado boton encendido/apagado....");
        if (motorEnabled == 1) {	//Si toca arrancar
#if SEGURIDAD
          rampa(0,motorSpeed);	//Rampa arranque
#else
          analogWrite(enablePin, motorSpeed);
          Serial.println("Arranco Motor sin rampa");
#endif
          }
        else {	//Si toca parar
#if SEGURIDAD
          rampa(motorSpeed,0);	//Rampa parada
#else
          analogWrite(enablePin, 0);
          Serial.println("Paro Motor sin rampa");
#endif
          }
        }
      }
      //Si detecto flanco ascendente en el boton de arranque/parada
      if (directionSwitchState != previousDirectionSwitchState) {	
        if (directionSwitchState == HIGH){
          motorDirection = !motorDirection;
          Serial.println("Pulsado boton cambio de direccion...");
          if (motorEnabled == 1){		//solo cambio sentido si el motor encendido pero si acumulo en cambio direccion
            if (motorDirection == 1) {
              Serial.println("cambio direccion parada....");
#if SEGURIDAD
              rampa(motorSpeed,0);	//Rampa parada
#endif
              digitalWrite(controlPin1, HIGH);
              digitalWrite(controlPin2, LOW);
              Serial.println("cambio direccion arranque....");
#if SEGURIDAD
              rampa(0,motorSpeed);	//Rampa arranque
#endif
            } 
            else {
              Serial.println("cambio direccion parada....");
#if SEGURIDAD
              rampa(motorSpeed,0);	//Rampa parada
#endif
              digitalWrite(controlPin1, LOW);
              digitalWrite(controlPin2, HIGH);
              Serial.println("cambio direccion arranque....");
#if SEGURIDAD
              rampa(0,motorSpeed);	//Rampa arranque
#endif
            }  
          }
        }
      }
    previousDirectionSwitchState = directionSwitchState;
    previousOnOffSwitchState = onOffSwitchState;
    previousMotorSpeed = motorSpeed;
  }
  else {
    Serial.println("Parada Emergencia");	//Cuando está pulsado (función de seta mergencia), paro el motor inmediatamente
    analogWrite(enablePin,0);
    previousMotorSpeed = 0;    //Actualizo la velocidad del motor.
  }
}

void rampa(int velocidadInicio, int velocidadFin){
  if (velocidadFin > velocidadInicio){
    Serial.print("Rampa Ascendente de ");
    Serial.print(velocidadInicio);
    Serial.print(" a ");
    Serial.println(velocidadFin);
    for (int i = velocidadInicio; i <= velocidadFin; i++){
      analogWrite(enablePin,i);
      Serial.print(i);
      Serial.print("-");
      delay(5);
    }
    Serial.println();
  }
  else{
    Serial.print("Rampa Descendente de ");
    Serial.print(velocidadInicio);
    Serial.print(" a ");
    Serial.println(velocidadFin);
    for (int i = velocidadInicio; i >= velocidadFin; i--){
      analogWrite(enablePin,i);
      Serial.print(i);
      Serial.print("-");
      delay(5);
    }
    Serial.println();
  }
}
