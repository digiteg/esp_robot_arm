
#ifndef DEBUG
#define DEBUG 1
#endif

#ifndef BRACCIO_UNIT_TEST
//#define BRACCIO_UNIT_TEST 1
#endif

#ifndef BRACCIO_DEMO
//#define  BRACCIO_DEMO 1
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

test(addnum) { assertEqual(addnum(10, 10), 20); }

#endif

#ifdef BRACCIO_DEMO

// Ciao Multistask ..
unsigned long previousTime = 0;
long timeInterval = 500;

int ciao_counter = 0;

int demo_step = -1;
int demo_counter = 0;

/*
 Examples for testing of basic  functionality
   The braccio Say 'Ciao' with the Tongue
   Braccio take the Sponge
   Braccio show the sponge to the user

*/

// The braccio Say 'Ciao' with the Tongue
bool sayCiao()
{

  unsigned long currentTime = millis();

  if (ciao_counter == 0)
  {
    BraccioI2C.servoMovement(20, 90, 0, 180, 160, 0, 15);
    ciao_counter++;
    previousTime = currentTime;
    return false;
  }

  // scheduled task executed afther wait interval timeIntervalServoMove
  if (currentTime - previousTime > timeInterval)
  {

    previousTime = currentTime; // restart time counter
    if (ciao_counter == 1)
      BraccioI2C.servoMovement(10, 90, 0, 180, 160, 0, 15);
    else if (ciao_counter == 2)
      BraccioI2C.servoMovement(10, 90, 0, 180, 160, 0, 73);

    ciao_counter++;

    if (ciao_counter >= 3)
    {
      ciao_counter = 1;
      return true;
    }
  }

  return false;
}

// Braccio take the Sponge
void takesponge()
{
  // starting position
  //(step delay  M1 , M2 , M3 , M4 , M5 , M6);
  BraccioI2C.servoMovement(20, 0, 45, 180, 180, 90, 0);

  // I move arm towards the sponge
  BraccioI2C.servoMovement(20, 0, 90, 180, 180, 90, 0);

  // the gripper takes the sponge
  BraccioI2C.servoMovement(20, 0, 90, 180, 180, 90, 60);

  // up the sponge
  BraccioI2C.servoMovement(20, 0, 45, 180, 45, 0, 60);
}

// Braccio show the sponge to the user
void showsponge()
{
  for (int i = 0; i < 2; i++)
  {

    //(step delay  M1 , M2 , M3 , M4 , M5 , M6 );
    BraccioI2C.servoMovement(10, 0, 45, 180, 45, 180, 60);

    BraccioI2C.servoMovement(10, 0, 45, 180, 45, 0, 60);
  }
}

// Braccio throw away the sponge
void throwsponge()
{
  //(step delay  M1 , M2 , M3 , M4 , M5 , M6 );
  BraccioI2C.servoMovement(20, 0, 45, 90, 45, 90, 60);

  BraccioI2C.servoMovement(5, 0, 45, 135, 90, 90, 60);

  BraccioI2C.servoMovement(5, 0, 90, 150, 90, 90, 0);
}

void demo()
{
  switch (demo_step)
  {
  case 0:
    if (demo_counter < 5)
    {
      if (sayCiao())
        demo_counter++;
    }
    else
      demo_step++;
    break;

  case 1:
    takesponge();
    demo_step++;
    break;
  case 2:
    showsponge();
    demo_step++;
    break;
  case 3:
    throwsponge();
    demo_step++;
    break;
  }
}

#endif // BRACCIO_DEMO

/*
  Main implemenation

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

  // put your main code here, to run repeatedly:
  BraccioI2C.loop();

#ifdef BRACCIO_DEMO

  if (demo_step < 0)
  {
#ifdef DEBUG
    Serial.println("Demo.. start");
#endif
    demo_step = 0;
  }
  demo();
  if (demo_step == 4)
  {
#ifdef DEBUG
    Serial.println("Demo.. finished");
#endif
    demo_step++;
  )
#endif

#ifdef BRACCIO_UNIT_TEST 
  TestRunner::run();
#endif
  }
