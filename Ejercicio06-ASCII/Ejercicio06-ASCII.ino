void setup() { 
  Serial.begin(9600); 
  Serial.println("ASCII Table ~ Character Map"); 
} 

int thisByte = 0; 

void loop() { 
  Serial.write(thisByte);    
  Serial.print(", dec: "); 
  Serial.print(thisByte);      
  Serial.print(", hex: "); 
  Serial.print(thisByte, HEX);     
  Serial.print(", oct: "); 
  Serial.print(thisByte, OCT);     
  Serial.print(", bin: "); 
  Serial.println(thisByte, BIN);   
  if(thisByte == 255) {
    while(true) { 
      continue; 
    } 
  } 
  // go on to the next character
  thisByte++;  
}
