int Boton1 = 0;
int Boton2 = 12;
int arrayleds []= {2, 3, 4, 5, 6, 7, 8};
int cantLed=6;
int EstadoLed=0;
int contar=0;
int LedEncendido=0;

void setup()
{
  pinMode(Boton1, INPUT);
  pinMode(Boton2, INPUT);
  for (int i=0;i<=cantLed;i++)
  {
    pinMode(arrayleds[i],OUTPUT);
  }
  digitalWrite(arrayleds[3], HIGH);            //Encendemos el LED central.
  Serial.begin(9600);
}

void loop()
{
  for(EstadoLed=0;EstadoLed<cantLed;EstadoLed++)    //Saco que led est� encendido.
  {
    if (digitalRead(arrayleds[EstadoLed]))
    {
      LedEncendido=EstadoLed;
    }
  }
  Serial.println("A");
   Serial.println(LedEncendido);
   delay(1000);
  if ((LedEncendido>=arrayleds[0]) && (LedEncendido<=arrayleds[cantLed]))                          //Si el led encendido esta dentro del array entre el segundo y el pen�ltimo (En los verdes).   
  {
    if (digitalRead(Boton1)!=1)                                                                                            //Si pulso el bot�n 1, el led se corre hacia la izquierda.
    {
      Serial.println("B");
      Serial.println(LedEncendido);
      Serial.print("LedEncendido: ");
      Serial.println(LedEncendido);
      Serial.print("arrayleds: ");
      Serial.println(arrayleds[0]);
      Serial.print("arrayleds cantidad leds: ");
      Serial.println(arrayleds[cantLed]);
      Serial.print("arrayleds led encendido: ");
      Serial.println(arrayleds[LedEncendido]);
      digitalWrite(arrayleds[LedEncendido], LOW);
      LedEncendido++;
      delay(130);
      digitalWrite(arrayleds[LedEncendido], HIGH);
    }
    if (digitalRead(Boton2)!=1)                           //Si pulso el bot�n 2, el led se corre hacia la derecha.
    {
      Serial.println("C");
      digitalWrite(arrayleds[LedEncendido], LOW);
      LedEncendido--;
      delay(130);
      digitalWrite(arrayleds[LedEncendido], HIGH);
    }
    delay(100);
  }

  Serial.println("D");
  /*if ((LedEncendido==arrayleds[0] || LedEncendido==arrayleds[cantLed]))                                      //Si el led encendido es el primero o el �ltimo(Rojo).
   {
   Serial.println("E");
   for(contar=0;contar<100000;contar++)
   {
   digitalWrite(arrayleds[LedEncendido], HIGH);
   delay(100);
   digitalWrite(arrayleds[LedEncendido], LOW);
   delay(100);
   }
   }*/
}



