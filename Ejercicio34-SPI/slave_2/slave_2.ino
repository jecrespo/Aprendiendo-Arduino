// Written by Nick Gammon
// April 2011

#include "pins_arduino.h"

// what to do with incoming data
byte command = 0;

void setup (void)
{

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;
 
  switch (command)
  {
  // no command? then this is the command
  case 0:
    command = c;
    SPDR = 0;
    break;
    
  // add to incoming byte, return result
  case 'a':
    SPDR = c + 15;  // add 15
    break;
    
  // subtract from incoming byte, return result
  case 's':
    SPDR = c - 8;  // subtract 8
    break;

  } // end of switch

}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
  
  // if SPI not active, clear current command
  if (digitalRead (SS) == HIGH)
    command = 0;
}  // end of loop