/* Blink without Delay
 
 Turns on and off a light emitting diode(LED) connected to a digital  
 pin, without using the delay() function.  This means that other code
 can run at the same time without being interrupted by the LED code.
 
 The circuit:
 * LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 that's attached to pin 13, so no hardware is needed for this example.
 
 
 created 2005
 by David A. Mellis
 modified 8 Feb 2010
 by Paul Stoffregen
 
 This example code is in the public domain.
 
 
 http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 */

#include "Timer.h"
#include <MsTimer2.h>

Timer t;

// constants won't change. Used here to 
// set pin numbers:
const int ledPin =  13;      // the number of the LED pin
const int ledPin2 =  10;
const int buttonPin = 9;
boolean buttonState;

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)
int count = 0;

void setup() {
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);  
  pinMode(buttonPin, INPUT_PULLUP);
  MsTimer2::set(5000,blinkLed2);  //http://playground.arduino.cc/Main/MsTimer2
  MsTimer2::start();
  t.every(3000,printCount);
  buttonState = digitalRead(buttonPin);
  }

  void loop()
  {
    // here is where you'd put code that needs to be running all the time.

    // check to see if it's time to blink the LED; that is, if the 
    // difference between the current time and last time you blinked 
    // the LED is bigger than the interval at which you want to 
    // blink the LED.
    t.update();
    unsigned long currentMillis = millis();

    if (buttonState != digitalRead(buttonPin)){
      count++;
      buttonState = !buttonState;
    }

    if(currentMillis - previousMillis > interval) {
      // save the last time you blinked the LED 
      previousMillis = currentMillis;   

      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW)
        ledState = HIGH;
      else
        ledState = LOW;

      // set the LED with the ledState of the variable:
      digitalWrite(ledPin, ledState);
    }
  }

void printCount(){
  Serial.print("He contado ");
  Serial.print(count);
  Serial.println(" pulsaciones de boton");
}

void blinkLed2(){
  static boolean ledState = LOW;
  digitalWrite(ledPin2, ledState);
  ledState = !ledState;
}


