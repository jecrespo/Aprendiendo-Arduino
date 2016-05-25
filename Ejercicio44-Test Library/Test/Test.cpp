/*
  Test.h - Test library for Arduino - implementation
  by aprendiendoarduino based on Test wiring library by John Doe.
*/

// include core Arduino API
#include "Arduino.h"

// include this library's description file
#include "Test.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Test::Test(int givenValue)
{
  // initialize this instance's variables
  value = givenValue;

  // do whatever is required to initialize the library
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Test::doSomething(void)
{
  // eventhough this function is public, it can access
  // and modify this library's private variables
  Serial.print("value is ");
  Serial.println(value);

  // it can also call private functions of this library
  doSomethingSecret();
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void Test::doSomethingSecret(void)
{
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
}

