/*

  Smoothing

  Reads repeatedly from an analog input, calculating a running average
  and printing it to the computer.  Keeps ten readings in an array and 
  continually averages them.
  
  The circuit:
    * Analog sensor (potentiometer will do) attached to analog input 0

  Created 22 April 2007
  By David A. Mellis  <dam@mellis.org>
  modified 9 Apr 2012
  by Tom Igoe
  http://www.arduino.cc/en/Tutorial/Smoothing
  
  This example code is in the public domain.

  Modified by jecrespo <aprendiendoarduino@gmail.com> adding fade effect in LED pin 9

*/


// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 10;
unsigned long time = 0;
int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int ledPin = 9;   //use a 220 ohms resitance

int inputPin = A0;

int lastOutputValue = 0;

void setup()
{
  Serial.println(------loop------);
  Serial.print("tiempo de bucle: ");  //for counting loop time 
  Serial.println(millis() - time);
  time = millis()
  // initialize serial communication with computer:
  Serial.begin(9600);                   
  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;          
}

void loop() {
  // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(inputPin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         
  // send it to the computer as ASCII digits
  Serial.print("Average: ");
  Serial.print(average);   
  delay(1);        // delay in between reads for stability
  
  int outputValue = map(average, 0, 1023, 0, 255); 
  
  Serial.print("\tlastOutputValue: ");
  Serial.print(lastOutputValue);
  Serial.print("\toutputValue: ");
  Serial.println(outputValue );
  
  if (outputValue > lastOutputValue){
  
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = lastOutputValue ; fadeValue <= outputValue; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 
  }
  else {
  for(int fadeValue = lastOutputValue ; fadeValue >= outputValue; fadeValue -=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);       
  }	
  } 
lastOutputValue = outputValue;
  delay(50);    //wait 50ms for delay effect
}
