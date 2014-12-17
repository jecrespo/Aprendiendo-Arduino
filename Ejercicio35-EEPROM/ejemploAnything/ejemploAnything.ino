#include <EEPROM.h>
#include "EEPROMAnything.h"

struct config_t
{
    long alarm;
    int mode;
} configuration;

void setup()
{
    EEPROM_readAnything(0, configuration);
    // ...
}
void loop()
{
    // let the user adjust their alarm settings
    // let the user adjust their mode settings
    // ...

    // if they push the "Save" button, save their configuration
    if (digitalRead(13) == HIGH)
        EEPROM_writeAnything(0, configuration);
}