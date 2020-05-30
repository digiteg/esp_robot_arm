
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
    return x + y;
}

test(addnum)
{
    assertEqual(addnum(10, 10), 20);
}

#endif

/*
// The braccio Say 'Ciao' with the Tongue
void sayCiao() {

  Braccio.ServoMovement(20,           90,  0, 180, 160,  0,  15);

  for (int i = 0; i < 5; i++) {
    Braccio.ServoMovement(10,           90,  0, 180, 160,  0,  15);
    delay(500);

    Braccio.ServoMovement(10,     90,  0,   180,   160,  0,   73);
    delay(500);
  }
}

// Braccio take the Sponge
void takesponge() {
  //starting position
//(step delay  M1 , M2 , M3 , M4 , M5 , M6);
  Braccio.ServoMovement(20,           0,  45, 180, 180,  90,  0);

  //I move arm towards the sponge
  Braccio.ServoMovement(20,           0,  90, 180, 180,  90,   0);

  //the gripper takes the sponge
  Braccio.ServoMovement(20,           0,  90, 180, 180,  90,  60 );

  //up the sponge
  Braccio.ServoMovement(20,         0,   45, 180,  45,  0, 60);
}


// Braccio show the sponge to the user
void showsponge() {
  for (int i = 0; i < 2; i++) {

    //(step delay  M1 , M2 , M3 , M4 , M5 , M6 );
    Braccio.ServoMovement(10,         0,   45, 180,  45,  180, 60);

    Braccio.ServoMovement(10,         0,   45, 180,  45,  0, 60);
  }
}

// Braccio throw away the sponge
void throwsponge() {
//(step delay  M1 , M2 , M3 , M4 , M5 , M6 );
  Braccio.ServoMovement(20,         0,   45, 90,  45,  90, 60);

  Braccio.ServoMovement(5,         0,   45, 135,  90,  90, 60);

  Braccio.ServoMovement(5,         0,   90, 150,  90,  90, 0);
}
*/

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
