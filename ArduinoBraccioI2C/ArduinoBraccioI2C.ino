
//#include "OOP.h"
#include "BraccioI2C.h"

#ifndef DEBUG
#define DEBUG 1
#endif

_BraccioI2C BraccioI2C; // Definition

void setup()
{
    // put your setup code here, to run once:

    BraccioI2C.setup(100);
    delay(100); // sleep for while

#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Setup.. Done");
    Serial.println("Listening .. On");
#endif
}

void loop()
{
    // put your main code here, to run repeatedly:
    BraccioI2C.loop();
    
}
