/* LoadAndSaveSettings
 * footswitch 2012-03-05, original code by Joghurt (2010)
 * Demonstrates how to load and save settings to the EEPROM
 * Tested on Arduino Uno R2 with Arduino 0023
 */
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>

// ID of the settings block
#define CONFIG_VERSION "ls1"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

// Example settings structure
struct StoreStruct {
  // The variables of your settings
  int a, b;
  char c;
  long d;
  float e[6];
  // This is for mere detection if they are your settings
  char version_of_program[4]; // it is the last variable of the struct
  // so when settings are saved, they will only be validated if
  // they are stored completely.
} settings = {
  // The default values
  220, 1884,
  'c',
  10000,
  {4.5, 5.5, 7, 8.5, 10, 12},
  CONFIG_VERSION
};

void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (//EEPROM.read(CONFIG_START + sizeof(settings) - 1) == settings.version_of_program[3] // this is '\0'
      EEPROM.read(CONFIG_START + sizeof(settings) - 2) == settings.version_of_program[2] &&
      EEPROM.read(CONFIG_START + sizeof(settings) - 3) == settings.version_of_program[1] &&
      EEPROM.read(CONFIG_START + sizeof(settings) - 4) == settings.version_of_program[0])
  { // reads settings from EEPROM
    for (unsigned int t=0; t<sizeof(settings); t++)
      *((char*)&settings + t) = EEPROM.read(CONFIG_START + t);
  } else {
    // settings aren't valid! will overwrite with default settings
    saveConfig();
  }
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(settings); t++)
  { // writes to EEPROM
    EEPROM.write(CONFIG_START + t, *((char*)&settings + t));
    // and verifies the data
    if (EEPROM.read(CONFIG_START + t) != *((char*)&settings + t))
    {
      // error writing to EEPROM
    }
  }
}


void setup() {
  loadConfig();
}

void loop() {
  // [...]
  int i = settings.c - 'a';
  // [...]

  // [...]
  settings.c = 'a';
  if (some_condition)
    saveConfig();
  // [...]
}