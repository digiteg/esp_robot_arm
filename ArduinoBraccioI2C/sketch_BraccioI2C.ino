
//#include "OOP.h"
#include "BraccioI2C.h"

#ifndef DEBUG
#define DEBUG 1
#endif

using namespace std;

template <typename T>
class testclass
{
public:
    T namelist[10];

    void setname(T name)
    {
        for (int i = 0; i < 10; i++)
        {
            namelist[i] = name;
        }
    }
};

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

    testclass<int> test;

    test.setname(10);

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
