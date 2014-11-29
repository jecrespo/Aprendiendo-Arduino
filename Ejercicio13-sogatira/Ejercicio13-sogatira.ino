//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// Juego Sogatira
// 
// Made by aprendiendoarduino aprendiendoarduino
// License: CC-BY-NC-SA 3.0
// Downloaded from: http://123d.circuits.io/circuits/463198-juego-sogatira

int ledcentro = 5;
int ledA1 = 4;
int ledA2 = 3;
int ledA3 = 2;
int ledB1 = 6;
int ledB2 = 8;
int ledB3 = 9;
int botonA = 12;
int botonB = 11;
int ledFIN =13;
boolean anteriorA = HIGH;
boolean anteriorB = HIGH;
int contador = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando...");
  pinMode(ledcentro, OUTPUT);
  pinMode(ledA1, OUTPUT);
  pinMode(ledA2, OUTPUT);
  pinMode(ledA3, OUTPUT);
  pinMode(ledB1, OUTPUT);
  pinMode(ledB2, OUTPUT);
  pinMode(ledB3, OUTPUT);
  pinMode(ledB3, OUTPUT);
  pinMode(botonA, INPUT_PULLUP);
  pinMode(botonB, INPUT_PULLUP);
  blink(ledcentro,5);
  digitalWrite(ledcentro,HIGH);
  Serial.println("Comienza el juego");
  
}


void loop() {
  boolean A = digitalRead(botonA);
  boolean B = digitalRead(botonB);
  
  //deteccion de flancos
  if ((anteriorA == HIGH) && (A == LOW)){	//Flanco descendiente
    contador ++;
    Serial.print("contador = ");
  	Serial.println(contador);
    actualizaLeds();
  }
   if ((anteriorB == LOW) && (B == HIGH)){	//Flanco ascendiente
    contador --;
    Serial.print("contador = ");
  	Serial.println(contador);
    actualizaLeds();
  }

  //Actualizar estados anteriores
  anteriorA = A;
  anteriorB = B;
    
  //Cuando alguien gana
  if (contador >= 10){
    digitalWrite(ledFIN,HIGH);
    Serial.println("Gana jugador A");
    digitalWrite(ledA2,LOW);
    digitalWrite(ledFIN,HIGH);
    while (true) blink(ledA3, 1);
  }
  else if (contador <= -10){
    digitalWrite(ledFIN,HIGH);
    Serial.println("Gana jugador B");
    digitalWrite(ledB2,LOW);
    digitalWrite(ledFIN,HIGH);
    while (true) blink(ledB3, 1);
  }
  //Si no gana nadie en 30 segundos gana el que tenga mayor contador
  if (millis() > 30000){
    digitalWrite(ledFIN,HIGH);
    digitalWrite(ledcentro,LOW);
    digitalWrite(ledA1,LOW);
    digitalWrite(ledA2,LOW);
    digitalWrite(ledB1,LOW);
    digitalWrite(ledB2,LOW);
    if (contador > 0){
      	Serial.println("Gana jugador A");
    	digitalWrite(ledFIN,HIGH);
    	while (true) blink(ledA3, 1);
    }
    else if (contador < 0){
      	Serial.println("Gana jugador B");
    	digitalWrite(ledFIN,HIGH);
    	while (true) blink(ledB3, 1);
    }
    else {
      Serial.println("Empate!");
      while (true) {
        blink(ledA3, 1);
        blink(ledB3, 1);
      }
    }
    
  }
}

void blink(int pin, int count){
  for (int i = 0; i < count ; i++){
  digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
  }
}

void actualizaLeds(){
  //Actualizar los leds en funcion del contador
  switch (contador) {
    case 0:
    case 1:
    case -1:
    	digitalWrite(ledA1,LOW);
    	digitalWrite(ledB1,LOW);
    	digitalWrite(ledcentro,HIGH);
    	break;
    case 2:
    	if (digitalRead(ledcentro)) blink(ledA1, 2);
    case 3:
    case 4:
    case 5:
    	digitalWrite(ledcentro,LOW);
    	digitalWrite(ledA2,LOW);
    	digitalWrite(ledA1,HIGH);
      	break;
    case -2:
    	if (digitalRead(ledcentro)) blink(ledB1, 2);
    case -3:
    case -4:
    case -5:
      	digitalWrite(ledcentro,LOW);
    	digitalWrite(ledB2,LOW);
    	digitalWrite(ledB1,HIGH);
      	break;
    case 6:
    	if (digitalRead(ledA1)) blink(ledA2, 2);
    case 7:
    case 8:
    case 9:
      	digitalWrite(ledA1,LOW);
    	digitalWrite(ledA2,HIGH);
      break;
    case -6:
    	if (digitalRead(ledB1)) blink(ledB2, 2);
    case -7:
    case -8:
    case -9:
      	digitalWrite(ledB1,LOW);
    	digitalWrite(ledB2,HIGH);
      break;
  }
}
