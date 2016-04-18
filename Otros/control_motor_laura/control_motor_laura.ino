
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
//Direccion I2C de la IMU
#define MPU 0x68

// direccion Display 0x27
// direccion EEPROM 0x57
// direccion Reloj 0x68
// Patillaje Fisico del modulo RS 4, RW 5, EN 6, D4 9, D5 10, D6 11, D7 12
// ORDEN DE LA FUNCION(addr,EN,RW,RS,D4,D5,D6,D7,BL, BL POL  );
                                                            
LiquidCrystal_I2C lcd(0x27, 16, 2);
//Ratios de conversion
#define A_R 16384.0
#define G_R 131.0
 
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779
 
//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
 
//Angulos
float Acc[2];//Acumula 2 valores es decir 0,1. "[x]"es para crear una matriz en este caso de 2 valores.
float Gy[2];
float Angle[2];
float tabla;
int velo;// velo=velocidad
int dire;// dire=dirección
//------------------------------------------//monster Motor Define variables y pines.-----------------------------------------------------------------

#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3
#define CS_THRESHOLD 100

/*VNH2SP30 pin definitions
 xxx[0] controls '1' outputs
 xxx[1] controls '2' outputs */
int inApin[2] = {7, 4};  // INA: Clockwise input "Entrada en sentido de las agujas del reloj"
int inBpin[2] = {8, 9}; // INB: Counter-clockwise input "Entrada en sentido contrario de las agujas del reloj"
int pwmpin[2] = {5, 6}; // PWM input" Entrada PWM"
int cspin[2] = {2, 3}; // CS: Current sense ANALOG input "Entrada analogica"
int enpin[2] = {0, 1}; // EN: Status of switches output (Analog pin)"Estado de la salida de los conmutadores (Pines analogicos)"

int statpin = 13;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
   pinMode(statpin, OUTPUT);
   
 // inicializa pines digitales como salidas.
  for (int i=0; i<2; i++)
  {
    pinMode(inApin[i], OUTPUT);
    pinMode(inBpin[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
  }
  // Inicializa el frenado
  for (int i=0; i<2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
  // motorGo(0, CW, 1023);
  // motorGo(1, CCW, 1023);
//--------------------------------------------------
 lcd.begin(); 
 lcd.clear();
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  //Leer los valores del Acelerometro de la IMU
   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true); //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
 
    //Se calculan los angulos Y, X respectivamente.
   Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
   Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
 
   //Leer los valores del Giroscopio
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,4,true); //A diferencia del Acelerometro, solo se piden 4 registros
   GyX=Wire.read()<<8|Wire.read();
   GyY=Wire.read()<<8|Wire.read();
 
   //Calculo del angulo del Giroscopio
   Gy[0] = GyX/G_R;
   Gy[1] = GyY/G_R;
 
   //Aplicar el Filtro Complementario
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
 /*crearemos condicion para restringir el angulo de inclinación (5º de inclinación máximo)
 (giroscopio 5º motor no se inclinara más y rectificara el ángulo)*/
tabla=Angle[1];//tabla es igual al angulo de "Y"
 if(tabla >=5){ //para cuando el angulo es mayor de 5º
  tabla=5;
  };
   if(tabla <=-5){ //para cunado el angulo es menor de 5º
  tabla=-5;
  };
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------Mapeo de mayor Ángulo mÁs velocidad de PWM --------------------------------------------------------------------------------------------------
velo=map((tabla*1000), 0,5000, 0,1023);// tabla * resolución, De 0 al valor de resolución, de 0 hasta el máximo 1023.
 if (velo < 0){
  velo = velo* -1;}// valor del PWM sea positivo siempre entre 0 y 1023.

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------Monster Motor sentido de giro------------------------------------------------------------------------------------------------------
/*Los canales del shield puenteados para obtener un suministro de corriente más alto.
Al tener los canales del monster puenteados necesitamos definir los piner en el programa de manera que vallan en paralelo en la forma de actuar.*/
if (Angle[1] < 0) //si angulo de "Y" es menor a 0 seria......
  dire=2; //giro a izquierdas; sentido contrario a las agujas del reloj.
  else //sino seria...
  dire=1; //giro  a derechas; a favor de las agujas del reloj.
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------MotorGO Instrucciones al motor-------------------------------------------------------------------------------------------------------------------------------------
motorGo(0,dire,velo);// Al estar los dos drivers unidos, tenemos que mandar lo mismo a los dos. "CH1" con los valores de dirección y velocidad.
motorGo(1,dire,velo);// Al estar los dos drivers unidos, tenemos que mandar lo mismo a los dos. "CH2" con los valores de dirección y velocidad.
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Mostrar los valores por consola
   lcd.setCursor(0, 0);//Escribe en linea 0
   
   lcd.print("Tabla:   ");
   lcd.print(tabla, 1);
   
   lcd.print(" ");//Borrar el último caracter
   lcd.setCursor(0, 1);//Escribe en linea 1
   
   lcd.print("Angle Y: ");
   lcd.print(Angle[1], 1);// para definir cuantos decimales es ,1 para que nos muestre un decimal.
   lcd.print(" ");//Borrar el último caracter
   /*
   Serial.print("Angle X: "); Serial.print(Angle[0]); Serial.print("\n");
   Serial.print("Angle Y: "); Serial.print(Angle[1]); Serial.print("\n------------\n");
   */
   delay(50); //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada medida
   
}
//--------------------------------------------Declaración de funciones--------------------------------------------------------------------------------------------
void motorOff(int motor)
{
  // Initialize braked
  for (int i=0; i<2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
  analogWrite(pwmpin[motor], 0);
}

/* motorGo()Pondra al motor a giar en una dirección especifica.
 El motor seguira girando en esa dirección y velocidad mientras no se le diga otra cosa.
  
 motor: esto debe ser 0 ó 1, con esto seleccionaremos uno de los dos motores para controlar. 
 direct: esto debe ser 0 y 3, con los siguientes resultados.
 0: frenado de VCC
 1: giro a favor del sentido de las agujas del reloj.
 2: giro contrario de las agujas del reloj.
 3: frenado de GND
 
 pwm: deberia tener un valor entre ? y 1023,cuanto más alto el nº más rapido girará.
 */
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
  if (motor <= 1)
  {
    if (direct <=4)
    {
      // Set inA[motor]
      if (direct <=1)
        digitalWrite(inApin[motor], HIGH);
      else
        digitalWrite(inApin[motor], LOW);

      // Set inB[motor]
      if ((direct==0)||(direct==2))
        digitalWrite(inBpin[motor], HIGH);
      else
        digitalWrite(inBpin[motor], LOW);

      analogWrite(pwmpin[motor], pwm);
    }
  }
}
