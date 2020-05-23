/*
    
    BraccioI2C.cpp - Braccio arm I2C communication  library Version 1.0
    Written by Milan Varga

*/

#include <Arduino.h>
#include <Wire.h>

#include "Braccio.h"

#include "BraccioI2C.h"
#include "RobotArmCmd.h"
#include "CircularBuffer.h"



volatile int callCounter = 0; // holding count of executed I2C calls

_CircularBuffer<robotArmCmd> CircularBuffer; // ??? keep me out

//----------------------------------------

// read from I2C
void _BraccioI2C::receiveCommand(int count)
{
    byte ReadBuff[10];

    if (count > 0)
    {
        Wire.readBytes(ReadBuff, count < 8 ? count : 8);

        if (ReadBuff[0] == 0x4D || ReadBuff[0] == 0x42)
            CircularBuffer.writeCircBuff(ReadBuff); // Move cmd & Begin cmd
    }

    callCounter++;
}

//Initialize Braccio I2C object
_BraccioI2C::_BraccioI2C()
{

    robotStatus.cmd = 0;
    robotStatus.soft_start_level = 0;
    robotStatus.isinit = false;
    robotStatus.isNewCmd = false;
}

void _BraccioI2C::setupI2C(int devicenum)
{
    Wire.begin(devicenum);                            // join i2c bus (devicenum ID for master)
    Wire.onReceive((void (*)(int)) & receiveCommand); // Magic added here = register event

    BraccioRobot.setupBraccioRobot();

    //     Wire.onReceive( &receiveCommand  ); // Magic added here = register event
}

RobotArmStatus _BraccioI2C::getRobotArmStatus()
{
    return robotStatus;
}

void _BraccioI2C::loopI2C()
{
    // put your main code here, to run repeatedly:

    if (!CircularBuffer.IsNotEmptycircBuff())
        return;

    robotArmCmd *r = CircularBuffer.readCircBuff();

    Serial.print("Robot Status: ");
    Serial.println(r->cmd);

    Serial.println(r->delay);
    Serial.println(r->base);
    Serial.println(r->shoulder);
    Serial.println(r->elbow);
    Serial.println(r->wrist_rot);
    Serial.println(r->wrist_ver);
    Serial.println(r->gripper);

    if (r->cmd == 0x4D) // New cmd.. M
    {
        robotStatus = *r;
        robotStatus.isNewCmd = true;

        BraccioRobot.ServoMovement(robotStatus.delay, robotStatus.base, robotStatus.shoulder, robotStatus.elbow, robotStatus.wrist_rot, robotStatus.wrist_ver, robotStatus.gripper);

        robotStatus.isNewCmd = false;
    }
    else if (r->cmd == 0x42 && !isArmInit) // New cmd.. B
    {
        robotStatus = *r;
        robotStatus.soft_start_level = r->getSoft_Start_Level();

        BraccioRobot.begin(robotStatus.soft_start_level);
        Serial.println(robotStatus.soft_start_level);

        robotStatus.isinit = true;
        isArmInit = true;
    }
}
