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
// Pines 22 a 27 para sondas de temperatura
// Pines 28 a 31 para PIR
// Conexión entre Arduino Mega y bornero con cable plano de 40 pines.
// Bornero: https://www.phoenixcontact.com/online/portal/es?uri=pxc-oc-itemdetail:pid=2315078&library=eses&tab=1
//
////////// DEFINICIONES //////////
#define VERSION	"v1.0"
#define USO_SERIE 1         		// Si no se desea emplear la salida serie, es posible no compilar esa parte poniendo esto a 0
#define N_sensores_PIR 4			// Numero de sensores PIR
#define N_sesores_T 6				// Numero de sensores de temperatura
#define TIEMPO_GRABAR_DATOS 30000 	// Cada 30 segundos grabo datos
//
////////// LIBRERIAS USADAS //////////
#include <SPI.h>             // Manejo del bus SPI que usa el ethernet shield para comunicarse con Arduino.
#include <Ethernet.h>        // Manejo del ethernet
#include "Timer.h"			 // Manejo del temporizador
//
////////// CONSTANTES & VARIABLES //////////
//
// SONDAS TEMPERATURA
int PIN_Sensores[N_sesores_T];	// Definicion de los pines digitales para cada sonda, se inicializa en el setup
float temperatura[N_sesores_T];	// Valor de la temepratura leida
float umbral_sup_temp = 25;		// Umbrales de temp fijos
float umbral_inf_temp = 15;
//
// SENSORES PIR
int PIN_PIR[N_sensores_PIR];	// Definicion de los pines digitales para cada PIR, se inicializa en el setup
int estado_PIR[N_sensores_PIR;	// Definicion estado PIR
//
//	TIMER
Timer t;
//
////////// CONFIGURACION CONEXION ETHERNET //////////
static uint8_t mac[]     = {0x90, 0xA2, 0xDA, 0xFF, 0xFF, 0xFF}; 
static uint8_t ip[]      = {192,168,1,10};		// Direccion IP de Arduino (ES LA IP RESERVADA PARA ARDUINO DESARROLLO)
static uint8_t subnet[]  = {255,255,255,0};
static uint8_t gateway[]  = {192,168,1,1};  
static uint8_t server[]  = {192,168,1,20};		// Direccion IP del servidor
EthernetClient client;    		      			//cliente ethernet que envia los datos al servidor
EthernetClient client_server;            		//cliente ethernet conectado al servidor de arduino
EthernetServer web_server = EthernetServer(80);	//Servidor ethernet que escucha en cada loop si hay algun cambio de consigna
//
////////// SETUP //////////
void setup() {
  // put your setup code here, to run once:
t.every(TIEMPO_GRABAR_DATOS, Send_Data);
#if USO_SERIE  
	Serial.begin(9600);
    Serial.println("-------------------- Inicio: MONITOR SALA ALTA SEGURIDAD " + VERSION + " --------------------");
    Serial.println("Inicializando Pines...");
    Serial.println("Inicializando Red...");
#endif
Ethernet.begin(mac, ip, gateway, gateway, subnet);
Inicializar_Pines();
Send_Evento("Reinicio");     //Mando evento de reinicio y también mando una petición de actualizar consigna para sincronizar con el servidor
}
//
////////// LOOP //////////
void loop() {
  // put your main code here, to run repeatedly:
t.update();
Leer_Sensores();	//Leo sensores en cada loop
client_server = web_server.available();		//Cliente conectado al servidor en cada loop
if (client_server > 0) Muestra_web();		//Si hay clientes esperado muestro web
}
//
////////// FUNCIONES //////////
void Inicializar_Pines(){
	
}

void Send_Evento(){
	
}

void Send_Data(){
	
}

void Leer_Sensores(){
	
}

void Muestra_web(){
	
}