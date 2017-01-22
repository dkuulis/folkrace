#include <SD.h>
#include <SPI.h>

#include "const.h"

// SD card SPI chip select (CS) is BUILTIN_SDCARD
#define SDCARD_CS BUILTIN_SDCARD

static Sd2Card card;
static SdVolume volume;

static File logFile;
static int log_id = 1;
static char logname[13]; // 8.3 filename and trailing 0

void sdCardSetup()
{
    if (!SD.begin(SDCARD_CS))
    {
        setupFail("Card failed, or not present");
    }

    openLog();
}

void formatName(int nr)
{
    strcpy(logname, "log");
    int x = strlen(logname);
    itoa(nr, logname+x, 10);
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
        formatName(log_id);
        log_id += 1;
        message("Trying logfile %s\n", logname);
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
        message("Failed to open %s\n", logname);
    }
}

void sdCardPrint(const char* msg)
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

void dumpFile(const char* command)
{
    closeLog();

    if (command[0] == ' ')
    {
        int nr;
        sscanf(command+1, "%i", &nr);
        formatName(nr);
    }

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(logname);

    // if the file is available, read from it:
    if (dataFile)
    {
        message("Dumping %s\n", logname);
        Serial.println("------------------------");
        while (dataFile.available())
        {
            Serial.write(dataFile.read());
        }
        dataFile.close();
        Serial.println("------------------------");
    }
    else
    {
        message("Cannot dump %s\n", logname);
    }

    openLog();
}

void listFiles()
{
    /*
    if (!card.init(SPI_HALF_SPEED, SDCARD_CS))
    {
        setupFail("Card failed, or not present");
    }

    if (!volume.init(card))
    {
        setupFail("Could not find FAT partition");
        return;
    }

    SdFile root;
    root.openRoot(volume);
    root.ls(LS_SIZE, 2);
    root.close();
    */
}

