
#ifndef DEBUG
#define DEBUG 1
#endif

#ifndef BRACCIO_UNIT_TEST
//#define BRACCIO_UNIT_TEST 0
#endif

//#include "OOP.h"


#include "BraccioI2C.h"

#ifdef BRACCIO_UNIT_TEST
#include <AUnit.h>
using namespace aunit;
#endif




_BraccioI2C BraccioI2C; // Definition

#ifdef BRACCIO_UNIT_TEST
int addnum(int x, int y)
{
    return x+y;
}



test(addnum) {
    assertEqual(addnum(10, 10), 20);
}

#endif

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

#ifdef BRACCIO_UNIT_TEST
     TestRunner::run();
#endif

    // put your main code here, to run repeatedly:
    BraccioI2C.loop();
    
}

