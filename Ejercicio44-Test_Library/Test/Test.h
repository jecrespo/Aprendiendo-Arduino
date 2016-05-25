/*
  Test.h - Test library for Arduino - header file
  by aprendiendoarduino based on Test wiring library by John Doe.
*/

// ensure this library description is only included once
#ifndef Test_h
#define Test_h

// include types & constants of Arduino core API
#include "Arduino.h"

// library interface description
class Test
{
  // user-accessible "public" interface
  public:
    Test(int);	//Contructor
    void doSomething(void);

  // library-accessible "private" interface
  private:
    int value;
    void doSomethingSecret(void);
};

#endif

