
//#include "OOP.h"
#include "BraccioI2C.h"

#ifndef DEBUG
#define DEBUG 1
#endif



#ifdef DEBUG
void printStatus(RobotArmStatus rstat)
{
    Serial.println("Robot Status: -------------");
    Serial.println(rstat.cmd);

    Serial.println(rstat.delay);
    Serial.println(rstat.base);
    Serial.println(rstat.shoulder);
    Serial.println(rstat.elbow);
    Serial.println(rstat.wrist_rot);
    Serial.println(rstat.wrist_ver);
    Serial.println(rstat.gripper);

    Serial.println(rstat.soft_start_level);
    Serial.println(rstat.isinit);
}
#endif

volatile _BraccioI2C BraccioI2C; // Definition

void setup()
{
    // put your setup code here, to run once:

   

    BraccioI2C.setupI2C(100);
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
    BraccioI2C.loopI2C();
    //Serial.println(BraccioI2C.robotStatus.isNewCmd);
    delay(100);
}
