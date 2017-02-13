// MONITOR SALA ALTA SEGURIDAD
// FUNCION: Gestion inteligente sala de alta seguridad
// Se montan 6 sondas de temperatura tipo DS18B20 con encapsulado metálico: http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf
// Se usan 4 sensores de movimiento (PIR) tipo HC-SR501: https://electronilab.co/wp-content/uploads/2013/12/HC-SR501.pdf
// Los datos se mandan por POST al servidor web que los almacena en una BBDD
// Se mandan los datos de: Temperatura y estado de PIR 
// Los eventos son: Temperatura fuera de rango, PIR activado, PIR desactivado y reinicio Arduino.
// Web embebida que muestra los datos y alertas en tiempo real.
// Gestión de los SMSs se hace desde la plataforma al recibir una alerta.
//
// HW: Para Arduino MEGA con EthernetShield.
// ARDUINO MEGA:
// Flash Memory	: 256 KB, de los cuales 8KB se usan por el bootloader
// SRAM           8 KB
// EEPROM         4 KB
//
// PINES UTILIZADOS
//
//
////////// DEFINICIONES //////////
//
//
////////// LIBRERIAS USADAS //////////
//
//
////////// CONSTANTES & VARIABLES //////////
//
// SONDAS TEMPERATURA
//
// SENSORES PIT
//
//	TIMER
//
////////// CONFIGURACION CONEXION ETHERNET //////////
//
//
////////// SETUP //////////
void setup() {
  // put your setup code here, to run once:

}
//
////////// LOOP //////////
void loop() {
  // put your main code here, to run repeatedly:

}
//
////////// FUNCIONES //////////
//
//