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

    openLog();
}

static File logFile;

void openLog()
{
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    logFile = SD.open("datalog.txt", FILE_WRITE);

    // check if file is opened
    if (!logFile)
    {
        setupFail("error opening data log");
    }
}

void sdCardPrint(const char* msg)
{
    // if the file is available, write to it
    if (logFile)
    {
        logFile.print(msg);
        // no flush() - use newline version
    }
}

void sdCardPrintln(const char* msg)
{
    // if the file is available, write to it
    if (logFile)
    {
        logFile.println(msg);
        logFile.flush();
    }
}

void closeLog()
{
    logFile.close();
}

void dumpFile()
{
    if (logFile)
    {
        closeLog();
    }

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt");

    // if the file is available, read from it:
    if (dataFile)
    {
        while (dataFile.available())
        {
            Serial.write(dataFile.read());
        }
        dataFile.close();
    }

    openLog();
}
