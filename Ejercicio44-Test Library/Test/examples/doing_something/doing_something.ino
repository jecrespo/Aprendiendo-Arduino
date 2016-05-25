#include <Test.h>

// Doing Something
// by aprendiendoarduino <http://www.aprendiendoarduino.com>

// Demostrates how to do something with the Test library

Test myTest = Test(2);

void setup()
{
}

void loop()
{
  myTest.doSomething();
  delay(500);
}

