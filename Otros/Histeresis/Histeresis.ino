const int sensorPin=A0;
const float baselineTemp=19.0;
const float paso=2.0;
float histe1=0,histe2=0,histe3=0;
float histeresis=1.0;


void setup(){

  Serial.begin(9600);

  for (int pinNumber=2;pinNumber<5;pinNumber++){
    pinMode(pinNumber,OUTPUT);
    digitalWrite(pinNumber,LOW);    
  }

}

void loop(){

  int sensorVal = analogRead(sensorPin);

  Serial.print("Sensor Value:");
  Serial.print(sensorVal);

  float voltage = (sensorVal/1024.0)*5.0;

  Serial.print(", Volts: ");
  Serial.print(voltage);

  Serial.print(", degree C: ");

  float temperature = (voltage - .5)*100;
  Serial.print(temperature);
  Serial.print(" Histe actual:");

  if (temperature < baselineTemp+histe1)
  {
    histe1=0;
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    Serial.println(histe1);    

  }
  else if (temperature>=(baselineTemp+paso+histe1) && temperature<(baselineTemp+(paso*2)+histe2))
  {
    histe1=-histeresis;
    histe2=0;
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW); 
    Serial.println(baselineTemp+paso+histe1);    

  }
  else if (temperature>=(baselineTemp+(paso*2)+histe2) && temperature<(baselineTemp+(paso*3))+histe3)
  {
    histe1=0;
    histe2=-histeresis;
    histe3=0;                
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
    Serial.println(baselineTemp+(paso*2)+histe2);    

  }
  else if(temperature >=(baselineTemp+(paso*3))+histe3)
  {
    histe2=0;
    histe3=-histeresis;
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    Serial.println(baselineTemp+(paso*3)+histe3);        
  }
  delay(1);

}