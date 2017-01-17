void setup()  
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    Serial.println(F("Ready"));

    // set the data rate for the BT Serial port
    Serial4.begin(9600);
    Serial4.println("Hello, world?");
}

/*
  
AT+VERSION - Returns the software version of the module - OKlinvorV1.x
AT+BAUDx - Sets the baud rate of the module - OK115200 
  The command AT+BAUD8 sets the baud rate to 115200
  1 >> 1200 
  2 >> 2400 
  3 >> 4800 
  4 >> 9600 (Default) 
  5 >> 19200 
  6 >> 38400 
  7 >> 57600 
  8 >> 115200 
  9 >> 230400

AT+NAMExxxxx - Sets the name of the module, any name can be specified up to 20 characters - OKsetname
AT+PINxxxx Sets the pairing password, any 4 digit number can be used, the default is 1234 - OKsetPIN
AT+PN Sets the parity of the module - OK None
  AT+PN >> No parity check
  
*/

void loop() // run over and over
{
    if (Serial4.available())
    {
        Serial.write(Serial4.read());
    }
    
    if (Serial.available()) 
    {
        Serial4.write(Serial.read());
    }
}


