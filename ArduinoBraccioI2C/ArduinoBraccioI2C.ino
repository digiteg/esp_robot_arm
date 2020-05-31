
#ifndef DEBUG
#define DEBUG 1
#endif

#ifndef BRACCIO_UNIT_TEST
//#define BRACCIO_UNIT_TEST 1
#endif

#ifndef BRACCIO_DEMO
#define BRACCIO_DEMO 1
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
long timeInterval = 1000;

int ciao_counter = 0;

int demo_step = 0;
int demo_counter = 0;

/*
 Examples for testing of basic  functionality
   The braccio Say 'Ciao' with the Tongue
   Braccio take the Sponge
   Braccio show the sponge to the user

*/

// The braccio Say 'Ciao' with the Tongue

void resetTimer()
{
    previousTime= millis();
}

bool delayLoop()
{

  unsigned long currentTime = millis();

	// scheduled task executed afther wait interval timeIntervalServoMove
	if (currentTime - previousTime > timeInterval)
	{
      previousTime = currentTime;
      return true;
  }
  return false;
}


void sayCiao()
{

    for (int i=0; i<3; i++)
    {
            BraccioI2C.servoMovement(20, 0, 15, 175, 0, 175, 68);
            BraccioI2C.servoMovement(20, 175, 165, 0, 175, 0, 10);

    }
}

// Braccio take the Sponge
void takesponge()
{

  Serial.println("-----------------------------------Take sponge------------------------------");

  /*
  Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
  M1=base degrees. Allowed values from 0 to 180 degrees
  M2=shoulder degrees. Allowed values from 15 to 165 degrees
  M3=elbow degrees. Allowed values from 0 to 180 degrees
  M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
  M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
  M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */

  //Starting position
  //(step delay  M1 , M2 , M3 , M4 , M5 , M6);
  BraccioI2C.servoMovement(20, 0, 45, 180, 180, 90, 10);

  //The braccio moves to the sponge. Only the M2 servo will moves
  BraccioI2C.servoMovement(20, 0, 90, 180, 180, 90, 10);

  //Close the gripper to take the sponge. Only the M6 servo will moves
  BraccioI2C.servoMovement(10, 0, 90, 180, 180, 90, 60);

  //Brings the sponge upwards.
  BraccioI2C.servoMovement(20, 0, 45, 180, 45, 0, 60);

  //Show the sponge. Only the M1 servo will moves
  BraccioI2C.servoMovement(20, 180, 45, 180, 45, 0, 60);

  //Return to the start position.
  BraccioI2C.servoMovement(20, 0, 90, 180, 180, 90, 60);

  //Open the gripper
  BraccioI2C.servoMovement(20, 0, 90, 180, 180, 90, 10);
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
    Serial.println("-----------------------------------BEGIN------------------------------");
    BraccioI2C.begin(SOFT_START_DEFAULT);
    demo_step++;
    break;

  case 1:
    
    if (!BraccioI2C.isProcessing() && delayLoop())
      demo_step++;
    break;
  case 2:
    Serial.println("-----------------------------------CIAO------------------------------");
    sayCiao();
    demo_step++;
    break;

  case 3:
    if (!BraccioI2C.isProcessing() && delayLoop())
      demo_step++;
    break;

  case 4:
    takesponge();
    demo_step++;
    break;

  case 5:
    if (!BraccioI2C.isProcessing() && delayLoop())
      demo_step++;
    break;

  case 6:
    // showsponge();
 //   BraccioI2C.stop();
    demo_step++;
    break;
  case 7:
    //  throwsponge();
 //   BraccioI2C.powerOff();
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

  demo();

#ifdef BRACCIO_UNIT_TEST
  TestRunner::run();
#endif
}
