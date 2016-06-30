/* Efectos Led
   --------------
   @author: aprendiendoarduino

*/

int pinArray[] = {2, 3, 4, 5, 6, 7};
int count = 0;
int timer = 100;

void setup() {
  // we make all the declarations at once
  for (count = 0; count < 6; count++) {
    pinMode(pinArray[count], OUTPUT);
  }
}

void loop() {
  //timer = analogRead(A0);	//El valor leido por analog read es el temporizador
  for (count = 0; count < 6; count++) {
    //timer = analogRead(A0);
    digitalWrite(pinArray[count], HIGH);
    delay(timer);
  }
  for (count = 0; count < 6; count++) {
    //timer = analogRead(A0);
    digitalWrite(pinArray[count], LOW);
    delay(timer);
  }
}
