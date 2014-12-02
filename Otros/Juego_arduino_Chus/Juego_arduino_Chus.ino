int Boton1 = 0;
int Boton2 = 12;
int arrayleds []= {2, 3, 4, 5, 6, 7, 8};	//Guarda los pines en un array
int cantLed=7;		//tenemos 7 leds, mejora entender el código
int EstadoLed=0;	//Solo para el for, pero poniendo for (int EstadoLed=0...) me ahorro una variable global
int contar=0;
int LedEncendido=0;	//Controla el led que está encendido, es el indice del array no el pin

void setup()
{
  pinMode(Boton1, INPUT);
  pinMode(Boton2, INPUT);
  for (int i=0;i<cantLed;i++)
  {
    pinMode(arrayleds[i],OUTPUT);
  }
  digitalWrite(arrayleds[3], HIGH);            //Encendemos el LED central.
  Serial.begin(9600);
}

void loop()
{
  for(EstadoLed=0;EstadoLed<cantLed;EstadoLed++)    //Saco que led est� encendido. antes cantLed era 6 y por lo tanto no llego al 7
  {
    if (digitalRead(arrayleds[EstadoLed]))
    {
      LedEncendido=EstadoLed;	//El led encendido va del 0 al 6, primer loop es 3
    }
  }
  Serial.print("Led Encendido: ");	//Sacar por puerto serie más información para hacer debug
   Serial.println(LedEncendido);
   //delay(1000);	quito el delay porque para el programa y no nos interesa
  if ((LedEncendido>=arrayleds[0]) && (LedEncendido<=arrayleds[cantLed-1]))    //Si el led encendido esta dentro del array entre el segundo y el pen�ltimo (En los verdes).   
//No tiene mucho sentido comprobar que está entre 0 y 6, siempre entro en este if
  {
    if (digitalRead(Boton1)!=1)  //Al detectar masa                                                                                          //Si pulso el bot�n 1, el led se corre hacia la izquierda.
    {
      Serial.println("Detecto un cero en boton 1... ");
      //Serial.println(LedEncendido);	lo quito porque está repetido
      Serial.print("LedEncendido: ");
      Serial.println(LedEncendido); //en el primer loop es un 3
      //Serial.print("arrayleds: ");	//Comento codigo que da información y siempre es la misma
      //Serial.println(arrayleds[0]);
      //Serial.print("arrayleds cantidad leds: ");
      //Serial.println(arrayleds[cantLed]);
      Serial.print("pin led encendido: ");
      Serial.println(arrayleds[LedEncendido]);	//me da el pin del array encendido
      digitalWrite(arrayleds[LedEncendido], LOW);
      LedEncendido++;
      delay(130);	//Ojo aquí el código se para y no detecto eventos
      digitalWrite(arrayleds[LedEncendido], HIGH);
    }
    if (digitalRead(Boton2)!=1)                           //Si pulso el bot�n 2, el led se corre hacia la derecha.
    {
      Serial.println("Detecto un cero en boton 2... ");
      digitalWrite(arrayleds[LedEncendido], LOW);
      LedEncendido--;
      delay(130);
      digitalWrite(arrayleds[LedEncendido], HIGH);
    }
    delay(100);
  }

 // Serial.println("D");
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



