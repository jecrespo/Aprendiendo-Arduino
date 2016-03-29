//Función:  Ejemplo avanzado de ajax

//----------------------- LIBRARIES ------------------------//
#include <SPI.h>                  // Manejo del bus SPI que usa el ethernet shield para comunicarse con Arduino.
#include <Ethernet.h>             // Manejo del puerto ethernet 

//----------------------- CONSTANTS ------------------------//
#define VERSION "v1.1"
#define DEBUG 1

//----------------------- VARIABLES RED ------------------------//
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x70, 0x22};
byte ip[] = {192, 168, 1, 179};
byte dns_server[] = {8, 8, 8, 8};
byte gateway[] = {192, 168, 1, 1};
byte subnet[] = {255, 255, 255, 0};
EthernetServer server = EthernetServer(80);

//----------------------- VARIABLES ------------------------//
boolean led = 0; //0 = OFF, 1 = ON
boolean manual = 0;  // 1 = que lo conecto manualmente
int umbral = 1000;	//Valor por defecto

//--------------------- FUNCTIONS --------------------------//

void muestroWeb(EthernetClient &client_web) {
  Serial.println(F("Muestro web"));
  client_web.println(F("HTTP/1.1 200 OK"));
  client_web.println(F("Content-Type: text/html"));
  client_web.println();
  client_web.print("<!DOCTYPE html>");  //Si pongo F Chrome me encapsula la web
  client_web.print(F("<html><head><title>Ejemplo Ajax en Arduino</title>"));
  client_web.print(F("<style type=\"text/css\">body {color: black;background-color: #D7DBDA;}"));
  client_web.print(F("div {background-color: #006666; color: white; border: 1px solid black; padding: 5px; margin: 5px; height: auto;"));
  client_web.print(F("width: 100px; border-radius: 10px; text-align: center; float: left; font-weight:bold;} .cabecera {clear: left;}"));
  client_web.print(F(".formulario {background-color: #004F00; width: auto;}.oculto{background-color: #004F00; border: none;}"));
  client_web.print(F(".OFF {background-color: #006666;} .ON {background-color: red;}"));
  client_web.print(F("input, #boton {width: 80px; border-radius: 5px; font-weight:bold;text-align: center;}#boton:hover{border: 3px solid black;}"));
  client_web.print(F("#boton:hover{border: 3px solid black;} #principal {background-color: #000F00;}"));
  client_web.print(F("</style>"));
  client_web.print(F("<script type=\"text/javascript\">function update(){var xhttp;xhttp=new XMLHttpRequest();xhttp.onreadystatechange=function(){"));
  client_web.print(F("if (xhttp.readyState == 4 && xhttp.status == 200) {var json = JSON.parse(xhttp.responseText);"));
  client_web.print(F("document.getElementById(\"A0\").innerHTML = json.arduino.A0;"));
  client_web.print(F("document.getElementById(\"A1\").innerHTML = json.arduino.A1;"));
  client_web.print(F("document.getElementById(\"A2\").innerHTML = json.Sarduino.A2;"));
  client_web.print(F("document.getElementById(\"D13\").innerHTML = json.arduino.led;"));
  client_web.print(F("document.getElementById(\"D13\").className = json.arduino.led;"));
  client_web.print(F("document.getElementById(\"boton\").innerHTML = json.arduino.manual;"));
  client_web.print(F("document.getElementById(\"boton\").className = json.arduino.manual;"));
  client_web.print(F("document.getElementById(\"dato_input\").value = json.arduino.umbral;}};"));
  client_web.print(F("xhttp.open(\"GET\", \"http://")); //Imprimo la IP del propio arduino
  for (int i = 0; i < 4; i++) {
    client_web.print(ip[i]);
    if (i < 3) client_web.print(F("."));
  }
  client_web.print(F("/ajax\", true);xhttp.send();}"));
  client_web.print(F("function boton_manual(){var xhttp;xhttp = new XMLHttpRequest();var boton = document.getElementById(\"boton\").innerHTML;"));
  client_web.print(F("if (boton == \"ON\" ){boton = \"OFF\";document.getElementById(\"boton\").className = 'OFF';}"));
  client_web.print(F("else {boton = \"ON\";document.getElementById(\"boton\").className = 'ON';}document.getElementById(\"boton\").innerHTML = boton;var url = \"http://"));
  for (int i = 0; i < 4; i++) {
    client_web.print(ip[i]);
    if (i < 3) client_web.print(F("."));
  }
  client_web.print(F("/boton/\" + boton; xhttp.open(\"GET\", url, true); xhttp.send();update();}"));
  client_web.print(F("function cambia_umbral(){var xhttp; xhttp = new XMLHttpRequest();var valor= document.getElementById(\"dato_input\").value; var url = \"http://"));
  for (int i = 0; i < 4; i++) {
    client_web.print(ip[i]);
    if (i < 3) client_web.print(F("."));
  }
  client_web.print(F("/umbral/\" + valor; xhttp.open(\"GET\", url, true); xhttp.send();setTimeout(function(){update()},500);}"));
  client_web.print(F("function cambiaIntervalo(tiempo){if (tiempo==0) clearInterval(id_interval);else id_interval = setInterval('update()',tiempo);}"));
  client_web.print(F("var id_interval = setInterval('update()',5000);"));
  client_web.print(F("</script></head><body>"));
  client_web.print(F("<div id = \"principal\" class=\"formulario\"><h2>Ejemplo Ajax en Arduino</h2><div id=\"datos\" class=\"formulario\">"));
  client_web.print(F("<p>DATOS</p><div class=\"cabecera\">Entrada A0</div><div id=\"A0\">"));
  client_web.print("N/A");
  client_web.print(F("</div><div class=\"cabecera\">Entrada A1</div><div id=\"A1\">"));
  client_web.print("N/A");
  client_web.print(F("</div><div class=\"cabecera\">Entrada A2</div><div id=\"A2\">"));
  client_web.print("N/A");
  client_web.print(F(" %</div><div class=\"cabecera\">Led</div><div id=\"D13\" "));
  led ? client_web.print(F("class=\"ON\">ON")) : client_web.print(F("class=\"OFF\">OFF"));
  client_web.print(F("</div></div><div class=\"formulario\"><p>CONFIGURACION</p><div class=\"cabecera\">Umbral Entrada A1</div>"));
  client_web.print(F("<div id=\"umbral\" class=\"oculto\"><input id=\"dato_input\" type=\"number\" onchange=\"cambia_umbral()\" value=\""));
  client_web.print(umbral);
  client_web.print(F("\" onmouseover=\"cambiaIntervalo(0)\" onmouseout=\"cambiaIntervalo(5000)\">"));
  client_web.print(F("</div><div class=\"cabecera\">Boton Manual</div><div id=\"boton\" onmouseover=\"cambiaIntervalo(0)\" onmouseout=\"cambiaIntervalo(5000)\""));
  client_web.print(F("onclick=\"boton_manual()\" "));
  manual ? client_web.print(F("class=\"ON\">ON")) : client_web.print(F("class=\"OFF\">OFF"));
  client_web.println(F("</div></div></div></body></html>"));
  client_web.flush();
  client_web.stop();  //Cierro el cliente
}

void muestroAjax(EthernetClient &client_ajax) {
#if DEBUG
  Serial.println(F("Muestro ajax"));
#endif
  client_ajax.println(F("HTTP/1.1 200 OK"));
  client_ajax.println(F("Content-Type: application/json; charset=utf-8"));
  client_ajax.println();
  client_ajax.print(F("{\"arduino\":{\"A0\":\""));
  client_ajax.print(analogRead(A0));
  client_ajax.print(F("\",\"A1\":\""));
  client_ajax.print(analogRead(A1));
  client_ajax.print(F("\",\"A2\":\""));
  client_ajax.print(analogRead(A2));
  client_ajax.print(F("\",\"led\":\""));
  led ? client_ajax.print(F("ON")) : client_ajax.print(F("OFF"));
  client_ajax.print(F("\",\"manual\":\""));
  manual ? client_ajax.print(F("ON")) : client_ajax.print(F("OFF"));
  client_ajax.print(F("\",\"umbral\":\""));
  client_ajax.print(umbral);
  client_ajax.println(F("\"}}"));
  client_ajax.flush();
  client_ajax.stop();  //Cierro el cliente
}

void cambioManual(EthernetClient &client_ajax, String valor) {
  boolean resultado = 1;
  if (valor == "ON") {
    manual = 1;
    led = 1;
    digitalWrite(13, led);
    Serial.println("Enciendo Led Manualmente");
  }
  else if (valor == "OFF") {
    manual = 0;
    Serial.println("Paso a modo Automático");
  }
  else resultado = 0;
  client_ajax.println(F("HTTP/1.1 200 OK"));
  client_ajax.println(F("Content-Type: text/html\r\n"));
  client_ajax.println();
  resultado ? client_ajax.println(F("OK")) : client_ajax.println(F("KO"));
  client_ajax.flush();
  client_ajax.stop();  //Cierro el cliente
}

void cambioUmbral(EthernetClient &client_ajax, String valor) {
  boolean resultado = 1;
#if DEBUG
  Serial.println(valor);
#endif
  if ((valor.toInt() > 1023) || (valor.toInt() < 0)) resultado = 0;
  else umbral = valor.toInt();
  client_ajax.println(F("HTTP/1.1 200 OK"));
  client_ajax.println(F("Content-Type: text/html\r\n"));
  client_ajax.println();
  resultado ? client_ajax.println(F("OK")) : client_ajax.println(F("KO"));
  client_ajax.flush();
  client_ajax.stop();  //Cierro el cliente
}


//----------------------- SETUP ----------------------------//
void setup()
{
  Serial.begin(9600);
  Serial.println(F("Ejemplo Ajax en Arduino"));
  Serial.print(F("Version: "));
  Serial.println(VERSION);
  Ethernet.begin(mac, ip, dns_server, gateway, subnet);
  server.begin();
  pinMode(13, OUTPUT);	//Built-in Led
  digitalWrite(13, led);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

//----------------------- LOOP -----------------------------//
void loop()
{
  //Lectura datos
  int lectura_A0 =  analogRead(A0);
  int lectura_A1 =  analogRead(A1);
  int lectura_A2 =  analogRead(A2);

  if (((lectura_A1) > umbral) && !led && !manual) //Solo regulo si el no está en manual
  {
    led = 1;
    digitalWrite(13, led);
    Serial.println("Enciendo Led");
  }
  if (((lectura_A1) < (umbral - 100)) && led && !manual) //hiteresis = 100
  {
    led = 0;
    digitalWrite(13, led);
    Serial.println("Apago Led");
  }

  //WEB
  EthernetClient client = server.available(); //Cliente conectado al servidor
  if (client) {
    String request = "";
    while (client.available()) {
      char c = client.read();
      if (c == '\n') break;
      else request += c;
    }
    while (client.available()) {  //como client.flush() no funciona, vacio el buffer
      client.read();
    }
#if DEBUG
    Serial.print(F("------------------------>  "));
    Serial.println(request);
#endif
    if (request.startsWith("GET / HTTP")) muestroWeb(client);
    else if (request.startsWith("GET /ajax HTTP")) muestroAjax(client);
    else if (request.startsWith("GET /boton/")) cambioManual(client, request.substring(16, request.indexOf(" HTTP/1.1")));
    else if (request.startsWith("GET /umbral/")) cambioUmbral(client, request.substring(12, request.indexOf(" HTTP/1.1")));
    else {
      //NO MUESTRO NADA
      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/html\r\n"));
      client.println();
      client.println(F("Peticion no registrada"));
#if DEBUG
      Serial.println(F("Peticion no registrada"));
#endif
      client.flush();
      client.stop();  //Cierro el cliente
    }
  }
}
