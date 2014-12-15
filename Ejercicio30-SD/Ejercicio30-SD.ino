/*--------------------------------------------------------------
  Program:      eth_websrv_SD_link

  Description:  Arduino web server that serves up a basic web
                page that links to a second page. Clicking the
                link will open the second page. The second page
                links back to the first page.
  
  Hardware:     Arduino Uno and official Arduino Ethernet
                shield. Should work with other Arduinos and
                compatible Ethernet shields.
                2Gb micro SD card formatted FAT16
                
  Software:     Developed using Arduino 1.0.5 software
                Should be compatible with Arduino 1.0 +
                
                Requires index.htm and page2.htm to be on the
                micro SD card in the Ethernet shield micro
                SD card socket.
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - SD card examples by David A. Mellis and
                  Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet
                - SD Card library documentation:
                  http://arduino.cc/en/Reference/SD

  Date:         2 March 2013
  Modified:     14 June 2013
                - removed use of String class, used too much SRAM
                - added StrClear() and StrContains() functions
                - disable Ethernet chip at startup
 
  Author:       W.A. Smith, http://startingelectronics.com
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// size of buffer used to capture HTTP requests
#define REQ_BUF_SZ   20

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 20);   // IP address, may need to change depending on network
EthernetServer server(80);       // create a server at port 80
File webFile;                    // handle to files on SD card
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

void setup()
{
    // disable Ethernet chip
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    
    Serial.begin(9600);       // for debugging
    
    // initialize SD card
    Serial.println("Initializing SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
    Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
    }
    Serial.println("SUCCESS - Found index.htm file.");

    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
}

void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // buffer first part of HTTP request in HTTP_req array (string)
                // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                Serial.print(c);    // print HTTP request character to serial monitor
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connnection: close");
                    client.println();
                    // open requested web page file
                    if (StrContains(HTTP_req, "GET / ")
                                 || StrContains(HTTP_req, "GET /index.htm")) {
                        webFile = SD.open("index.htm");        // open web page file
                    }
                    else if (StrContains(HTTP_req, "GET /page2.htm")) {
                        webFile = SD.open("page2.htm");        // open web page file
                    }
                    // send web page to client
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read());
                        }
                        webFile.close();
                    }
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}