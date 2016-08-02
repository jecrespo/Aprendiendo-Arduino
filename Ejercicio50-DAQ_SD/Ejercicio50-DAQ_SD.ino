//Data Acquisition (DAQ) a tarjeta SD
//Ejemplo para leer y guardar en una tarjeta SD cada 30 segundos la tempertura y humedad de una sonda DHT22
//Los datos son guardados en un fichero llamado datalog.csv
//Los datos también son mostrados por en una pantalla LCD modelo https://www.sparkfun.com/products/9395
//Para leer los datos de SD usar sketck: https://www.arduino.cc/en/Tutorial/DumpFile
#include <dht.h>
#include <SPI.h>
#include <SD.h>
#include <MsTimer2.h>

const int chipSelect = 4;
const int dht_pin = 8;

dht DHT;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  MsTimer2::set(30000, saveData); // 30 sec period
  MsTimer2::start();

  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  if (!SD.exists("datalog.csv")) {
    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    dataFile.println("Time(sec),Temperature,Humidity");
    Serial.println("Creo nuevo fichero");
    dataFile.close();
  }
  else Serial.println("Fichero existente");
}

void loop() {
  //mando al display la información
  Serial.write("                "); // clear display
  Serial.write("                ");
  Serial.write(254); // move cursor to beginning of first line
  Serial.write(128);
  int chk = DHT.read22(dht_pin);
  if (chk == DHTLIB_OK) {
    Serial.print("Temp: " + String(DHT.temperature));
    Serial.write(254); // move cursor to beginning of second line
    Serial.write(192);
    Serial.print("Humedad: " + String(DHT.humidity));
  }
  else {
    Serial.print("Error sonda");
  }
  delay(1000);
}

void saveData()
{
  // make a string for assembling the data to log:
  String dataString = String(millis() / 1000) + ",";

  dataString = dataString + DHT.temperature + "," + DHT.humidity + "," ;

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
