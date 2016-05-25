/*
Serial Plotter Example
*/

int x = -20;

void setup() {
   Serial.begin(9600); 
}

void loop() {
    Serial.println(x*x);
    x++;
    delay(5);
    if (x == 20) x = -20;
}
