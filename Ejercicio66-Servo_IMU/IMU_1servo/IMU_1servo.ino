#include "Wire.h"  // libreria I2C
#include <Servo.h>	// librer�a servo

//Direcci�n I2C de la IMU
#define MPU 0x68 // asegurarse que esta direcci�n I2C de la IMU

// Ratios de conversion especificados en la documentaci�n
// Deberemos dividir los valores que nos d� el Giroscopio y el
// Aceler�metro entre estas constantes para obtener un valor
// coherente. RAD_A_DEG es la conversi�n de radianes a grados.
#define A_R 16384.0  // aceleracion
#define G_R 131.0    // giroscopo

//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779

//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;

//Angulos
float Acc[2];
float Gy[2];
float Angle[2];

//servo
Servo myservo;

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  //Servo en pin 9
  myservo.attach(9);
  //Inicio Serial
  Serial.begin(9600);
}

void loop()
{
  //Leer los valores del Acelerometro de la IMU
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); //A partir del 0x3B, se piden 6 registros
  AcX = Wire.read() << 8 | Wire.read(); //Cada valor ocupa 2 registros
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

  //Se calculan los �ngulos Y, X respectivamente.
  Acc[1] = atan(-1 * (AcX / A_R) / sqrt(pow((AcY / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_TO_DEG;
  Acc[0] = atan((AcY / A_R) / sqrt(pow((AcX / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_TO_DEG;

  Serial.println("Angulo Acelerometro X: " + String(Acc[0]));
  Serial.println("Angulo Acelerometro Y: " + String(Acc[1]));

  //Indico posici�n al servo
  myservo.write(Acc[0]);

  //Leer los valores del Giroscopio
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 4, true); //A diferencia del Acelerometro, solo se piden 4 registros
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();

  //Calculo del angulo del Giroscopio
  Gy[0] = GyX / G_R;
  Gy[1] = GyY / G_R;

  Serial.println("Angulo del Giroscopio X: " + String(Gy[0]));
  Serial.println("Angulo del Giroscopio Y: " + String(Gy[1]));

  //Aplicar el Filtro Complementario
  Angle[0] = 0.98 * (Angle[0] + Gy[0] * 0.010) + 0.02 * Acc[0];
  Angle[1] = 0.98 * (Angle[1] + Gy[1] * 0.010) + 0.02 * Acc[1];

  //Mostrar los valores por consola
  Serial.print("Angle X: "); Serial.print(Angle[0]); Serial.print("\n");
  Serial.print("Angle Y: "); Serial.print(Angle[1]); Serial.print("\n------------\n");

  delay(10); //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada lectura (es una aproximaci�n ser�a el tiempo de loop)
}
