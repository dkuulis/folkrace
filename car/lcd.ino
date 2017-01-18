#include <i2c_t3.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "const.h"

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(-1); // no reset pin

void lcdSetup()
{
    Wire1.begin(I2C_MASTER, 0, I2C_PINS_37_38, I2C_PULLUP_EXT, 400000); // enable LCD comm
    
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an splashscreen
    // internally, this will display the splashscreen.
    display.display();
}

void lcdLoop(unsigned long time, int mode)
{
}
