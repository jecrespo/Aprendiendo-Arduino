#include <FastLED.h>
#include <Timer.h>
#include <Servo.h>

#define NUM_LEDS 64
#define DATA_PIN 6

CRGB leds[NUM_LEDS];
Servo myservo;

Timer t;
int minuteEvent;
int secondEvent;

int minutos = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  myservo.attach(7);
  myservo.write(180);

  minuteEvent = t.every(60000, minuto);
  secondEvent = t.every(1000, segundo);

  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Red;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(30);
  }

  //clear the existing led values
  FastLED.clear();
  FastLED.show();

  mueve_servo();
}

void loop() {
  t.update();
}

void minuto() {
  leds[minutos].blue = 15;
  minutos++;
  Serial.println(minutos);
  FastLED.show();
  if (minutos  == 10) {
    for (int dot = 0; dot <= minutos - 1; dot++) {
      leds[dot].green = 15;
      leds[dot].blue = 0;
      FastLED.show();
      mueve_servo();
    }
  }
  if (minutos  == 20) {
    for (int dot = 10; dot <= minutos - 1; dot++) {
      leds[dot].green = 15;
      leds[dot].red = 15;
      leds[dot].blue = 0;
      FastLED.show();
      mueve_servo();
    }
  }
  if (minutos  == 30) {
    for (int dot = 20; dot <= minutos - 1; dot++) {
      leds[dot].red = 15;
      leds[dot].blue = 15;
      FastLED.show();
      mueve_servo();
    }
  }
  if (minutos  == 40) {
    for (int dot = 30; dot <= minutos - 1; dot++) {
      leds[dot].red = 15;
      leds[dot].blue = 0;
      FastLED.show();
      mueve_servo();
    }
  }
  if (minutos  == 50) {
    t.stop(minuteEvent);
    t.stop(secondEvent);
    t.every(1000, finalizar);
    mueve_servo();
    mueve_servo();
    mueve_servo();
  }
}

void segundo() {
  static boolean encendido = false;
  if (encendido)
    leds[minutos] = CRGB::Black;
  else
    leds[minutos].blue = 15;
  encendido = !encendido;
  FastLED.show();
}

void finalizar() {
  static boolean encendido = true;
  if (encendido)
    FastLED.clear();
  else {
    for (int dot = 0; dot < NUM_LEDS; dot++) {
      leds[dot].red = 25;
    }
  }
  encendido = !encendido;
  FastLED.show();
}

void mueve_servo() {
  for (int pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(5);
  }
  for (int pos = 0; pos <= 180; pos += 1) {
    // in steps of 1 degree
    myservo.write(pos);
    delay(5);
  }
  for (int pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(5);
  }
  for (int pos = 0; pos <= 180; pos += 1) {
    // in steps of 1 degree
    myservo.write(pos);
    delay(5);
  }
}

