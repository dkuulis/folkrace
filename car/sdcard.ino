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
static int log_id = 1;
static char logname[13]; // 8.3 filename and trailing 0

void formatName()
{
    strcpy(logname, "log");
    int x = strlen(logname);
    itoa(log_id, logname+x, 10);
    strcat(logname, ".txt");
}

void sdCardEeprom(int action)
{
    eepromRW(EEPROM_LOG_ID, 1, log_id, action);
}

// find unused logfile name
void findName()
{
    sdCardEeprom(EEPROM_READ);
    do 
    {
        formatName();
        log_id += 1;
        datalog("Trying logfile ", logname, LOG_WARNING);
    } while (SD.exists(logname));
    sdCardEeprom(EEPROM_WRITE);
}

void openLog()
{
    findName();
    
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    logFile = SD.open(logname, FILE_WRITE);

    // check if file is opened
    if (!logFile)
    {
        datalog("Failed to open logfile ", logname, LOG_ERROR);
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
    if (logFile)
    {
        logFile.close();
    }
}

void dumpFile()
{
    closeLog();

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(logname);

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
