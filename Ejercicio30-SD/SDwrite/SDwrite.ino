#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

//Texto a escribir
char texto[] = "<!DOCTYPE html>/r/n<html>/r/n<head>/r/n<title>Arduino SD Card Web Page</title>/r/n</head>/r/n<body>/r/n<h1>Hello from the Arduino SD Card!</h1>/r/n<p>A web page from the Arduino SD card server.</p>/r/n</body>/r/n</html>";
//nombre fichero
char filename[] = "index.htm";

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  Serial.println("Escribiendo en tarjeta SD...");
  Serial.println("Fichero: " + (String)filename);
  Serial.println("Contenido: " + (String)texto);

  File dataFile = SD.open(filename, FILE_WRITE);

  if (dataFile) {
    dataFile.println(texto);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void loop()
{
  //nada
}

