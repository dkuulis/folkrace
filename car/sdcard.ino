#include <SD.h>
#include <SPI.h>

#include "const.h"

// SD card SPI chip select (CS) is BUILTIN_SDCARD
#define SDCARD_CS BUILTIN_SDCARD

void sdCardSetup()
{
    if (!SD.begin(SDCARD_CS)) 
    {
        setupFail("Card failed, or not present");
    }
}
